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
};

void quit(int sig);