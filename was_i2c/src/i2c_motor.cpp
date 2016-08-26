#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt8.h"
#include "BlackLib/BlackLib.h"
#include <iostream>
#include <std_msgs/Empty.h>
//#include <sstream>
//#include <string>

#define LEFT_PIN_F  BlackLib::GPIO_22 //Beaglebone P8.19
#define LEFT_PIN_B  BlackLib::GPIO_23 //Beaglebone P8.13

#define RIGHT_PIN_F  BlackLib::GPIO_50 //Beaglebone P9.14
#define RIGHT_PIN_B  BlackLib::GPIO_51 //Beaglebone P9.16

uint8_t BaseLeftSpeed = 62;  // in dec: 53
uint8_t BaseRightSpeed = 67; //in dec: 55
uint8_t BrakeLeftValue = 0x40;
uint8_t BrakeRightValue = 0x40;
uint8_t ctrl_byte_dis = 0x00; //disable analog out
uint8_t ctrl_byte_en = 0x40; //enable analog out
uint8_t init_value = 0x00;
BlackLib::BlackI2C  myI2C2_0(BlackLib::I2C_2, 0x48);
BlackLib::BlackI2C  myI2C2_1(BlackLib::I2C_2, 0x49);
BlackLib::BlackI2C  myI2C2_2(BlackLib::I2C_2, 0x4a);
BlackLib::BlackI2C  myI2C2_3(BlackLib::I2C_2, 0x4b);
BlackLib::BlackGPIO left_FORWARD(BlackLib::BlackGPIO(LEFT_PIN_F, BlackLib::output));
BlackLib::BlackGPIO left_BACKWARD(BlackLib::BlackGPIO(LEFT_PIN_B, BlackLib::output));
BlackLib::BlackGPIO right_FORWARD(BlackLib::BlackGPIO(RIGHT_PIN_F, BlackLib::output));
BlackLib::BlackGPIO right_BACKWARD(BlackLib::BlackGPIO(RIGHT_PIN_B, BlackLib::output));

void init_i2c()
{
        left_FORWARD.setValue(BlackLib::high); // init mode: FORWARD 2 wheels
        left_BACKWARD.setValue(BlackLib::low);
        right_FORWARD.setValue(BlackLib::high);
        right_BACKWARD.setValue(BlackLib::low);

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
   // if(resultOfWrite)
   //          ROS_INFO_STREAM("Write I2C2_0 successfully: " << msg->data);
   // else     ROS_INFO_STREAM("Write I2C2_0 failed: " << msg->data);
}

void I2C2_1_callback(const std_msgs::UInt8::ConstPtr& msg){
    bool resultOfWrite = myI2C2_1.writeByte(ctrl_byte_en, msg->data);
   // if(resultOfWrite)
   //          ROS_INFO_STREAM("Write I2C2_1 successfully: " << msg->data);
   // else     ROS_INFO_STREAM("Write I2C2_1 failed: " << msg->data);

}

void I2C2_2_callback(const std_msgs::UInt8::ConstPtr& msg){
       //enable analog out 
    bool resultOfWrite = myI2C2_2.writeByte(ctrl_byte_en, msg->data);
  //  if(resultOfWrite)
//	     ROS_INFO_STREAM("Write I2C2_2 successfully: " << msg->data);
   // else     ROS_INFO_STREAM("Write I2C2_2 failed: " << msg->data);
}

void I2C2_3_callback(const std_msgs::UInt8::ConstPtr& msg){
    bool resultOfWrite = myI2C2_3.writeByte(ctrl_byte_en, msg->data);
   // if(resultOfWrite)
  //           ROS_INFO_STREAM("Write I2C2_3 successfully: " << msg->data);
//    else     ROS_INFO_STREAM("Write I2C2_3 failed: " << msg->data);

}

void Movement_callback(const std_msgs::String::ConstPtr& msg){
                          if(msg->data == "SpinLeft"){
                          left_FORWARD.setValue(BlackLib::high); // init mode: FORWARD 2 wheels
                          left_BACKWARD.setValue(BlackLib::low);
                          right_FORWARD.setValue(BlackLib::low);
                         right_BACKWARD.setValue(BlackLib::high);
		}

                             if(msg->data == "SpinRight"){
                            left_FORWARD.setValue(BlackLib::low); // init mode: FORWARD 2 wheels
                          left_BACKWARD.setValue(BlackLib::high);
                          right_FORWARD.setValue(BlackLib::high);
                         right_BACKWARD.setValue(BlackLib::low);
                }

		 if(msg->data == "Left"){
		      bool resultOfWrite = myI2C2_2.writeByte(ctrl_byte_en, BaseLeftSpeed);
		}
                if(msg->data == "Right"){
                         bool resultOfWrite = myI2C2_0.writeByte(ctrl_byte_en, BaseRightSpeed);
		}
                if(msg->data == "Forward"){
    			    left_FORWARD.setValue(BlackLib::high); // init mode: FORWARD 2 wheels
	    	          left_BACKWARD.setValue(BlackLib::low);
      			  right_FORWARD.setValue(BlackLib::high);
       			 right_BACKWARD.setValue(BlackLib::low);

			 bool resultOfWrite1 = myI2C2_2.writeByte(ctrl_byte_en, BaseLeftSpeed);
                         bool resultOfWrite2 = myI2C2_0.writeByte(ctrl_byte_en, BaseRightSpeed);
                }
                if(msg->data == "Backward"){
                         left_FORWARD.setValue(BlackLib::low); // init mode: FORWARD 2 wheels
                         left_BACKWARD.setValue(BlackLib::high);
                         right_FORWARD.setValue(BlackLib::low);
                         right_BACKWARD.setValue(BlackLib::high);

                         bool resultOfWrite1 = myI2C2_2.writeByte(ctrl_byte_en, BaseLeftSpeed);
                         bool resultOfWrite2 = myI2C2_0.writeByte(ctrl_byte_en, BaseRightSpeed);
                }
                if(msg->data == "Stop"){
                         bool resultOfWrite1 = myI2C2_2.writeByte(ctrl_byte_en, 0x00);
                         bool resultOfWrite2 = myI2C2_0.writeByte(ctrl_byte_en, 0x00);
                }
                if(msg->data == "BrL"){
                         bool resultOfWrite1 = myI2C2_3.writeByte(ctrl_byte_en, BrakeLeftValue);
		    if(resultOfWrite1)
             		ROS_INFO_STREAM("Write I2C2_3 successfully: " << msg->data);
   		     else     ROS_INFO_STREAM("Write I2C2_3 failed: " << msg->data);

                }
                if(msg->data == "BrR"){
                         bool resultOfWrite1 = myI2C2_1.writeByte(ctrl_byte_en, BrakeRightValue);
               
                }
                if(msg->data == "BrBoth"){
                         bool resultOfWrite1 = myI2C2_1.writeByte(ctrl_byte_en, BrakeRightValue);
			 bool resultOfWrite2 = myI2C2_3.writeByte(ctrl_byte_en, BrakeLeftValue);
                }

                if(msg->data == "Br2"){
                         bool resultOfWrite1 = myI2C2_3.writeByte(ctrl_byte_en, 0x00);
			 bool resultOfWrite2 = myI2C2_1.writeByte(ctrl_byte_en, 0x00);
                }
 }


int main (int argc, char** argv){
    init_i2c();
    ros::init(argc, argv, "was_i2c_node");
    ros::NodeHandle nh;
    ros::Subscriber I2C2_0_sub = nh.subscribe("RW", 1000, I2C2_0_callback); // right wheel
    ros::Subscriber I2C2_1_sub = nh.subscribe("RB", 1000, I2C2_1_callback); // right brake
    ros::Subscriber I2C2_2_sub = nh.subscribe("LW",1000,I2C2_2_callback);  // left wheel
    ros::Subscriber I2C2_3_sub = nh.subscribe("LB", 1000, I2C2_3_callback); // left brake
    ros::Subscriber Movement_sub = nh.subscribe("was_teleop/movement", 1000, Movement_callback); // Movement from keyboard

    ros::Rate loop_rate(5);
    while(ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
    }
}
