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
uint8_t init_value = 0x00;
BlackLib::BlackI2C  myI2C2_0(BlackLib::I2C_2, 0x48);
BlackLib::BlackI2C  myI2C2_1(BlackLib::I2C_2, 0x49);
BlackLib::BlackI2C  myI2C2_2(BlackLib::I2C_2, 0x4a);
BlackLib::BlackI2C  myI2C2_3(BlackLib::I2C_2, 0x4b);

void init_i2c()
{
    bool isOpened_I2C2_0 = myI2C2_0.open(BlackLib::ReadWrite | BlackLib::NonBlock);
    bool isOpened_I2C2_1 = myI2C2_1.open(BlackLib::ReadWrite | BlackLib::NonBlock);
    bool isOpened_I2C2_2 = myI2C2_2.open(BlackLib::ReadWrite | BlackLib::NonBlock);
    bool isOpened_I2C2_3 = myI2C2_3.open(BlackLib::ReadWrite | BlackLib::NonBlock);
   if(!isOpened_I2C2_0)
    {
        std::cout << "I2C2_0 failed" << std::endl;
	exit(1);
    }
    else std::cout << "I2C2_0 initialized successfully!" << std::endl;

   if(!isOpened_I2C2_1)
    {
        std::cout << "I2C2_1 failed" << std::endl;
        exit(1);
    }
    else std::cout << "I2C2_1 initialized successfully!" << std::endl;

   if(!isOpened_I2C2_2)
    {
        std::cout << "I2C2_2 failed" << std::endl;
        exit(1);
    }
    else std::cout << "I2C2_2 initialized successfully!" << std::endl;

   if(!isOpened_I2C2_3)
    {
        std::cout << "I2C2_3 failed" << std::endl;
        exit(1);
    }
    else std::cout << "I2C2_3 initialized successfully!" << std::endl;

    bool initWrite_0 = myI2C2_0.writeByte(ctrl_byte_en, init_value);
    if(initWrite_0)
             ROS_INFO_STREAM("Write I2C2_0 init value successfully");
    else     ROS_INFO_STREAM("Write I2C2_0 init value failed");

    bool initWrite_1 = myI2C2_1.writeByte(ctrl_byte_en, init_value);
    if(initWrite_0)
             ROS_INFO_STREAM("Write I2C2_1 init value successfully");
    else     ROS_INFO_STREAM("Write I2C2_1 init value failed");

    bool initWrite_2 = myI2C2_2.writeByte(ctrl_byte_en, init_value);
    if(initWrite_2)
             ROS_INFO_STREAM("Write I2C2_2 init value successfully");
    else     ROS_INFO_STREAM("Write I2C2_2 init value failed");

    bool initWrite_3 = myI2C2_3.writeByte(ctrl_byte_en, init_value);
    if(initWrite_3)
             ROS_INFO_STREAM("Write I2C2_3 init value successfully");
    else     ROS_INFO_STREAM("Write I2C2_3 init value failed");

}

void I2C2_0_callback(const std_msgs::UInt8::ConstPtr& msg){
    bool resultOfWrite = myI2C2_0.writeByte(ctrl_byte_en, msg->data);
    if(resultOfWrite)
             ROS_INFO_STREAM("Write I2C2_0 successfully: " << msg->data);
    else     ROS_INFO_STREAM("Write I2C2_0 failed: " << msg->data);
}

void I2C2_1_callback(const std_msgs::UInt8::ConstPtr& msg){
    bool resultOfWrite = myI2C2_1.writeByte(ctrl_byte_en, msg->data);
    if(resultOfWrite)
             ROS_INFO_STREAM("Write I2C2_1 successfully: " << msg->data);
    else     ROS_INFO_STREAM("Write I2C2_1 failed: " << msg->data);

}

void I2C2_2_callback(const std_msgs::UInt8::ConstPtr& msg){
       //enable analog out 
    bool resultOfWrite = myI2C2_2.writeByte(ctrl_byte_en, msg->data);
    if(resultOfWrite)
	     ROS_INFO_STREAM("Write I2C2_2 successfully: " << msg->data);
    else     ROS_INFO_STREAM("Write I2C2_2 failed: " << msg->data);
}

void I2C2_3_callback(const std_msgs::UInt8::ConstPtr& msg){
    bool resultOfWrite = myI2C2_3.writeByte(ctrl_byte_en, msg->data);
    if(resultOfWrite)
             ROS_INFO_STREAM("Write I2C2_3 successfully: " << msg->data);
    else     ROS_INFO_STREAM("Write I2C2_3 failed: " << msg->data);

}

int main (int argc, char** argv){
    init_i2c();
    ros::init(argc, argv, "was_i2c_node");
    ros::NodeHandle nh;
    ros::Subscriber I2C2_0_sub = nh.subscribe("I2C2_0_topic", 1000, I2C2_0_callback);
    ros::Subscriber I2C2_1_sub = nh.subscribe("I2C2_1_topic", 1000, I2C2_1_callback);
    ros::Subscriber I2C2_2_sub = nh.subscribe("I2C2_2_topic",1000,I2C2_2_callback);
    ros::Subscriber I2C2_3_sub = nh.subscribe("I2C2_3_topic", 1000, I2C2_3_callback);

    ros::Rate loop_rate(5);
    while(ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
    }
}
