#include "ros/ros.h"
#include "was_lift.h"
#include "BlackLib/BlackLib.h"

#define CYCLINDER_PIN_0  BlackLib::GPIO_60
#define CYCLINDER_PIN_1  BlackLib::GPIO_115

int main(int argc, char **argv) {
        ros::init(argc, argv, "was_control");

        was_lift *lift_controller = new was_lift;
        // lift_controller->do_lift();
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
                ros::spinOnce();
        }

        // ros::spin();
        return 0;
}