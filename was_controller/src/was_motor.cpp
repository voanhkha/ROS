#include "was_motor.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "BlackLib/BlackLib.h"
#include <iostream>

//#include <sstream>

//#include <string>

#define WAS_DEBUG
#define PUT_TO_MAIN


#ifndef PUT_TO_MAIN

#endif

was_motor::was_motor(void) {
        motor_right_sub = nh.subscribe("was_control/motor_right",
                1, &was_motor::motor_right_cb, this);
        is_motor_right = false;

        motor_left_sub = nh.subscribe("was_control/motor_left",
                1, &was_motor::motor_left_cb, this);
        is_motor_left = false;

        motor_stop_sub = nh.subscribe("was_control/motor_stop",
                1, &was_motor::motor_stop_cb, this);
	is_stop = false;

        motor_right_dir_sub = nh.subscribe("was_control/motor_right_dir",
                1, &was_motor::motor_right_dir_cb, this);
        is_motor_right_dir = false;

        motor_left_dir_sub = nh.subscribe("was_control/motor_left_dir",
                1, &was_motor::motor_left_dir_cb, this);
        is_motor_left_dir = false;

#ifdef WAS_DEBUG
        std::cout << "Initialized was_motor" << std::endl;
#endif
}


void was_motor::motor_right_dir_cb(const std_msgs::String::ConstPtr& cmd) {
#ifdef WAS_DEBUG
                std::cout << "motor_right_dir_cb: ";
#endif
        if (cmd->data == "RIGHT_FORWARD") {
                right_dir = RIGHT_FORWARD;
                is_motor_right_dir = true;
#ifdef WAS_DEBUG
                std::cout << "Right Forward" << std::endl;
#endif
        }
        else if (cmd->data == "RIGHT_BACKWARD") {
                right_dir = RIGHT_BACKWARD;
                is_motor_right_dir = true;
#ifdef WAS_DEBUG
                std::cout << "Right Backward" << std::endl;
#endif
        }
}

void was_motor::motor_left_dir_cb(const std_msgs::String::ConstPtr& cmd) {
#ifdef WAS_DEBUG
                std::cout << "motor_left_dir_cb: ";
#endif
        if (cmd->data == "LEFT_FORWARD") {
                left_dir = LEFT_FORWARD;
                is_motor_left_dir = true;
#ifdef WAS_DEBUG
                std::cout << "Left Forward" << std::endl;
#endif
        }
        else if (cmd->data == "LEFT_BACKWARD") {
                left_dir = LEFT_BACKWARD;
                is_motor_left_dir = true;
#ifdef WAS_DEBUG
                std::cout << "Left Backward" << std::endl;
#endif
        }
}



void was_motor::motor_right_cb(const std_msgs::String::ConstPtr& cmd) {
#ifdef WAS_DEBUG
                std::cout << "motor_right_cb: ";
#endif
        if (cmd->data == "RIGHT_DECREASE") {
                digital_r = RIGHT_DECREASE;
                is_motor_right = true;
#ifdef WAS_DEBUG
                std::cout << "Decrease" << std::endl;
#endif
        }
        else if (cmd->data == "RIGHT_INCREASE") {
                digital_r = RIGHT_INCREASE;
                is_motor_right = true;
#ifdef WAS_DEBUG
                std::cout << "Increase" << std::endl;
#endif
        }
}

void was_motor::motor_left_cb(const std_msgs::String::ConstPtr& cmd) {
#ifdef WAS_DEBUG
                std::cout << "motor_left_cb: ";
#endif
        if (cmd->data == "LEFT_DECREASE") {
                digital_l = LEFT_DECREASE;
                is_motor_left = true;
#ifdef WAS_DEBUG
                std::cout << "Decrease" << std::endl;
#endif
        }
        else if (cmd->data == "LEFT_INCREASE") {
                digital_l = LEFT_INCREASE;
                is_motor_left = true;
#ifdef WAS_DEBUG
                std::cout << "Increase" << std::endl;
#endif
        }
}

void was_motor::motor_stop_cb(const std_msgs::String::ConstPtr& cmd) {
#ifdef WAS_DEBUG
                std::cout << "motor_stop_cb: ";
#endif
                is_stop = true;
}

void was_motor::do_motor(void) {
        while (ros::ok()) {
		if (is_motor_right) {
			switch(digital_r) {
				case RIGHT_DECREASE:
					is_motor_right = true;
					#ifndef PUT_TO_MAIN
					data--;
					pcf8591_i2c2();							
					#endif
					break;
				case RIGHT_INCREASE:
					is_motor_right = true;
					#ifndef PUT_TO_MAIN
					data++;	
					pcf8591_i2c2();
					#endif	
					break;
			}
		}
	
               if (is_motor_left) {
                        switch(digital_l) {
                                case LEFT_DECREASE:
                                        is_motor_left = true;
                                        #ifndef PUT_TO_MAIN
                                        data--;
                                        pcf8591_i2c2();
                                        #endif
                                        break;
                                case LEFT_INCREASE:
                                        is_motor_left = true;
                                        #ifndef PUT_TO_MAIN
                                        data++;
                                        pcf8591_i2c2();
                                        #endif
                                        break;
                        }
                }
	}
}

