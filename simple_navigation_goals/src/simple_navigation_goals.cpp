// base code from:
// https://github.com/ros-planning/navigation_tutorials/blob/indigo-devel/simple_navigation_goals_tutorial/src/simple_navigation_goals.cpp

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

	// we'll send a goal to the robot to move 2 meters forward
	goal.target_pose.header.frame_id = "base_link";
	goal.target_pose.header.stamp = ros::Time::now();

	goal.target_pose.pose.position.x = 2.0;
	goal.target_pose.pose.position.y = 0.2;

	// this is not drone, so we need to pass just yaw orientation
	goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(M_PI);

	ROS_INFO("Sending goal");
	ac.sendGoal(goal);

	ac.waitForResult();

	if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
		ROS_INFO("Hooray, the base moved 2 meters forward");
	else
		ROS_INFO("The base failed to move forward 2 meters for some reason");

	return 0;
}