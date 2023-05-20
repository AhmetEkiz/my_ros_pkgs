#include <iostream>
#include "Stopper.h"
#include <geometry_msgs/Twist.h>

using namespace std;

bool scan_came = false;

Stopper::Stopper()
{
	keepMoving = true;
	// Advertise a new publisher for the robot's velocity command topic
	commandPub = node.advertise<geometry_msgs::Twist>("cmd_vel", 10);
	// Subscribe to the simulated robot's laser scan topic
	laserSub = node.subscribe("scan", 1, &Stopper::scanCallback, this);
}


// MOVING FUNCTIONS

// Send a velocity command
void Stopper::moveForward() {
	geometry_msgs::Twist msg; // The default constructor will set all commands to 0
	msg.linear.x = FORWARD_SPEED_MPS;
	commandPub.publish(msg);
};

// stop the vehicle
void Stopper::stop(){
	geometry_msgs::Twist msg; // The default constructor will set all commands to 0
	msg.linear.x = 0;
	msg.angular.z = 0;
	commandPub.publish(msg);
};

// turn vehicle left or right
void Stopper::turn(){
	
	geometry_msgs::Twist msg; // The default constructor will set all commands to 0
	msg.linear.x = 0;

	// TODO: converting ratio:
	// http://wiki.ros.org/turtlesim/Tutorials/Rotating%20Left%20and%20Right
	//Converting from angles to radians
 	float angular_speed = SPEED*2*PI/360;
 	float relative_angle = ANGLE*2*PI/360;

	if(v_state == TURN_LEFT){
		msg.angular.z = +relative_angle;
	}
	else if(v_state == TURN_RIGHT){
		msg.angular.z = -relative_angle;
	}

	double t0 = ros::Time::now().toSec();
	double t1;
	float current_angle = 0;

	while(current_angle < relative_angle){
  		commandPub.publish(msg);
		t1 = ros::Time::now().toSec();
		current_angle = angular_speed*(t1-t0);
		ROS_INFO("current_angle: %f, relative_angle: %f, angular_speed: %f",current_angle, relative_angle, angular_speed);
	}
	ROS_INFO("TURN IS OVER");
	stop();
	v_state == FORWARD;
	ros::Duration(0.1).sleep();
};

// FUNCTIONS

// check given indexes' range is not closer than minimum proximity range
void Stopper::check_empty(int index_start, int index_end){

	ROS_INFO("check scan_ptr");
	
	ROS_INFO("checked scan_ptr");
	float closestRange = find_closest(index_start, index_end);
		
	ROS_INFO_STREAM("Closest range: " << closestRange);

	if (closestRange < MIN_PROXIMITY_RANGE_M) {
		ROS_INFO("STOPPED!");
		v_state = STOP_FORWARD;
		stop();
	}
	
}

void Stopper::check_front(){
	check_empty(right_front_start, right_front_end);
	check_empty(left_front_start, left_front_end);
}

// find closest range 
float Stopper::find_closest(int minIndex, int maxIndex){
	
	// ROS_INFO("Find closest started");
	// ROS_INFO("minIndex %d , maxIndex %d", minIndex, maxIndex);
	// ROS_INFO("Find closest started %f", scan_ptr->ranges[minIndex]);
	float closestRange = scan_ptr->ranges[minIndex];
	// ROS_INFO("scan_ptr is not empty");
	for (int currIndex = minIndex; currIndex <= maxIndex; currIndex++) {

		if (scan_ptr->ranges[currIndex] < closestRange) {
			closestRange = scan_ptr->ranges[currIndex];
		}
	}

	return closestRange;
	
}

// decide which way to turn
void Stopper::turn_sides(){

	
	float closestRange_left = find_closest(back_left, front_left);
	float closestRange_right = find_closest(back_right, front_right);

	if(closestRange_left>=closestRange_right){

		check_empty(back_left, front_left);  //check left side is empty before going it

		ROS_INFO("TURNING LEFT");
		v_state = TURN_LEFT;
	}

	else if(closestRange_left<closestRange_right){

		check_empty(back_right, front_right);   //check right side is empty before going it
		ROS_INFO("TURNING RIGHT");
		v_state = TURN_RIGHT;
	}

}

// Save laser scan message
void Stopper::scanCallback(const sensor_msgs::LaserScan::ConstPtr& scan)
{
	ROS_INFO("scanCallback started");
	scan_ptr = scan;
	scan_came = true;
	ROS_INFO("scanCallback ended");
}

void Stopper::startMoving()
{
	// while(!scan_came){
	// 	ROS_INFO("waiting scan");
	// 	ros::Duration(0.1).sleep();
	// }

	ros::Rate rate(30);
	
	ROS_INFO("Start moving");

	// Keep spinning loop until user presses Ctrl+C or the robot got too close to an obstacle
	while (ros::ok()) {
		// ROS_INFO("IN WHILE");
		if(v_state == FORWARD && scan_came){
			ROS_INFO("check front");
			check_front();
			ROS_INFO("checked front");
			// if it is still FORWARD
			if(v_state == FORWARD){
				moveForward();
				ROS_INFO("moved forward");
			}
		}

		else if(v_state == STOP_FORWARD){
		
			turn_sides();
		}

		else if(v_state == TURN_LEFT || v_state == TURN_RIGHT){
			turn();
		}


		else if(v_state == STOP){
			stop();
			ROS_INFO("Program stopped!");
			break;   // break the program
		}

		ros::spinOnce(); // Need to call this function often to allow ROS to process incoming messages
		rate.sleep();
	}

}