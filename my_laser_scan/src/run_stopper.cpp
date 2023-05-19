#include "Stopper.h"

int main(int argc, char **argv)
{
	// Initiate new ROS node named "stopper"
	ros::init(argc, argv, "stopper");

	// Create new stopper object
	Stopper stopper;

	ros::Duration(5.0).sleep();  // Sleep for one second

	// Start the movement
	stopper.startMoving();
	
	return 0;
};