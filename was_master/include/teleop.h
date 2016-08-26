#include "ros/ros.h"

class was_teleop
{
public:
        was_teleop();
        void keyLoop();

private:
        ros::NodeHandle nh;
        ros::Publisher movement_pub;
        ros::Publisher lift_pub;
	ros::Publisher motor_left_pub;
	ros::Publisher motor_right_pub;
	ros::Publisher motor_stop_pub;
        ros::Publisher motor_left_dir_pub;
        ros::Publisher motor_right_dir_pub;

};

void quit(int sig);
