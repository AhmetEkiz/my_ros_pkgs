#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

class Stopper
{
public:
	// Tunable parameters
	static constexpr double FORWARD_SPEED_MPS = 0.5;

	// front of the robot
	static constexpr double MIN_SCAN_ANGLE_RAD = -30.0 / 180 * M_PI;
	static constexpr double MAX_SCAN_ANGLE_RAD = +30.0 / 180 * M_PI;

	static constexpr float MIN_PROXIMITY_RANGE_M = 0.1;  // Should be smaller than sensor_msgs::LaserScan::range_max



	Stopper();
	void startMoving();

private:
	ros::NodeHandle node;
	ros::Publisher commandPub; // Publisher to the robot's velocity command topic
	ros::Subscriber laserSub;  // Subscriber to the robot's laser scan topic
	bool keepMoving;		   // Indicates whether the robot should continue moving
	void moveForward();
	void scanCallback(const sensor_msgs::LaserScan::ConstPtr &scan);
};