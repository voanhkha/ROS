#include "ros/ros.h"
#include "std_msgs/String.h"


#define LEFT_DECREASE 1        
#define LEFT_INCREASE 2 

#define RIGHT_DECREASE 8	
#define RIGHT_INCREASE 9

#define LEFT_FORWARD 3        
#define LEFT_BACKWARD 4 

#define RIGHT_FORWARD 5        
#define RIGHT_BACKWARD 6 


class was_motor{
public:
        ros::NodeHandle nh;

        int digital_r;
        bool is_motor_right;
        
        ros::Subscriber motor_right_sub;
        void motor_right_cb(const std_msgs::String::ConstPtr& cmd);

        int digital_l;
        bool is_motor_left;

        ros::Subscriber motor_left_sub;
        void motor_left_cb(const std_msgs::String::ConstPtr& cmd);

        bool is_stop;
        ros::Subscriber motor_stop_sub;
        void motor_stop_cb(const std_msgs::String::ConstPtr& cmd);

        bool is_motor_left_dir;
        int left_dir;
	ros::Subscriber motor_left_dir_sub;
        void motor_left_dir_cb(const std_msgs::String::ConstPtr& cmd);

        bool is_motor_right_dir;
        int right_dir;
        ros::Subscriber motor_right_dir_sub;
        void motor_right_dir_cb(const std_msgs::String::ConstPtr& cmd);

        was_motor(void);
        void do_motor(void);
};
