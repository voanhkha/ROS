# was_sensor

- Build NOTE: please copy BlackLib into rootfs:
	/usr/lib/BlackLib.so
	/usr/include/BlackLib/Black*/Black*.h

- Platform: ARMv7
- Run:
	# Please ensure roscore is running before executing following commands
	$ rosrun was_sensor range_sensor &
	$ rosrun was_sensor lineTracker_sensor
- ROS Nodes: (total = 2)
	+ range_sensor: read ADC value from ADC[0..4], send data to was_master
		< Publish:
			- range_pub:
				+ type	: std_msgs::UInt16MultiArray
				+ topic	: "was_sensor/ultrasound"
				+ rate	: 10Hz

	+ lineTracker_sensor: read binary value from GPIO connected to line tracking sensor, send data to was_master
		< Publish:
			- lineTracker_pub:
				+ type	: std_msgs::UInt32
				+ topic	: "was_sensor/lineTracking"
				+ rate	: 10Hz