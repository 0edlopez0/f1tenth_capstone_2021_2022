#include "ros/ros.h"
#include "std_msgs/String.h"

void callBackFunc(const std_msgs::String::ConstPtr& msg) {
	ROS_INFO("I heard: [%s]", msg->data.c_str());
}

int main(int argc, char **argv) {
	ros::init(argc, argv, "actuators");
	ros::NodeHandle n;
	ros::Subscriber sub = n.subscribe("commands", 1000, callBackFunc);
	ros::spin();
}


