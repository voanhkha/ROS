#!/usr/bin/env python
import rospy
from std_msgs.msg import UInt32, String
from was_pid.msg import *
import serial

global Kp, Kd, Ki, posi_error, sum_posi_error, last_posi_error, MaxLeftSpeed, MaxRightSpeed, BaseLeftSpeed, BaseRightSpeed
Kp = 1
Kd = Ki = 0
posi_error = sum_posi_error = last_posi_error = 0
MaxLeftSpeed = MaxRightSpeed = 100
BaseLeftSpeed = BaseRightSpeed = 50

def IR_callback(msg):
	global Kp, Kd, Ki, posi_error, last_posi_error, sum_posi_error, MaxLeftSpeed, MaxRightSpeed, BaseLeftSpeed, BaseRightSpeed
	IR_values = format(msg.data, "05b") 
	posi_error = int(IR_values[0])*(-20) +int(IR_values[1])*(-10) + int(IR_values[3])*10 + int(IR_values[4])*20
	sum_posi_error = sum_posi_error + posi_error
	tuning = Kp*posi_error + Kd*(posi_error - last_posi_error) + Ki*sum_posi_error
	last_posi_error = posi_error
	LeftSpeed = BaseLeftSpeed + tuning
	RightSpeed = BaseRightSpeed - tuning
	UART1_pub.publish(str(LeftSpeed))
	UART2_pub.publish(str(RightSpeed))
	print tuning

def PID_Tuning_callback(msg):
	global Kp, Kd, Ki
	if msg.PID_Type == "Kp":
		Kp = msg.PID_Coef
	elif msg.PID_Type == "Kd":
		Kd = msg.PID_Coef
	elif msg.PID_Type == "Ki":
		Ki = msg.PID_Coef
	else:
		print 'PID_Type error!'		

rospy.init_node('IR_subscriber')
IR_sub = rospy.Subscriber('was_sensor/lineTracking', UInt32, IR_callback)
PID_sub = rospy.Subscriber('PID_Tuning_Topic', PID_msg, PID_Tuning_callback)
UART1_pub = rospy.Publisher('UART1_topic',String)
UART2_pub = rospy.Publisher('UART2_topic',String)
rospy.spin()
