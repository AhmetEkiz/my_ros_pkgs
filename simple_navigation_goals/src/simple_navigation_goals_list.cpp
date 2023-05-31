// base code from:
// https://github.com/ros-planning/navigation_tutorials/blob/indigo-devel/simple_navigation_goals_tutorial/src/simple_navigation_goals.cpp

// send turtlebot to the given list of coordinates in order

#include <ros/ros.h>
#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <tf/transform_datatypes.h>

#include <boost/thread.hpp>

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


void spinThread()
{
	ros::spin();
}


int main(int argc, char **argv)
{
	// // coordinates to go x,y,yaw
	// float coords[3][3] = {{6.3123, -4.4526, -50}, \
    //               {0.8100, 1.6451, 90}, \
    //               {-6.9616, -3.1483, -50}}; 
	
	float coords[3][3] = {{4.8, -5.7, -50}, \
						  {-0.4, 0.9, 90}, \
						  {-7.75, -4.6, 50}}; 

	// float coords[3][3] = {{9.1, -5.45, -50}, \
    //               		  {0.8100, 1.6451, 90}, \
    //               		  {-6.9616, -3.1483, -50}}; 

	// init with first position
	float x=-4.3, y=0.25, yaw;   // to store values  


	ros::init(argc, argv, "simple_navigation_goals"); // created new node

	ros::NodeHandle n; // ROS node object to communicat with ROS

	// use threads from boost - boot kutuphanesini kullanarak thread olusturacagiz
	boost::thread spin_thread = boost::thread(boost::bind(&spinThread));

	// typedef ile degistirilen action lib kutuphanesi ile ac nesnesi tanimlandi
	// it actually contructs action client to communicate with action named "move_base"
	// 	that adheres to the MoveBaseAction interface.
	MoveBaseClient ac("move_base");                           

	// give some time for connections to register
	sleep(2.0);

	// message type 
	move_base_msgs::MoveBaseGoal goal;
	


	// TODO: read position from topic
	// move_base_msgs::MoveBaseGoal goal_prev;    // previous coordinates of vehicle


	// calculate radian

	for(int i=0; i<3; i++){

		x = coords[i][0] - x;
		y = coords[i][1] - y;
		yaw = (coords[i][2]*(M_PI/180));    // convert radian

		ROS_INFO("\nSending: \nx: %f\ny: %f, \nyaw:%f",x,y,yaw);

		// we'll send a goal to the robot to move 2 meters forward
		goal.target_pose.header.frame_id = "base_link";
		goal.target_pose.header.stamp = ros::Time::now();

		goal.target_pose.pose.position.x = x;
		goal.target_pose.pose.position.y = y;

		ROS_INFO("\nPOSE: \nx: %f\ny: %f",goal.target_pose.pose.position.x, \
													goal.target_pose.pose.position.y);

		// this is not drone, so we need to pass just yaw orientation
		goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);

		ROS_INFO("Sending goal");
		ac.sendGoal(goal);

		ac.waitForResult();

		if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
			ROS_INFO("Hooray, the base moved 2 meters forward");
		else
			ROS_INFO("The base failed to move forward 2 meters for some reason");
	}

	return 0;
}