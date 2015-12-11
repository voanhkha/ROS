#include "master.h"

#define WAS_DEBUG

was_master::was_master(void) {
                range_sub = nh.subscribe("was_sensor/ultrasound",
                                         1000, &was_master::range_cb, this);
                lineTracking_sub = nh.subscribe("was_sensor/lineTracking",
                                                1000, &was_master::lineTracking_cb, this);
                movement_sub = nh.subscribe("was_teleop/movement",
                                        1000, &was_master::movement_cb, this);
                lift_sub = nh.subscribe("was_teleop/lift",
                                        1000, &was_master::lift_cb, this);

                lift_pub = nh.advertise<std_msgs::String>("was_control/lift", 1);
        }

void was_master::range_cb(const std_msgs::UInt16MultiArray::ConstPtr& msg) {
        for (int i = 0; i < msg->data.size(); i++)
        {
        	std::cout << "Rng[" << i << "]: " << msg->data.at(i) << " | ";
        }
        std::cout << std::endl;
}

void was_master::lineTracking_cb(const std_msgs::UInt32::ConstPtr& msg) {
        ROS_INFO("Line Tracking value: [%d]", msg->data);
}

void was_master::movement_cb(const std_msgs::String::ConstPtr& cmd) {
        std::cout << cmd->data << std::endl;
}

void was_master::lift_cb(const std_msgs::String::ConstPtr& cmd) {
        std::cout << cmd->data << std::endl;

        std_msgs::String sending_cmd;
        sending_cmd.data = cmd->data;

#ifdef WAS_DEBUG
        std::cout << "Sending_cmd: " << sending_cmd.data << std::endl;
#endif

        lift_pub.publish(sending_cmd);
}
