#include "ros/ros.h"
#include "std_msgs/UInt16MultiArray.h"
#include "BlackLib/BlackLib.h"
#include <sstream>

// Approximately calculated gradient of "range = f(voltage)"
// Because "f(voltage)" is approximately linear, the equation should be:
// range = SLOPE x Voltage + INTERCEPTER, where:
#define SLOPE        1
#define INTERCEPTER  0

#define NUM_RANGE_SENSOR        5

#define WAS_DEBUG

int main(int argc, char **argv)
{
        ros::init(argc, argv, "was_range_sensor");
        ros::NodeHandle nh;
        ros::Publisher range_pub = nh.advertise<std_msgs::UInt16MultiArray>("was_sensor/ultrasound", 1000);
        ros::Rate loop_rate(10);

        std_msgs::UInt16MultiArray was_range_msg;

        BlackLib::BlackADC *readADCBuff[NUM_RANGE_SENSOR];
	readADCBuff[0]= new BlackLib::BlackADC(BlackLib::AIN0);
	readADCBuff[1]= new BlackLib::BlackADC(BlackLib::AIN1);
	readADCBuff[2]= new BlackLib::BlackADC(BlackLib::AIN2);
	readADCBuff[3]= new BlackLib::BlackADC(BlackLib::AIN3);
	readADCBuff[4]= new BlackLib::BlackADC(BlackLib::AIN4);

        while (ros::ok()) {
        	was_range_msg.data.clear();
        	for (int i = 0; i < NUM_RANGE_SENSOR; ++i)
        	{
        		// was_range_msg.data[i] =  ( ( readADCBuff[i]->getNumericValue() * SLOPE ) + INTERCEPTER );
                        was_range_msg.data.push_back( ( readADCBuff[i]->getNumericValue() * SLOPE ) + INTERCEPTER );

#ifdef  WAS_DEBUG
                        std::cout << "ADC[" << i << "]: " << readADCBuff[i]->getNumericValue() << " | ";
#endif
        	}
                std::cout << std::endl;

                range_pub.publish(was_range_msg);

                ros::spinOnce();

                loop_rate.sleep();
        }
        return 0;
}
