#include "ros/ros.h"
#include <geometry_msgs/Twist.h>

double angular_v  = 0;

void trajectoryReceived(const geometry_msgs::Twist &msg) {
	//implement control system (PID) and control motors via VES

	int motor_eff = 500;

	int servo_eff = 500;
	
	if ( !((Kp<=0. && Ki<=0. && Kd<=0.) || (Kp>=0. && Ki>=0. && Kd>=0.)) ) // All 3 gains should have the same sign
	{
		ROS_WARN("All three gains (Kp, Ki, Kd) should have the same sign for stability.");
	}
	
	plant_state = state_msg.data;

	error.at(2) = error.at(1);
	error.at(1) = error.at(0);
	error.at(0) = setpoint - plant_state; // Current error goes to slot 0

	// calculate delta_t
	if (!prev_time.isZero()) // Not first time through the program  
	{
		delta_t = ros::Time::now() - prev_time;
		prev_time = ros::Time::now();
		if (0 == delta_t.toSec())
	{
		ROS_ERROR("delta_t is 0, skipping this loop. Possible overloaded cpu at time: %f", ros::Time::now().toSec());
	       return;
		}
	}
	else
	{
		ROS_INFO("prev_time is 0, doing nothing");
		prev_time = ros::Time::now();
		return;
	}
	

	ros::NodeHandle n;
	//TO DO: This shit below dont work for some reason, I think it has to do with <int> part but idk
	//ros::Publisher pub = n.advertise<int>("control_effort", 1000);

	//TO DO: Can add back once above works
	//pub.publish(motor_eff);
}



int main(int argc, char **argv) {
	ros::init(argc, argv, "neutron_base_controller");

	ros::NodeHandle n;
	ros::NodeHandle node_priv("~");

	// parameters 
	node_priv.param<double>("Kp", Kp, 1.0);
	node_priv.param<double>("Ki", Ki, 0.0);
	node_priv.param<double>("Kd", Kd, 0.0);
	node_priv.param<double>("upper_limit", upper_limit, 1000.0);
	node_priv.param<double>("lower_limit", lower_limit, -1000.0);
	node_priv.param<double>("windup_limit", windup_limit, 1000.0);
	node_priv.param<double>("cutoff_frequency", cutoff_frequency, -1.0);
	node_priv.param<std::string>("topic_from_controller", topic_from_controller, "control_effort");
	node_priv.param<std::string>("topic_from_plant", topic_from_plant, "state");
	node_priv.param<std::string>("setpoint_topic", setpoint_topic, "setpoint");
	node_priv.param<std::string>("pid_enable_topic", pid_enable_topic, "pid_enable");
	node_priv.param<double>("max_loop_frequency", max_loop_frequency, 1.0);
	node_priv.param<double>("min_loop_frequency", min_loop_frequency, 1000.0);

	// Two parameters to allow for error calculation with discontinous value
	node_priv.param<bool>("angle_error", angle_error, false);
	node_priv.param<double>("angle_wrap", angle_wrap, 2.0*3.14159);
	
	
	ros::Subscriber sub = n.subscribe("cmd_vel", 1000, trajectoryReceived);

	ros::spin();

	return 0;
}
