
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <turtlesim/Pose.h>
#include <iostream> 

#include <std_srvs/Empty.h>


turtlesim::PoseConstPtr g_pose;
turtlesim::PoseConstPtr g_pose_prev;  // previous g_pose 


void poseCallback(const turtlesim::PoseConstPtr& pose)
{
	g_pose = pose;
}

void show_pose(){
	ROS_INFO("Received pose:\n x:%f\n y:%f\n angular:%f\n linear:%f\n theta:%f", g_pose->x, g_pose->y, g_pose->angular_velocity, g_pose->linear_velocity, g_pose->theta);
}

void check_pose(){
	/*check pose changes*/

	if (!g_pose)
	{
		// ROS_INFO("g_pose is empty");
		return;
	}

	if (g_pose_prev)
	{	
			// ROS_INFO("both are not empty");
			if(g_pose->x != g_pose_prev->x || g_pose->y != g_pose_prev->y || g_pose->angular_velocity != g_pose_prev->angular_velocity || g_pose->linear_velocity != g_pose_prev->linear_velocity || g_pose->theta != g_pose_prev->theta ){
				show_pose();
			}
			g_pose_prev = g_pose;
	}

	else{
		// ROS_INFO("g_pose_prev is empty");
		g_pose_prev = g_pose;
		show_pose();
	}
}


int main(int argc, char **argv) {
	
	ros::init(argc, argv, "show_pose_topic");
	ros::NodeHandle nh;
	ros::Subscriber pose_sub = nh.subscribe("turtle1/pose", 1, poseCallback);
	ros::ServiceClient reset = nh.serviceClient<std_srvs::Empty>("reset");
	ros::Rate loop_rate(120);

	std_srvs::Empty empty;
	reset.call(empty);

	ROS_INFO("Program STARTED!");

	while (ros::ok())
	{
		check_pose();
		ros::spinOnce();
		loop_rate.sleep();
	}
	return 0;
}