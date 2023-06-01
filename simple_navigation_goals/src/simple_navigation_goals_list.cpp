// base code from:
// https://github.com/ros-planning/navigation_tutorials/blob/indigo-devel/simple_navigation_goals_tutorial/src/simple_navigation_goals.cpp

// send turtlebot3 to the given list of coordinates in order

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
	float coords[3][3] = {{6.3123, -4.4526, -50}, \
                  {0.8100, 1.6451, 90}, \
                  {-6.287, -2.896, -50}}; 
	// float coords[3][3] = {{-6.287, -2.896, -50}, \
    //               {0.8100, 1.6451, 90}, \
	// 			  {6.3123, -4.4526, -50}}; 
	
	float x, y, yaw;   // to store positions  


	ros::init(argc, argv, "simple_navigation_goals"); // created new node

	ros::NodeHandle n;  // ROS node object to communicat with ROS

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


	while(true){
		for(int i=0; i<3; i++){

			x = coords[i][0];
			y = coords[i][1];
			yaw = (coords[i][2]*(M_PI/180));    // convert radian

			ROS_INFO("\nSending: \nx: %f\ny: %f, \nyaw:%f",x,y,yaw);

			// we'll send a goal to the robot to move 2 meters forward. [Select: base_link]
			// we selected map as a origin
			goal.target_pose.header.frame_id = "map";
			goal.target_pose.header.stamp = ros::Time::now();

			goal.target_pose.pose.position.x = x;
			goal.target_pose.pose.position.y = y;

			// this is not drone, so we need to pass just yaw orientation
			goal.target_pose.pose.orientation = tf::createQuaternionMsgFromYaw(yaw);

			ROS_INFO("Sending goal");
			ac.sendGoal(goal);

			ac.waitForResult();

			if (ac.getState() == actionlib::SimpleClientGoalState::SUCCEEDED)
				ROS_INFO("Hooray, Goal Reached Successfully");
			else
				ROS_INFO("The bot failed to move to the goal for some reason");
		}
	}
	return 0;
}