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
                
		motor_left_sub = nh.subscribe("was_teleop/motor_left",
                                        1000, &was_master::motor_left_cb, this);
		motor_left_pub = nh.advertise<std_msgs::String>("was_control/motor_left", 1);
                
		motor_right_sub = nh.subscribe("was_teleop/motor_right",
                                        1000, &was_master::motor_right_cb, this);
                motor_right_pub = nh.advertise<std_msgs::String>("was_control/motor_right", 1);
                
		motor_stop_sub = nh.subscribe("was_teleop/motor_stop",
                                        1000, &was_master::motor_stop_cb, this);
                motor_stop_pub = nh.advertise<std_msgs::String>("was_control/motor_stop", 1);
		
		motor_right_dir_sub = nh.subscribe("was_teleop/motor_right_dir",
                                        1000, &was_master::motor_right_dir_cb, this);
                motor_right_dir_pub = nh.advertise<std_msgs::String>("was_control/motor_right_dir", 1);

                motor_left_dir_sub = nh.subscribe("was_teleop/motor_left_dir",
                                        1000, &was_master::motor_left_dir_cb, this);
                motor_left_dir_pub = nh.advertise<std_msgs::String>("was_control/motor_left_dir", 1);

        }

void was_master::range_cb(const std_msgs::UInt16MultiArray::ConstPtr& msg) {
        for (int i = 0; i < msg->data.size(); i++)
        {
        	std::cout << "Rng[" << i << "]: " << msg->data.at(i) << " | ";
        }
        std::cout << std::endl;
}

void was_master::lineTracking_cb(const std_msgs::UInt32::ConstPtr& msg) {
        ROS_INFO("Line Tracking: [%d]", msg->data);
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

void was_master::motor_left_cb(const std_msgs::String::ConstPtr& cmd) {
        std::cout << cmd->data << std::endl;

        std_msgs::String sending_cmd;
        sending_cmd.data = cmd->data;

#ifdef WAS_DEBUG
        std::cout << "Sending_cmd: " << sending_cmd.data << std::endl;
#endif

        motor_left_pub.publish(sending_cmd);
}

void was_master::motor_right_cb(const std_msgs::String::ConstPtr& cmd) {
        std::cout << cmd->data << std::endl;

        std_msgs::String sending_cmd;
        sending_cmd.data = cmd->data;

#ifdef WAS_DEBUG
        std::cout << "Sending_cmd: " << sending_cmd.data << std::endl;
#endif

        motor_right_pub.publish(sending_cmd);
}

void was_master::motor_stop_cb(const std_msgs::String::ConstPtr& cmd) {
        std::cout << cmd->data << std::endl;

        std_msgs::String sending_cmd;
        sending_cmd.data = cmd->data;

#ifdef WAS_DEBUG
        std::cout << "Sending_cmd: " << sending_cmd.data << std::endl;
#endif

        motor_stop_pub.publish(sending_cmd);
}

void was_master::motor_right_dir_cb(const std_msgs::String::ConstPtr& cmd) {
        std::cout << cmd->data << std::endl;

        std_msgs::String sending_cmd;
        sending_cmd.data = cmd->data;

#ifdef WAS_DEBUG
        std::cout << "Sending_cmd: " << sending_cmd.data << std::endl;
#endif

        motor_right_dir_pub.publish(sending_cmd);
}

       
void was_master::motor_left_dir_cb(const std_msgs::String::ConstPtr& cmd) {
        std::cout << cmd->data << std::endl;

        std_msgs::String sending_cmd;
        sending_cmd.data = cmd->data;

#ifdef WAS_DEBUG
        std::cout << "Sending_cmd: " << sending_cmd.data << std::endl;
#endif

        motor_left_dir_pub.publish(sending_cmd);
}
 
