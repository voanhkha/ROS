#include "ros/ros.h"
#include "std_msgs/String.h"
#include "std_msgs/UInt32.h"
#include <signal.h>
#include <termios.h>
#include <stdio.h>
#include "teleop.h"

// For moving
#define KEYCODE_RIGHT           0x64    // D
#define KEYCODE_LEFT            0x61    // A
#define KEYCODE_FORWARD         0x77    // W
#define KEYCODE_BACKWARD        0x73    // S
#define KEYCODE_P		0x70      // P
#define KEYCODE_B		0x62	//B
#define KEYCODE_N		0x6E	//N
#define KEYCODE_M		0x6D	//M
// For lift
#define KEYCODE_UP              0x65    // E
#define KEYCODE_DOWN            0x63    // C

// For both lift and moving
#define KEYCODE_STOP            0x20    // SPACE

// For PCF8591
#define KEYCODE_1		0x31 //Left wheel decrease		
#define KEYCODE_2		0x32 //Left wheel increase
#define KEYCODE_8               0x38 //Right wheel decrease           
#define KEYCODE_9               0x39 //Right wheel increase
#define KEYCODE_0               0x30 //Both wheel stop

#define KEYCODE_LEFT_3               0x33 //Left wheel Forward
#define KEYCODE_LEFT_4               0x34 //Left wheel Backward

#define KEYCODE_RIGHT_5               0x35 //Right wheel Forward
#define KEYCODE_RIGHT_6               0x36 //Right wheel Backward

// #define WAS_DEBUG

int kfd = 0;
struct termios cooked, raw;

was_teleop::was_teleop()
{
        movement_pub = nh.advertise<std_msgs::String>("was_teleop/movement", 1000);
        lift_pub = nh.advertise<std_msgs::String>("was_teleop/lift", 1);
	motor_left_pub = nh.advertise<std_msgs::String>("was_teleop/motor_left", 1);
	motor_right_pub = nh.advertise<std_msgs::String>("was_teleop/motor_right", 1);
	motor_stop_pub = nh.advertise<std_msgs::String>("was_teleop/motor_stop", 1);
	motor_right_dir_pub = nh.advertise<std_msgs::String>("was_teleop/motor_right_dir", 1);
	motor_left_dir_pub = nh.advertise<std_msgs::String>("was_teleop/motor_left_dir", 1);
}

void was_teleop::keyLoop()
{
        char c;
        bool is_moving=false;
        bool is_lifting=false;
	bool is_motor_left=false;
	bool is_motor_right=false;
	bool is_stop=false;
	bool is_left_forward=false;
	bool is_left_backward=false;
	bool is_right_forward=false;
	bool is_right_backward=false;

        // get the console in raw mode
        tcgetattr(kfd, &cooked);
        memcpy(&raw, &cooked, sizeof(struct termios));
        raw.c_lflag &=~ (ICANON | ECHO);
        // Setting a new line, then end of file
        raw.c_cc[VEOL] = 1;
        raw.c_cc[VEOF] = 2;
        tcsetattr(kfd, TCSANOW, &raw);

        puts("Reading from keyboard");
        puts("---------------------------");
        puts("Use keys to move the Robot.");
        puts("W: Forward 2 wheels");
	puts("S: Backward 2 wheels");
	puts("A: Spin LEFT");
	puts("D: Spin RIGHT");
	puts("SPACE: Stop and disable PID");
	puts("P: Enable PID");
	puts("B: Brake LEFT");
	puts("M: Brake RIGHT");
	puts("N: Brake 2 wheels");
        puts("---------------------------");

	std_msgs::String topicdata;
        std::string movement_cmd;
        std::string lift_cmd;
	std::string motor_left_cmd;
	std::string motor_right_cmd;
	std::string motor_stop_cmd;
	std::string motor_left_dir_cmd;
	std::string motor_right_dir_cmd;

        while(1)
        {
                // get the next event from the keyboard
                if(read(kfd, &c, 1) < 0)
                {
                        perror("read():");
                        exit(-1);
                }

                ROS_DEBUG("value: 0x%02X\n", c);

#ifdef  WAS_DEBUG
                std::cout << "Pressing: " << std::hex << std::showbase << (int) c << std::endl;
#endif

                switch(c)
                {
                        case KEYCODE_LEFT:
                       		topicdata.data = "Left";
				movement_pub.publish(topicdata);
                                break;
                        case KEYCODE_RIGHT:           
			       topicdata.data = "Right";
                               movement_pub.publish(topicdata);
                                break;
                        case KEYCODE_FORWARD:
                               topicdata.data = "Forward";
                               movement_pub.publish(topicdata);
				break;
                        case KEYCODE_P:
                               topicdata.data = "PID_en";
                               movement_pub.publish(topicdata);
                                break;
                        case KEYCODE_BACKWARD:
                                topicdata.data = "Backward";
                                movement_pub.publish(topicdata);
                                break;
                        case KEYCODE_B:
                                topicdata.data = "BrL";
                                movement_pub.publish(topicdata);
                                break;
                        case KEYCODE_M:
                                topicdata.data = "BrR";
                                movement_pub.publish(topicdata);
                                break;
                        case KEYCODE_N:
                                topicdata.data = "Br2";
                                movement_pub.publish(topicdata);
                                break;
                        case KEYCODE_UP:
                                ROS_DEBUG("LIFT_UP");
                                lift_cmd = "LIFT_UP";
                                is_lifting = true;
                                break;
                        case KEYCODE_DOWN:
                                ROS_DEBUG("LIFT_DOWN");
                                lift_cmd = "LIFT_DOWN";
                                is_lifting = true;
                                break;
                        case KEYCODE_STOP:
                               topicdata.data = "Stop";
                               movement_pub.publish(topicdata);
                               break;
                	case KEYCODE_1:
				ROS_DEBUG("MOTOR LEFT");
				is_motor_left = true;
				motor_left_cmd = "LEFT_DECREASE";
  				break;
                        case KEYCODE_2:
                                ROS_DEBUG("MOTOR LEFT");
                                is_motor_left = true;
                                motor_left_cmd = "LEFT_INCREASE";
				break;
          
	                case KEYCODE_8:
                                ROS_DEBUG("MOTOR RIGHT");
                                is_motor_right = true;
                                motor_right_cmd = "RIGHT_DECREASE";
                                break;
                        case KEYCODE_9:
                                ROS_DEBUG("MOTOR RIGHT");
                                is_motor_right = true;
                                motor_right_cmd = "RIGHT_INCREASE";
                                break;
                        case KEYCODE_0:
                                ROS_DEBUG("MOTOR STOP");
                                is_stop = true;
                                motor_stop_cmd = "MOTOR_STOP";
                                break;

                        case KEYCODE_LEFT_3:
                                ROS_DEBUG("LEFT FORWARD");
                                is_left_forward = true;
                                motor_left_dir_cmd = "LEFT_FORWARD";
                                break;

                        case KEYCODE_LEFT_4:
                                ROS_DEBUG("LEFT BACKWARD");
                                is_left_backward = true;
                                motor_left_dir_cmd = "LEFT_BACKWARD";
                                break;

                        case KEYCODE_RIGHT_5:
                                ROS_DEBUG("RIGHT FORWARD");
                                is_right_forward = true;
                                motor_right_dir_cmd = "RIGHT_FORWARD";
                                break;

                        case KEYCODE_RIGHT_6:
                                ROS_DEBUG("RIGHT BACKWARD");
                                is_right_backward = true;
                                motor_right_dir_cmd = "RIGHT_BACKWARD";
                                break;

		}

                if (is_moving) {
                        std_msgs::String cmd;
                        cmd.data = movement_cmd;
                        movement_pub.publish(cmd);
                        is_moving=false;
                }
                if (is_lifting) {
                        std_msgs::String cmd;
                        cmd.data = lift_cmd;
                        lift_pub.publish(cmd);
                        is_lifting=false;
                }
		if (is_motor_left) {
                        std_msgs::String cmd;
                        cmd.data = motor_left_cmd;
                        motor_left_pub.publish(cmd);
                        is_motor_left=false;

		}
                if (is_motor_right) {
                        std_msgs::String cmd;
                        cmd.data = motor_right_cmd;
                        motor_right_pub.publish(cmd);
                        is_motor_right=false;

                }
            	if (is_stop) {
                        std_msgs::String cmd;
                        cmd.data = motor_stop_cmd;
                        motor_stop_pub.publish(cmd);
                        is_stop=false;
                        is_motor_left=false;
                        is_motor_right=false;
                }

                if (is_left_forward) {
                        std_msgs::String cmd;
                        cmd.data = motor_left_dir_cmd;
                        motor_left_dir_pub.publish(cmd);
                        is_left_forward=false;
                }

                if (is_left_backward) {
                        std_msgs::String cmd;
                        cmd.data = motor_left_dir_cmd;
                        motor_left_dir_pub.publish(cmd);
                        is_left_backward=false;
                }

                if (is_right_forward) {
                        std_msgs::String cmd;
                        cmd.data = motor_right_dir_cmd;
                        motor_right_dir_pub.publish(cmd);
                        is_right_forward=false;
                }

                if (is_right_backward) {
                        std_msgs::String cmd;
                        cmd.data = motor_right_dir_cmd;
                        motor_right_dir_pub.publish(cmd);
                        is_right_backward=false;
                }

	}
}

void quit(int sig)
{
        (void)sig;
        tcsetattr(kfd, TCSANOW, &cooked);
        ros::shutdown();
        exit(0);
}
