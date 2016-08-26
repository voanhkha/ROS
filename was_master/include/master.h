#include "ros/ros.h"
#include "std_msgs/UInt32.h"
#include "std_msgs/UInt16MultiArray.h"
#include "std_msgs/String.h"

class was_master{
private:
        ros::NodeHandle nh;

        ros::Subscriber range_sub;
        void range_cb(const std_msgs::UInt16MultiArray::ConstPtr& msg);

        ros::Subscriber lineTracking_sub;
        void lineTracking_cb(const std_msgs::UInt32::ConstPtr& msg);

        ros::Subscriber movement_sub;
        void movement_cb(const std_msgs::String::ConstPtr& cmd);
        
        ros::Subscriber lift_sub;
        void lift_cb(const std_msgs::String::ConstPtr& cmd);

        ros::Publisher lift_pub;

        ros::Subscriber motor_left_sub;
        void motor_left_cb(const std_msgs::String::ConstPtr& cmd);
	ros::Publisher motor_left_pub;

        ros::Subscriber motor_right_sub;
        void motor_right_cb(const std_msgs::String::ConstPtr& cmd);
        ros::Publisher motor_right_pub;

        ros::Subscriber motor_stop_sub;
        void motor_stop_cb(const std_msgs::String::ConstPtr& cmd);
        ros::Publisher motor_stop_pub;

        ros::Subscriber motor_right_dir_sub;
        void motor_right_dir_cb(const std_msgs::String::ConstPtr& cmd);
        ros::Publisher motor_right_dir_pub;

        ros::Subscriber motor_left_dir_sub;
        void motor_left_dir_cb(const std_msgs::String::ConstPtr& cmd);
        ros::Publisher motor_left_dir_pub;

public:
        was_master(void);
};
