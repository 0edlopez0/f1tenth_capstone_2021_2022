#include "ros/ros.h"
#include "std_msgs/String.h"
#include <string>
#include<sstream>
#include <curses.h>
#include <stdlib.h>

#define KEY_UP 72
#define KEY_DOWN 80
#define KEY_LEFT 75
#define KEY_RIGHT 77


int main(int argc, char **argv) {
	ros::init(argc, argv, "manual");

	ros::NodeHandle n;

	ros::Publisher chatter_pub = n.advertise<std_msgs::String>("commands", 1000);

	ros::Rate loop_rate(10);

	while(ros::ok()) {
		int r = rand() % 4 + 1;
		std_msgs::String msg;
		switch(r) {
		case 1:
			msg.data = "UP";
			ROS_INFO("%s", msg.data.c_str());
			chatter_pub.publish(msg);
			break;
		case 2:
			msg.data = "DOWN";
			ROS_INFO("%s", msg.data.c_str());
			chatter_pub.publish(msg);
			break;
		case 3:
			msg.data = "LEFT";
			ROS_INFO("%s", msg.data.c_str());
			chatter_pub.publish(msg);
			break;
		case 4:
			msg.data = "RIGHT";
			ROS_INFO("%s", msg.data.c_str());
			chatter_pub.publish(msg);
			break;
		default:
			msg.data = "-";
			chatter_pub.publish(msg);
			break;
		}

		ros::spinOnce();

		loop_rate.sleep();
	}

	return 0;
}
