# was_master

- Platform: x86, x86_64
- ROS Nodes: (total = 2)
	+ was_master: receive message from sensors, commands from teleop, 
		      send control commands to actuators/motors
		> Subscribe:
			- range_sub: Ultrasonic range sensors
				+ type 		: std_msgs<std_msgs::UInt16MultiArray>
				+ topic		: "was_sensor/ultrasound"
				+ callback 	: range_cb

			- lineTracking_sub: line tracking sensors
				+ type		: std_msgs::UInt32
				+ topic		: "was_sensor/lineTracking"
				+ callback 	: lineTracking_cb

			- lift_sub: receive keyboard input command keys
				+ type		: std_msgs::String
				+ topic		: "was_teleop/lift"
				+ callback 	: lift_cb

		< Publish:
			- lift_pub: receive teleop cmd, send commands to was_controller
				+ type	: std_msgs::String
				+ topic	: "was_sensor/ultrasound"
				+ rate	: 10Hz

	+ was_teleop: read user keyboard input, send command keys to was_master
		< Publish:
			- lift_pub: read keyboard input, send teleop cmds to was_master
				+ type	: std_msgs::String
				+ topic	: "was_teleop/lift"
				+ rate	: every time predefined key is pressed

			- movement_pub: 
				+ type	: std_msgs::String
				+ topic	: "was_teleop/movement"
				+ rate	: every time predefined key is pressed
			