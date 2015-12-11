#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt8.h"
#include "BlackLib/BlackLib.h"
#include <iostream>
#include <std_msgs/Empty.h>

//#include <sstream>

//#include <string>

uint8_t ctrl_byte_dis = 0x00; //disable analog out
uint8_t ctrl_byte_en = 0x40; //enable analog out
BlackLib::BlackI2C  myI2C2_0(BlackLib::I2C_2, 0x48);
BlackLib::BlackI2C  myI2C2_2(BlackLib::I2C_2, 0x50);

void init_i2c()
{
    bool isOpened_I2C2_0 = myI2C2_0.open(BlackLib::ReadWrite | BlackLib::NonBlock);
    bool isOpened_I2C2_2 = myI2C2_2.open(BlackLib::ReadWrite | BlackLib::NonBlock);

   if(!isOpened_I2C2_0)
    {
        std::cout << "I2C2_0 failed" << std::endl;
	exit(1);
    }
    else std::cout << "I2C2_0 initialized successfully!" << std::endl;

   if(!isOpened_I2C2_2)
    {
        std::cout << "I2C2_2 failed" << std::endl;
        exit(1);
    }
    else std::cout << "I2C2_2 initialized successfully!" << std::endl;
}

void I2C2_0_callback(const std_msgs::UInt8::ConstPtr& msg){
    bool resultOfWrite = myI2C2_0.writeByte(ctrl_byte_en, msg->data);
    if(resultOfWrite)
             ROS_INFO_STREAM("Write I2C2_0 successfully: " << msg->data);
    else     ROS_INFO_STREAM("Write I2C2_0 failed: " << msg->data);

}

void I2C2_2_callback(const std_msgs::UInt8::ConstPtr& msg){
       //enable analog out 
    bool resultOfWrite = myI2C2_2.writeByte(ctrl_byte_en, msg->data);
    if(resultOfWrite)
	     ROS_INFO_STREAM("Write I2C2_2 successfully: " << msg->data);
    else     ROS_INFO_STREAM("Write I2C2_2 failed: " << msg->data);
}

int main (int argc, char** argv){
    init_i2c();
    ros::init(argc, argv, "was_i2c_node");
    ros::NodeHandle nh;
    ros::Subscriber I2C2_0_sub = nh.subscribe("LeftWheel_topic", 1000, I2C2_0_callback);
    ros::Subscriber I2C2_2_sub = nh.subscribe("RightWheel_topic",1000,I2C2_2_callback);
    
    ros::Rate loop_rate(5);
    while(ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
    }
}
