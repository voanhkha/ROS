#include "ros/ros.h"
#include <std_msgs/UInt32.h>
#include <std_msgs/UInt8.h>
#include <std_msgs/Int16.h>
#include <std_msgs/String.h>
#include "BlackLib/BlackLib.h"
#include <sstream>
#include <bitset>
#include <iostream>
#include <string>
#include <iomanip>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <ctime>
#include <chrono>

//Define registers for gyro
#define L3G4200D_WHO_AM_I       0x0F
#define L3G4200D_CTRL_REG1      0x20
#define L3G4200D_CTRL_REG2      0x21
#define L3G4200D_CTRL_REG3      0x22
#define L3G4200D_CTRL_REG4      0x23
#define L3G4200D_CTRL_REG5      0x24
#define L3G4200D_REFERENCE      0x25
#define L3G4200D_OUT_TEMP       0x26
#define L3G4200D_STATUS_REG     0x27

#define L3G4200D_OUT_X_L        0x28
#define L3G4200D_OUT_X_H        0x29
#define L3G4200D_OUT_Y_L        0x2A
#define L3G4200D_OUT_Y_H        0x2B
#define L3G4200D_OUT_Z_L        0x2C
#define L3G4200D_OUT_Z_H        0x2D

#define L3G4200D_FIFO_CTRL_REG  0x2E
#define L3G4200D_FIFO_SRC_REG   0x2F

#define L3G4200D_INT1_CFG       0x30
#define L3G4200D_INT1_SRC       0x31
#define L3G4200D_INT1_THS_XH    0x32
#define L3G4200D_INT1_THS_XL    0x33
#define L3G4200D_INT1_THS_YH    0x34
#define L3G4200D_INT1_THS_YL    0x35
#define L3G4200D_INT1_THS_ZH    0x36
#define L3G4200D_INT1_THS_ZL    0x37
#define L3G4200D_INT1_DURATION 0x38

static BlackLib::BlackI2C myI2C(BlackLib::I2C_2, 0x69);
static float z_global;
static float z_offset;
static int8_t angle, spin_en;

 //       ros::NodeHandle nh;
   //     ros::Publisher leftmotor_pub, rightmotor_pub;

//	leftmotor_pub = nh.advertise<std_msgs::UInt8>("LW", 1000);
  //      rightmotor_pub = nh.advertise<std_msgs::UInt8>("RW", 1000);
	 // ros::init(argc, argv, "was_gyro_sensor");
//        ros::Rate loop_rate(10);
  //      ros::NodeHandle nh;
       // ros::Publisher leftmotor_pub;// = nh.advertise<std_msgs::UInt8>("LW", 1000);
       // ros::Publisher rightmotor_pub;// = nh.advertise<std_msgs::UInt8>("RW", 1000);
       // ros::Publisher gyro_pub;// = nh.advertise<std_msgs::UInt32>("was_gyro", 1000);
       // ros::Rate loop_rate1(0.2);



void init_gyro()
{
    bool isOpened = myI2C.open(BlackLib::ReadWrite | BlackLib::NonBlock);
    if(!isOpened)
    {
        std::cout << "I2C_2 failed" << std::endl;
	exit(1);
    }
    else std::cout << "I2C_2 initialized successfully!" << std::endl;
    // Turns on the L3G4200D's gyro and places it in normal mode.
    // Normal power mode, all axes enabled (for detailed info see datasheet)
    myI2C.writeByte(L3G4200D_CTRL_REG2, 0x00);   // highpass filter disabled
    myI2C.writeByte(L3G4200D_CTRL_REG3, 0x00);
    myI2C.writeByte(L3G4200D_CTRL_REG4, 0x00);   // sets acuracy to 250 dps (degree per second)
    myI2C.writeByte(L3G4200D_CTRL_REG5, 0x00);  // deactivates the filters (only use one of these options)
    myI2C.writeByte(L3G4200D_CTRL_REG1, 0x0F);  // starts gyro measurement
    
}

float get_gyro_value()
{
		uint8_t values[2];
                values[0] = myI2C.readByte(L3G4200D_OUT_Z_L);
                values[1] = myI2C.readByte(L3G4200D_OUT_Z_H);
                int Z = static_cast<int16_t>(static_cast<uint16_t>(values[0]) | (static_cast<uint16_t>(values[1]) << 8));
                float Z_float = Z*0.00875;
		return Z_float;
}

void calib_gyro()
{
        float z_total;
        for (int i = 0; i < 200; i++)
        {
                float z = get_gyro_value();
                z_total = z + z_total;
                usleep(10000);
        }
        z_offset = z_total/200;
}


void Spin_callback(const std_msgs::Int16::ConstPtr& msg){
       	spin_en = 1;
	angle = msg->data;
}

int main(int argc, char **argv)
{
        ros::init(argc, argv, "was_gyro_sensor");
        ros::NodeHandle nh;
	ros::Publisher leftmotor_pub = nh.advertise<std_msgs::UInt8>("LW", 1000);
        ros::Publisher rightmotor_pub = nh.advertise<std_msgs::UInt8>("RW", 1000);
	ros::Publisher movement_pub = nh.advertise<std_msgs::String>("was_teleop/movement", 1000);
	ros::Subscriber spin_sub = nh.subscribe("SpinAngle", 1000, Spin_callback); 
	ros::Rate loop_rate(10);
        ros::Rate loop_rate1(0.2);
	std_msgs::UInt8 SpinSpeed, SpinZero;
	std_msgs::String topicdata;
	SpinSpeed.data = 45;
	SpinZero.data = 0;
	init_gyro();
        while (ros::ok()) {
	   if (spin_en == 1){
		calib_gyro();
        	float total_angle = 0;
        	std::chrono::time_point<std::chrono::system_clock> start_time, end_time;
        	start_time = std::chrono::system_clock::now();
        	if(angle>0){
			topicdata.data = "SpinRight";
                	movement_pub.publish(topicdata);
			rightmotor_pub.publish(SpinSpeed);
			leftmotor_pub.publish(SpinSpeed);
        	}
        	else{
			topicdata.data = "SpinLeft";
			movement_pub.publish(topicdata);
              	  	leftmotor_pub.publish(SpinSpeed);
			rightmotor_pub.publish(SpinSpeed);
        	}

        	while (abs(total_angle)<abs(angle)) {
                	z_global = get_gyro_value() - z_offset;
                	end_time = std::chrono::system_clock::now();
                	std::chrono::duration<double> elapsed_seconds = end_time - start_time;
                	start_time = std::chrono::system_clock::now();
                	total_angle = total_angle + z_global*elapsed_seconds.count();
                	ros::spinOnce();
                	loop_rate.sleep();
       	 	}
		spin_en = 0;
		topicdata.data = "BrBoth"; // break 2 wheels
		movement_pub.publish(topicdata); 
                rightmotor_pub.publish(SpinZero); // send speed 0 to right wheel
                leftmotor_pub.publish(SpinZero); // send speed 0 to left wheel
		loop_rate.sleep(); // wait a moment before cancel the breaks
		topicdata.data = "Br2"; //cancel both breaks
		movement_pub.publish(topicdata);
            }		
	        ros::spinOnce();
                loop_rate1.sleep();
        }
        return 0;
}
