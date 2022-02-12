#include "ros/ros.h"
#include <geometry_msgs/Twist.h>

double angular_v  = 0;

void trajectoryReceived(const geometry_msgs::Twist &msg) {
	//implement control system (PID) and control motors via VESC
 	

	



	int motor_eff = 500;

	int servo_eff = 500;



	ros::Publisher pub = n.advertise<int>("control_effort", 1000);

	pub.publish(eff);
}



int main(int argc, char **argv) {
	ros::init(argc, argv, "neutron_base_controller");

	ros::NodeHandle n;

	ros::Subscriber sub = n.subscribe("cmd_vel", 1000, trajectoryReceived);

	ros::spin();

	return 0;
}
