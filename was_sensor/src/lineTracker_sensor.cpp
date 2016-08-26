#include "ros/ros.h"
#include <std_msgs/UInt8.h>
#include "BlackLib/BlackLib.h"
#include <sstream>
#include <bitset>

//Define bits for LED on IR sensor
#define T1      0
#define T2      1
#define T3      2
#define T4      3
#define T5      4

int main(int argc, char **argv)
{
        ros::init(argc, argv, "was_lineTracker_sensor");
        ros::NodeHandle n;
        ros::Publisher lineTracker_pub = n.advertise<std_msgs::UInt8>("was_sensor/lineTracking", 1000);
	ros::Rate loop_rate(20);

        std::bitset<5> trackingValue_bitset;
        BlackLib::BlackGPIO t1Value(BlackLib::GPIO_45, BlackLib::input);
        BlackLib::BlackGPIO t2Value(BlackLib::GPIO_47, BlackLib::input);
        BlackLib::BlackGPIO t3Value(BlackLib::GPIO_27, BlackLib::input);
        BlackLib::BlackGPIO t4Value(BlackLib::GPIO_44, BlackLib::input);
        BlackLib::BlackGPIO t5Value(BlackLib::GPIO_26, BlackLib::input);

        std_msgs::UInt8 trackingValue;

        while (ros::ok()) {
                trackingValue_bitset[T1] = t1Value.getNumericValue();
                trackingValue_bitset[T2] = t2Value.getNumericValue();
                trackingValue_bitset[T3] = t3Value.getNumericValue();
                trackingValue_bitset[T4] = t4Value.getNumericValue();
                trackingValue_bitset[T5] = t5Value.getNumericValue();
                trackingValue.data = (int)trackingValue_bitset.to_ulong();
                lineTracker_pub.publish(trackingValue);
                ros::spinOnce();
                loop_rate.sleep();
        }
        return 0;
}
