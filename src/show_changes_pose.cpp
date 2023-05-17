
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <turtlesim/Pose.h>
#include <iostream> 

turtlesim::PoseConstPtr g_pose;

void poseCallback(const turtlesim::PoseConstPtr& pose)
{
	g_pose = pose;
	ROS_INFO("Received pose:\n x:%f\n y:%f\n angular:%f\n linear:%f\n theta:%f", pose->x, pose->y, pose->angular_velocity, pose->linear_velocity, pose->theta);
}

int main(int argc, char **argv) {
	
	ros::init(argc, argv, "show_pose_topic");
	ros::NodeHandle nh;
	// subscribe
	ros::Subscriber pose_sub = nh.subscribe("turtle1/pose", 1, poseCallback);
	ros::spin(); 

	return 0;
}