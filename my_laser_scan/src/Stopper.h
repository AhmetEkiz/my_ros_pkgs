#include <ros/ros.h>
#include <sensor_msgs/LaserScan.h>

class Stopper
{
public:

	enum State{
		FORWARD,   // go forward
		STOP_FORWARD,   // stop forward
		TURN,  // need to turn, decide which way to turn
		TURN_RIGHT, 
		TURN_LEFT,
		STOP_TURN,   // stop turn you were there
		STOP,
	};

	const float PI = 3.1415926535897;
	
	// Tunable parameters
	static constexpr double FORWARD_SPEED_MPS = 0.2;  
	static constexpr double SPEED = 10;   //degrees/sec
	static constexpr double ANGLE = 90; //degrees

	static constexpr double TURN_SPEED_MPS = 0.2;
	const int duration_to_turn = 1;   // 1 second
	const int angle = 90;  // angle to turn left or right

	static constexpr float MIN_PROXIMITY_RANGE_M = 0.3;  // Should be smaller than sensor_msgs::LaserScan::range_max

	// my constants for laser scan indexes
	// FRONT Laser Scan Indexes
	const int right_front_start = 0;
	const int right_front_end = 5;

	const int left_front_start = 352;
	const int left_front_end = 359;

	// LEFT Laser Scan Indexes
	const int front_left = 94;
	const int back_left = 82;

	// RIGHT Laser Scan Indexes
	const int front_right = 276;
	const int back_right = 264;

	sensor_msgs::LaserScan::ConstPtr scan_ptr;

	// bool turn = false;  // is it turning right now?
	State v_state = FORWARD;   //vehicle state

	Stopper();
	void startMoving();

private:
	ros::NodeHandle node;
	ros::Publisher commandPub; // Publisher to the robot's velocity command topic
	ros::Subscriber laserSub;  // Subscriber to the robot's laser scan topic

	bool keepMoving;		   // Indicates whether the robot should continue moving
	void moveForward();
	void stop();
	void turn();
	void check_empty(int index_start, int index_end);
	void check_front();
	void turn_sides();
	float find_closest(int minIndex, int maxIndexs);
	void scanCallback(const sensor_msgs::LaserScan::ConstPtr &scan);
	
};