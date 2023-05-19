#include <iostream>
#include "Stopper.h"
#include <geometry_msgs/Twist.h>

using namespace std;


Stopper::Stopper()
{
	keepMoving = true;
	// Advertise a new publisher for the robot's velocity command topic
	commandPub = node.advertise<geometry_msgs::Twist>("cmd_vel", 10);
	// Subscribe to the simulated robot's laser scan topic
	laserSub = node.subscribe("scan", 1, &Stopper::scanCallback, this);
}

// Send a velocity command
void Stopper::moveForward() {
	geometry_msgs::Twist msg; // The default constructor will set all commands to 0
	msg.linear.x = FORWARD_SPEED_MPS;
	commandPub.publish(msg);
};


// Process the incoming laser scan message
void Stopper::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	// Find the closest range between the defined minimum and maximum angles
	// int minIndex = ceil((MIN_SCAN_ANGLE_RAD - scan->angle_min) / scan->angle_increment);
	// int maxIndex = floor((MAX_SCAN_ANGLE_RAD - scan->angle_min) / scan->angle_increment);

	int	minIndex = 0;
	int maxIndex = 359;

	ROS_INFO("\nminIndex: %d\nmaxIndex: %d",minIndex, maxIndex);

	float closestRange = scan->ranges[minIndex];
	
	for (int currIndex = minIndex; currIndex <= maxIndex; currIndex++) {

		// ROS_INFO("\n[CURRENT]\n Index: %d\n Range: %f", currIndex, scan->ranges[currIndex]);
		// ROS_INFO("\n Index: %d Range: %f", currIndex, scan->ranges[currIndex]);
		cout << "\n -- Index:" << currIndex << "  Range:" << scan->ranges[currIndex] << " -- ";

		if (scan->ranges[currIndex] < closestRange) {
			closestRange = scan->ranges[currIndex];
			
		}
	}

	ROS_INFO_STREAM("Closest range: " << closestRange);

	if (closestRange < MIN_PROXIMITY_RANGE_M) {
		ROS_INFO("Stop!");
		
		// keepMoving = false;
	}
}

void Stopper::startMoving()
{
	ros::Rate rate(10);
	ROS_INFO("Start moving");

	// Keep spinning loop until user presses Ctrl+C or the robot got too close to an obstacle
	while (ros::ok() && keepMoving) {
		// moveForward();
		ros::spinOnce(); // Need to call this function often to allow ROS to process incoming messages
		rate.sleep();
	}
}