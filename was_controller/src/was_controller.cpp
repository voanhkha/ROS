#include "ros/ros.h"
#include "was_lift.h"
#include "was_motor.h"
#include "BlackLib/BlackLib.h"

#define CYCLINDER_PIN_0  BlackLib::GPIO_60
#define CYCLINDER_PIN_1  BlackLib::GPIO_115

#define LEFT_PIN_F  BlackLib::GPIO_22 //Beaglebone P8.19
#define LEFT_PIN_B  BlackLib::GPIO_23 //Beaglebone P8.13

#define RIGHT_PIN_F  BlackLib::GPIO_50 //Beaglebone P9.14
#define RIGHT_PIN_B  BlackLib::GPIO_51 //Beaglebone P9.16

#define PUT_TO_MAIN

#ifdef PUT_TO_MAIN

uint8_t ctrl_byte_dis = 0x00; //disable analog out
uint8_t ctrl_byte_en = 0x40; //enable analog out
BlackLib::BlackI2C  myI2C2_0(BlackLib::I2C_2, 0x48); //Right wheel
BlackLib::BlackI2C  myI2C2_1(BlackLib::I2C_2, 0x49); //Right brake
BlackLib::BlackI2C  myI2C2_2(BlackLib::I2C_2, 0x4a); //Left wheel
BlackLib::BlackI2C  myI2C2_3(BlackLib::I2C_2, 0x4b); //Left brake 

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
    else std::cout << "I2C2_0 (Right wheel) initialized successfully!" << std::endl;

   if(!isOpened_I2C2_1)
    {
        std::cout << "I2C2_1 failed" << std::endl;
        exit(1);
    }
    else std::cout << "I2C2_1 (Right brake) initialized successfully!" << std::endl;

   if(!isOpened_I2C2_2)
    {
        std::cout << "I2C2_2 failed" << std::endl;
        exit(1);
    }
    else std::cout << "I2C2_2 (Left wheel) initialized successfully!" << std::endl;

   if(!isOpened_I2C2_3)
    {
        std::cout << "I2C2_3 failed" << std::endl;
        exit(1);
    }
    else std::cout << "I2C2_3 (Left brake) initialized successfully!" << std::endl;

    bool resultOfWrite0 = myI2C2_0.writeByte(ctrl_byte_en, 0);
    bool resultOfWrite1 = myI2C2_1.writeByte(ctrl_byte_en, 0);
    bool resultOfWrite2 = myI2C2_2.writeByte(ctrl_byte_en, 0);
    bool resultOfWrite3 = myI2C2_3.writeByte(ctrl_byte_en, 0);
    if(resultOfWrite0)
             ROS_INFO_STREAM("Write I2C2_0 successfully: ");
    else     ROS_INFO_STREAM("Write I2C2_0 failed: ");
    if(resultOfWrite1)
             ROS_INFO_STREAM("Write I2C2_1 successfully: ");
    else     ROS_INFO_STREAM("Write I2C2_1 failed: ");
    if(resultOfWrite2)
             ROS_INFO_STREAM("Write I2C2_2 successfully: ");
    else     ROS_INFO_STREAM("Write I2C2_2 failed: ");
    if(resultOfWrite3)
             ROS_INFO_STREAM("Write I2C2_3 successfully: ");
    else     ROS_INFO_STREAM("Write I2C2_3 failed: ");

}


uint8_t data0 = 0;
uint8_t data1 = 0;
uint8_t data2 = 0;
uint8_t data3 = 0;

void mdelay (int ms)
{
	int i;
	for (i=0; i < ms; i++);
}

bool pcf8591_i2c2_0(uint8_t dta)
{

    bool resultOfWrite = myI2C2_0.writeByte(ctrl_byte_en, dta);
    //mdelay(1000);
 
    return resultOfWrite;
}

bool pcf8591_i2c2_1(uint8_t dta)
{

    bool resultOfWrite = myI2C2_1.writeByte(ctrl_byte_en, dta);
    //mdelay(1000);

    return resultOfWrite;
}

bool pcf8591_i2c2_2(uint8_t dta)
{

    bool resultOfWrite = myI2C2_2.writeByte(ctrl_byte_en, dta);
    //mdelay(1000);

    return resultOfWrite;
}

bool pcf8591_i2c2_3(uint8_t dta)
{

    bool resultOfWrite = myI2C2_3.writeByte(ctrl_byte_en, dta);
    //mdelay(1000);

    return resultOfWrite;
}

bool motor_stop(void)
{
    bool resultOfWrite = myI2C2_0.writeByte(ctrl_byte_en, 0);
    resultOfWrite = myI2C2_2.writeByte(ctrl_byte_en, 0);  
    return resultOfWrite;
}

#endif

int main(int argc, char **argv) {
      	init_i2c();
 
	ros::init(argc, argv, "was_control");

        was_lift *lift_controller = new was_lift;
        was_motor *motor_controller = new was_motor;
        //lift_controller->do_lift();
        BlackLib::BlackGPIO lift_GPIO_UP(BlackLib::BlackGPIO(CYCLINDER_PIN_0, BlackLib::output));
        BlackLib::BlackGPIO lift_GPIO_DOWN(BlackLib::BlackGPIO(CYCLINDER_PIN_1, BlackLib::output));

        BlackLib::BlackGPIO left_FORWARD(BlackLib::BlackGPIO(LEFT_PIN_F, BlackLib::output));
        BlackLib::BlackGPIO left_BACKWARD(BlackLib::BlackGPIO(LEFT_PIN_B,
BlackLib::output));

        BlackLib::BlackGPIO right_FORWARD(BlackLib::BlackGPIO(RIGHT_PIN_F, BlackLib::output));
        BlackLib::BlackGPIO right_BACKWARD(BlackLib::BlackGPIO(RIGHT_PIN_B, BlackLib::output));

        left_FORWARD.setValue(BlackLib::low);
        left_BACKWARD.setValue(BlackLib::low);

        right_FORWARD.setValue(BlackLib::low);
        right_BACKWARD.setValue(BlackLib::low);

       while (ros::ok()) {
	         if (lift_controller->is_lifting) {
                        switch (lift_controller->lifting) {
                                case LIFT_UP:
                                        lift_GPIO_UP.setValue(BlackLib::high);
                                        lift_GPIO_DOWN.setValue(BlackLib::low);
                                        lift_controller->is_lifting = false;
                                        std::cout << "Lifting UP" << std::endl;
                                        break;
                                case LIFT_DOWN:
                                        lift_GPIO_UP.setValue(BlackLib::low);
                                        lift_GPIO_DOWN.setValue(BlackLib::high);
                                        lift_controller->is_lifting = false;
                                        std::cout << "Lifting DOWN" << std::endl;
                                        break;
                                case LIFT_STOP:
                                        lift_GPIO_UP.setValue(BlackLib::low);
                                        lift_GPIO_DOWN.setValue(BlackLib::low);
                                        lift_controller->is_lifting = false;
                                        std::cout << "STOP" << std::endl;
                                        break;
                        }
                }
//		motor_controller->do_motor();
#ifdef PUT_TO_MAIN

		if(motor_controller->is_motor_left_dir) {
			switch(motor_controller->left_dir) {
				case LEFT_FORWARD:
					motor_controller->is_motor_left_dir = false;
					left_FORWARD.setValue(BlackLib::high);
					left_BACKWARD.setValue(BlackLib::low);
					printf("LEFT_FORWARD...\n");
					break;
				case LEFT_BACKWARD:
                                        motor_controller->is_motor_left_dir = false;
					left_FORWARD.setValue(BlackLib::low);
					left_BACKWARD.setValue(BlackLib::high);
                                        printf("LEFT_BACKWARD...\n");
                                        break;
			}
		}

                if(motor_controller->is_motor_right_dir) {
                        switch(motor_controller->right_dir) {
                                case RIGHT_FORWARD:
                                        motor_controller->is_motor_right_dir = false;
                                        right_FORWARD.setValue(BlackLib::high);
                                        right_BACKWARD.setValue(BlackLib::low);
                                        printf("RIGHT_FORWARD...\n");
                                        break;
                                case RIGHT_BACKWARD:
                                        motor_controller->is_motor_right_dir = false;
                                        right_FORWARD.setValue(BlackLib::low);
                                        right_BACKWARD.setValue(BlackLib::high);
                                        printf("RIGHT_BACKWARD...\n");
                                        break;
			}
                } 

   
                if (motor_controller->is_motor_right) {
                        switch (motor_controller->digital_r) {
                                case RIGHT_DECREASE:
					motor_controller->is_motor_right = true;
					data0--;
                                       	printf("Right wheel STOP\n"); 
					pcf8591_i2c2_0(0x00);
                                     	motor_controller->is_motor_right = 0; 
					break;
                                case RIGHT_INCREASE:
                                        motor_controller->is_motor_right = true;
                                        data0++;
                                       	printf("Right wheel SPIN\n"); 
                                        pcf8591_i2c2_0(0x2F);
                                     	motor_controller->is_motor_right = 0; 
                                        break;
                        }
                }

                if (motor_controller->is_motor_left) {
                        switch (motor_controller->digital_l) {
                                case LEFT_DECREASE:
                                        motor_controller->is_motor_left = true;
                                        printf("Left wheel STOP\n");
                                        pcf8591_i2c2_2(0x00);
                                        motor_controller->is_motor_left = 0;
                                        break;
                                case LEFT_INCREASE:
                                        motor_controller->is_motor_left = true;
                                        printf("Left wheel SPIN\n");
                                        pcf8591_i2c2_2(0x2D);
                                        motor_controller->is_motor_left = 0;
                                        break;
                        }
                }

		if (motor_controller->is_stop) {
			printf("Motor stop\n");
			motor_stop();
		}

#endif                
		ros::spinOnce();
        }
        // ros::spin();
        return 0;
}
