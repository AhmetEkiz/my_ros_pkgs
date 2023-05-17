
#include <ros/ros.h>
#include <std_msgs/Int32.h>
#include <turtlesim/Pose.h>
#include <iostream> 
#include <std_srvs/Empty.h>

#include <geometry_msgs/Twist.h>


turtlesim::PoseConstPtr g_pose;
turtlesim::PoseConstPtr g_pose_prev;  // previous g_pose 

geometry_msgs::Twist speeds;   // speed of turtle


void poseCallback(const turtlesim::PoseConstPtr& pose)
{
	g_pose = pose;
}

void show_pose(){
	if(!g_pose){
		ROS_INFO("Received pose:\n x:%f\n y:%f\n angular:%f\n linear:%f\n theta:%f", g_pose->x, g_pose->y, g_pose->angular_velocity, g_pose->linear_velocity, g_pose->theta);
	}
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
	
	ros::init(argc, argv, "move_linear");
	ros::NodeHandle nh;
	ros::Subscriber pose_sub = nh.subscribe("turtle1/pose", 1, poseCallback);
	ros::Publisher twist_pub = nh.advertise<geometry_msgs::Twist>("turtle1/cmd_vel", 10);
	ros::ServiceClient reset = nh.serviceClient<std_srvs::Empty>("reset");
	ros::Rate loop_rate(120);

	std_srvs::Empty empty;
	reset.call(empty);

	ROS_INFO("Program STARTED!");

	// show_pose();

	speeds.linear.x = 3;   // move forward
	speeds.linear.y = 0;
	speeds.linear.z = 0;
	speeds.angular.x = 0;   // move forward
	speeds.angular.y = 0;
	speeds.angular.z = 0;

	twist_pub.publish(speeds);  
	ros::Duration(3).sleep();
	speeds.linear.x = 0;   // stop
	// show_pose();

	// ros::spinOnce();



	return 0;
}