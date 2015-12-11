#include <ros/ros.h>
#include <serial/serial.h>
#include <std_msgs/String.h>
#include <std_msgs/Empty.h>

serial::Serial UART1;
serial::Serial UART2;

void UART1_callback(const std_msgs::String::ConstPtr& msg){
    ROS_INFO_STREAM("Writing to UART1: " << msg->data);
    UART1.write(msg->data);
}

void UART2_callback(const std_msgs::String::ConstPtr& msg){
	ROS_INFO_STREAM("Writing to UART2: " << msg->data);
	UART2.write(msg->data);
}

int main (int argc, char** argv){
    ros::init(argc, argv, "serial_node");
    ros::NodeHandle nh;
    ros::Subscriber UART1_sub = nh.subscribe("UART1_topic", 1000, UART1_callback);
    ros::Subscriber UART2_sub = nh.subscribe("UART2_topic",1000,UART2_callback);
    try
    {
        UART1.setPort("/dev/ttyS1");
        UART1.setBaudrate(9600);
        serial::Timeout to = serial::Timeout::simpleTimeout(1000);
        UART1.setTimeout(to);
        UART1.open();
    }

    catch (serial::IOException& e)
    {
    	ROS_ERROR_STREAM("Unable to open UART1");
	return -1;
    }

    try
    {
	UART2.setPort("/dev/ttyS4");
	UART2.setBaudrate(9600);
	serial::Timeout to = serial::Timeout::simpleTimeout(1000);
	UART2.setTimeout(to);
	UART2.open();
    }

    catch (serial::IOException& e)
    {
        ROS_ERROR_STREAM("Unable to open UART2");
        return -1;
    }

    if(UART1.isOpen()){
        ROS_INFO_STREAM("UART1 initialized");
    }else{
        return -1;
    }

    if(UART2.isOpen()){
	ROS_INFO_STREAM("UART2 initialized");
    }else{
	return -1;
    }

    ros::Rate loop_rate(10);
    while(ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
    }
}

