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
BlackLib::BlackI2C  myI2C_0(BlackLib::I2C_0, 0x69);
BlackLib::BlackI2C  myI2C_2(BlackLib::I2C_2, 0x70);

void init_i2c()
{
    bool isOpened_I2C_0 = myI2C_0.open(BlackLib::ReadWrite | BlackLib::NonBlock);
    bool isOpened_I2C_2 = myI2C_2.open(BlackLib::ReadWrite | BlackLib::NonBlock);

   if(!isOpened_I2C_0)
    {
        std::cout << "I2C_0 failed" << std::endl;
	exit(1);
    }
    else std::cout << "I2C_0 initialized successfully!" << std::endl;

   if(!isOpened_I2C_2)
    {
        std::cout << "I2C_2 failed" << std::endl;
        exit(1);
    }
    else std::cout << "I2C_2 initialized successfully!" << std::endl;
}

void I2C_0_callback(const std_msgs::UInt8::ConstPtr& msg){
       //enable analog out 
    uint16_t data_en =((ctrl_byte_en << 8) | msg->data);
    bool resultOfWrite = myI2C_0.writeWord(0x0, data_en);
    if(resultOfWrite)
             ROS_INFO_STREAM("Write I2C_0 successfully: " << data_en);
    else     ROS_INFO_STREAM("Write I2C_0 failed: " << data_en);

}

void I2C_2_callback(const std_msgs::UInt8::ConstPtr& msg){
       //enable analog out 
    uint16_t data_en =((ctrl_byte_en << 8) | msg->data);
    bool resultOfWrite = myI2C_2.writeWord(0x0, data_en);
    if(resultOfWrite)
	     ROS_INFO_STREAM("Write I2C_2 successfully: " << data_en);
    else     ROS_INFO_STREAM("Write I2C_2 failed: " << data_en);
}

int main (int argc, char** argv){
    init_i2c();
    ros::init(argc, argv, "was_i2c_node");
    ros::NodeHandle nh;
    ros::Subscriber I2C_0_sub = nh.subscribe("LeftWheel_topic", 1000, I2C_0_callback);
    ros::Subscriber I2C_2_sub = nh.subscribe("RightWheel_topic",1000,I2C_2_callback);
    
    ros::Rate loop_rate(5);
    while(ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
    }
}
