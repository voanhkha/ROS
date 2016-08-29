# ROS rev.1 (26/8/2016)
ROS (Robot Operating System) on Ubuntu+Beaglebone

+ Terminal 1: (use keyboard to control the robot)  
roscore &  
rosrun was_i2c i2c_motor &  
rosrun was_master was_teleop  

+ Terminal 2: (for sensors)  
rosrun was_sensor lineTracker_sensor &  
rosrun was_gyro gyro_sensor &  

+ Terminal 3: (for PID tuning)  
rosrun was_pid PID_Compute.py  


+ How to control the robot: (on Terminal 1)   
W: forward mode and spin 2 wheels  
S: backward mode and spin 2 wheels  
A: spin left wheel with the previous mode  
D: spin right wheel with the previous mode  
P: enable PID mode (need to manually move the robot onto the line)  
SPACE: stop 2 wheels, stop PID mode  
B: Brake left wheel  
M: Brake right wheel  
N: Cancel brake on 2 wheels  
All of the control functions can be found in /was_i2c/src/i2c_motor.cpp. This file receives topic messages directly from was_master/src/teleop.cpp and calls the callbacks for each keypad.  

+ PID algorithm:  
BaseLeftSpeed, BaseRightSpeed: speed for each wheel when it moves straight on the line.  
MaxLeftSpeed, MaxRightSpeed: maximum speed for each wheel to avoid high speed if we tune PID too much.  
posi: the position value of 5 infrared (IR) sensors: more negative if more out on the left and more positive if more out on the right.  
tuning: the value to be summed/subtracted to left wheel and subtracted/summed to the right wheel if the robot is out on the left/right of the line.  
Kp: greater Kp -> greater tuning.  
Kd: need to be much greater than Kp -> decrease zigzag amplitude.  
Ki: small and may not be used.  

+ How to tune PID:  
Open Terminal 1, press SPACE to stop and disable PID mode.
Ctrl-C (stop) the PID_Compute.py node on Terminal 3.  
Open was_pid/src/PID_Compute.py to edit the PID values.  
Revise Kp, Kd, BaseLeftSpeed, BaseRightSpeed, MaxLeftSpeed, MaxRightSpeed.  
"rosrun was_pid PID_Compute.py" without catkin_make.  
Manually move the robot onto the line.  
Open terminal 1 and press P to enable PID mode and watch the robot moving.   

+ Spinning for WAS Robot:  

Running was_gyro/src/gyro_sensor.cpp. This ROS node waits for a message from topic "SpinAngle". The message is a number indicating the angle for spinning (can be negative).    

Publish the message directly by:  
rostopic pub SpinAngle std_msgs/Int16 -- <angle>  
Example:  
rostopic pub SpinAngle std_msgs/Int16 -- 90 (spin 90 counter-clockwise)  
rostopic pub SpinAngle std_msgs/Int16 -- -45 (spin 45 clockwise)  

We can publish the message to the 'SpinAngle' topic from another node as well.  
