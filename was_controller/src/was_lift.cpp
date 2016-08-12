#include "was_lift.h"
#include "ros/ros.h"
#include "std_msgs/String.h"
#include "BlackLib/BlackLib.h"
#include <iostream>

#define WAS_DEBUG

#define CYCLINDER_PIN_0  BlackLib::GPIO_60
#define CYCLINDER_PIN_1  BlackLib::GPIO_115


was_lift::was_lift(void) {
        lift_sub = nh.subscribe("was_control/lift",
                1, &was_lift::lift_cb, this);
        lifting = LIFT_STOP;
        is_lifting = false;

#ifdef WAS_DEBUG
        std::cout << "Initialized was_lift" << std::endl;
#endif
}

void was_lift::lift_cb(const std_msgs::String::ConstPtr& cmd) {
#ifdef WAS_DEBUG
                std::cout << "lift_cb: ";
#endif
        if (cmd->data == "LIFT_UP") {
                lifting = LIFT_UP;
                is_lifting = true;
#ifdef WAS_DEBUG
                std::cout << "UP" << std::endl;
#endif
        }
        else if (cmd->data == "LIFT_DOWN") {
                lifting = LIFT_DOWN;
                is_lifting = true;
#ifdef WAS_DEBUG
                std::cout << "DOWN" << std::endl;
#endif
        }
        else if (cmd->data == "STOP") {
                lifting = LIFT_STOP;
                is_lifting = true;
#ifdef WAS_DEBUG
                std::cout << "STOP" << std::endl;
#endif
        }
}

void was_lift::do_lift(void) {
        BlackLib::BlackGPIO lift_GPIO_DOWN(BlackLib::BlackGPIO(CYCLINDER_PIN_0, BlackLib::output));
        BlackLib::BlackGPIO lift_GPIO_UP(BlackLib::BlackGPIO(CYCLINDER_PIN_1, BlackLib::output));
        while (ros::ok()) {
                if (is_lifting) {
                        switch (lifting) {
                                case LIFT_UP:
                                        lift_GPIO_UP.setValue(BlackLib::high);
                                        lift_GPIO_DOWN.setValue(BlackLib::low);
                                        is_lifting = false;
                                        std::cout << "Lifting UP" << std::endl;
                                        break;
                                case LIFT_DOWN:
                                        lift_GPIO_UP.setValue(BlackLib::low);
                                        lift_GPIO_DOWN.setValue(BlackLib::high);
                                        is_lifting = false;
                                        std::cout << "Lifting DOWN" << std::endl;
                                        break;
                                case LIFT_STOP:
                                        lift_GPIO_UP.setValue(BlackLib::low);
                                        lift_GPIO_DOWN.setValue(BlackLib::low);
                                        is_lifting = false;
                                        std::cout << "STOP" << std::endl;
                                        break;
                        }
                }
        }
}
