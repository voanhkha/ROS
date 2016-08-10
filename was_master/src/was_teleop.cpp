#include "ros/ros.h"
#include "teleop.h"
#include <termios.h>
#include <signal.h>

int main(int argc, char** argv)
{
        ros::init(argc, argv, "was_teleop");
        was_teleop teleop_cmd;

        signal(SIGINT,quit);

        teleop_cmd.keyLoop();

        return(0);
}
