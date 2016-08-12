#include "ros/ros.h"
#include "std_msgs/String.h"

#define LEFT_DECREASE	1
#define LEFT_INCREASE	2


class was_motor{
public:
        ros::NodeHandle nh;

        int digital;
        bool is_motor_left;
        
        ros::Subscriber motor_left_sub;
        void motor_left_cb(const std_msgs::String::ConstPtr& cmd);

        was_motor(void);
        void do_motor(void);
};
