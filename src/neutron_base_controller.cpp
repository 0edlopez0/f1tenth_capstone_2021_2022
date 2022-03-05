#include "ros/ros.h"
#include <geometry_msgs/Twist.h>

double angular_v  = 0;

void trajectoryReceived(const geometry_msgs::Twist & state_vel) {
	//implement control system (PID) and control motors via VESC

	int motor_eff = 500;

	int servo_eff = 500;
	
	if ( !((Kp<=0. && Ki<=0. && Kd<=0.) || (Kp>=0. && Ki>=0. && Kd>=0.)) ) // All 3 gains should have the same sign
	{
		ROS_WARN("All three gains (Kp, Ki, Kd) should have the same sign for stability.");
	}
	
	
	motor_setpoint = cmd_vel;
	motor_state = state_vel;

	motor_error.at(2) = motor_error.at(1);
	motor_error.at(1) = motor_error.at(0);
	motor_error.at(0) = motor_setpoint - motor_state; // Current error goes to slot 0

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
	
	// integrate error
	motor_error_integral += motor_error.at(0) * delta_t.toSec();
	// Apply windup limit to limit the size of the integral term
	if (motor_error_integral > fabsf(windup_limit)){
		motor_error_integral = fabsf(windup_limit);
	}
	if (motor_error_integral < -fabsf(windup_limit)){
		motor_error_integral = -fabsf(windup_limit);
	}
	
	// take derivative of error
	motor_error_deriv.at(2) = motor_error_deriv.at(1);
	motor_error_deriv.at(1) = motor_error_deriv.at(0);
	motor_error_deriv.at(0) = (motor_error.at(0)-motor_error.at(1))/delta_t.toSec();
	
	// calculate control effort
	motor_u_p = Kp * error.at(0);
	motor_u_i = Ki * error_integral;
	motor_u_d = Kd * error_deriv.at(0);
	motor_control_effort = motor_u_p + motor_u_i + motor_u_d;
	
	
	// control effort limits
	if (motor_control_effort > upper_limit){
		motor_control_effort = upper_limit;
	} else if (motor_control_effort < lower_limit){
		motor_control_effort = lower_limit;
	}
	
	// publish control effort
	control_effort_pub.publish(motor_control_effort);
	
	
	ros::NodeHandle n;
	//TO DO: This shit below dont work for some reason, I think it has to do with <int> part but idk
	//ros::Publisher pub = n.advertise<int>("control_effort", 1000);

	//TO DO: Can add back once above works
	//pub.publish(motor_eff);
}



int main(int argc, char **argv) {
	ros::init(argc, argv, "neutron_base_controller");

	ros::NodeHandle n;

	// parameters 
	n.param<double>("Kp", Kp, 1.0);
	n.param<double>("Ki", Ki, 0.0);
	n.param<double>("Kd", Kd, 0.0);
	n.param<double>("upper_limit", upper_limit, 100.0);
	n.param<double>("lower_limit", lower_limit, -100.0);
	n.param<double>("windup_limit", windup_limit, 1000.0);
	n.param<double>("cutoff_frequency", cutoff_frequency, -1.0);
	n.param<std::string>("topic_from_controller", topic_from_controller, "control_effort");
	n.param<std::string>("topic_from_plant", topic_from_plant, "state");
	n.param<std::string>("setpoint_topic", setpoint_topic, "setpoint");
	n.param<std::string>("pid_enable_topic", pid_enable_topic, "pid_enable");
	n.param<double>("max_loop_frequency", max_loop_frequency, 1.0);
	n.param<double>("min_loop_frequency", min_loop_frequency, 1000.0);

	// Two parameters to allow for error calculation with discontinous value
	n.param<bool>("angle_error", angle_error, false);
	n.param<double>("angle_wrap", angle_wrap, 2.0*3.14159);
	
	// instantiate publishers and subscribers
	
	ros::Publisher control_effort_pub = n.advertise<int>("control_effort", 1000);
	
	ros::Subscriber sub = n.subscribe("cmd_vel", 1000, trajectoryReceived);
	ros::Subscriber plant_state_sub = n.subscribe("state_vel",1000, trajectoryReceived);
	
	ros::spin();

	return 0;
}
