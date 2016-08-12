#include "was_motor.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "BlackLib/BlackLib.h"
#include <iostream>

//#include <sstream>

//#include <string>

#define WAS_DEBUG
#define PUT_TO_MAIN

//I2C2 accessing to PCF8591

#ifndef PUT_TO_MAIN
uint8_t data = 0;

bool pcf8591_i2c2(void)
{

    BlackLib::BlackI2C  myI2c(BlackLib::I2C_2, 0x48);

    bool isOpened = myI2c.open( BlackLib::ReadWrite | BlackLib::NonBlock );

    if( !isOpened )
    {
        std::cout << "I2C2 DEVICE CAN\'T OPEN.;" << std::endl;
        exit(1);
    }


    //control byte: 
    uint8_t ctrl_byte_dis = 0x00; //disable analog out
    uint8_t ctrl_byte_en = 0x40; //enable analog out


    //disable analog out
    //uint16_t data_dis =((ctrl_byte_dis << 8) | data);

    //enable analog out 
    uint16_t data_en =((ctrl_byte_en << 8) | data);

    //write to pcf
    bool resultOfWrite           = myI2c.writeWord(0x0, data_en);
    return resultOfWrite;
}
#endif

was_motor::was_motor(void) {
        motor_left_sub = nh.subscribe("was_control/motor_left",
                1, &was_motor::motor_left_cb, this);
        is_motor_left = false;

#ifdef WAS_DEBUG
        std::cout << "Initialized was_motor" << std::endl;
#endif
}


void was_motor::motor_left_cb(const std_msgs::String::ConstPtr& cmd) {
#ifdef WAS_DEBUG
                std::cout << "motor_left_cb: ";
#endif
        if (cmd->data == "LEFT_DECREASE") {
                digital = LEFT_DECREASE;
                is_motor_left = true;
#ifdef WAS_DEBUG
                std::cout << "Decrease" << std::endl;
#endif
        }
        else if (cmd->data == "LEFT_INCREASE") {
                digital = LEFT_INCREASE;
                is_motor_left = true;
#ifdef WAS_DEBUG
                std::cout << "Increase" << std::endl;
#endif
        }
}

void was_motor::do_motor(void) {
        while (ros::ok()) {
		if (is_motor_left) {
			switch(digital) {
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

