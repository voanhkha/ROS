#include "ros/ros.h"
#include "master.h"

int main(int argc, char **argv) {
        ros::init(argc, argv, "master");

        was_master wasMaster = was_master();

        ros::spin();

        return 0;
}