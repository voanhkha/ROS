#include "ros/ros.h"
#include "std_msgs/String.h"

#define LIFT_STOP	0
#define LIFT_UP		1
#define LIFT_DOWN	2


class was_lift{
public:
        ros::NodeHandle nh;

        int lifting;
        bool is_lifting;
        
        ros::Subscriber lift_sub;
        void lift_cb(const std_msgs::String::ConstPtr& cmd);

        was_lift(void);
        void do_lift(void);
};