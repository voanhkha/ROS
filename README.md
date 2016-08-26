# ROS rev.1 (26/8/2016)
ROS (Robot Operating System) on Ubuntu+Beaglebone

+ Terminal 1: (use keyboard to control the robot)
roscore &;
rosrun was_i2c i2c_motor &;
rosrun was_master was_teleop;

+ Terminal 2: (for sensors)
rosrun was_sensor lineTracker_sensor &
rosrun was_gyro gyro_sensor &

+ Terminal 3: (for PID tuning)
rosrun was_pid PID_Compute.py


+ How to control the robot: (on Terminal 1) 
- W: forward mode and spin 2 wheels
- S: backward mode and spin 2 wheels
- A: spin left wheel with the previous mode
- D: spin right wheel with the previous mode
- P: enable PID mode (need to manually move the robot onto the line)
- SPACE: stop 2 wheels, stop PID mode
- B: Brake left wheel
- M: Brake right wheel
- N: Cancel brake on 2 wheels
- All of the control functions can be found in /was_i2c/src/i2c_motor.cpp. This file receives topic messages directly from was_master/src/teleop.cpp and calls the callbacks for each keypad.

+ How to tune PID: 
- Ctrl-C (stop) the PID_Compute.py node on Terminal 3 and revise it.
- 
