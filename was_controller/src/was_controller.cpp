#include "ros/ros.h"
#include "was_lift.h"
#include "was_motor.h"
#include "BlackLib/BlackLib.h"

#define CYCLINDER_PIN_0  BlackLib::GPIO_60
#define CYCLINDER_PIN_1  BlackLib::GPIO_115

#define PUT_TO_MAIN

#ifdef PUT_TO_MAIN
uint8_t data = 0;

void mdelay (int ms)
{
	int i;
	for (i=0; i < ms; i++);
}

bool pcf8591_i2c2(void)
{

    BlackLib::BlackI2C  myI2c(BlackLib::I2C_2, 0x48);

    bool isOpened = myI2c.open( BlackLib::ReadWrite | BlackLib::NonBlock );

    if( !isOpened )
    {
        std::cout << "I2C2 DEVICE CAN\'T OPEN.;" << std::endl;
        exit(1);
    }

    //control byte: 
    uint8_t ctrl_byte_en = 0x40; //enable analog out

    bool resultOfWrite = myI2c.writeByte(ctrl_byte_en, data);
    mdelay(10000);
 
    return resultOfWrite;
}

#endif

int main(int argc, char **argv) {
        ros::init(argc, argv, "was_control");

        was_lift *lift_controller = new was_lift;
        was_motor *motor_controller = new was_motor;
        //lift_controller->do_lift();
        BlackLib::BlackGPIO lift_GPIO_UP(BlackLib::BlackGPIO(CYCLINDER_PIN_0, BlackLib::output));
        BlackLib::BlackGPIO lift_GPIO_DOWN(BlackLib::BlackGPIO(CYCLINDER_PIN_1, BlackLib::output));
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
                if (motor_controller->is_motor_left) {
                        switch (motor_controller->digital) {
                                case LEFT_DECREASE:
                                        motor_controller->is_motor_left = true;
					data--;
                                       	printf("data = %d\n", data); 
					pcf8591_i2c2();
                                     	motor_controller->is_motor_left = 0; 
					break;
                                case LEFT_INCREASE:
                                        motor_controller->is_motor_left = true;
                                        data++;
                                       	printf("data = %d\n", data); 
                                        pcf8591_i2c2();
                                     	motor_controller->is_motor_left = 0; 
                                        break;
                        }
                }
#endif                
		ros::spinOnce();
        }

        // ros::spin();
        return 0;
}
