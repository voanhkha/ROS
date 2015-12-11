#!/usr/bin/env python
import rospy
from std_msgs.msg import UInt32
from was_pid.msg import *
import Adafruit_BBIO.UART as UART
import serial

UART.setup("UART2")
ser = serial.Serial(port="/dev/ttyS2", baudrate=9600)
ser.close()
ser.open()
if ser.isOpen():
	print "Serial Open!"
	ser.write("Hello!")
ser.close()

global Kp, Kd, Ki
Kp = Kd = Ki = 0
global posi_error
global sum_posi_error
global last_posi_error
posi_error = sum_posi_error = last_posi_error = 0
MaxLeftSpeed = 100
MaxRightSpeed = 100

def IR_callback(msg):
	global Kp, Kd, Ki, posi_error, last_posi_error, sum_posi_error
	IR_values = format(msg.data, "05b") 
	posi_error = int(IR_values[0])*(-20) +int(IR_values[1])*(-10) + int(IR_values[3])*10 + int(IR_values[4])*20
	sum_posi_error = sum_posi_error + posi_error
	tuning = Kp*posi_error + Kd*(posi_error - last_posi_error) + Ki*sum_posi_error
	last_posi_error = posi_error
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
IR_sub = rospy.Subscriber('/was_sensor/movement/lineTracking', UInt32, IR_callback)
PID_sub = rospy.Subscriber('PID_Tuning_Topic', PID_msg, PID_Tuning_callback)
rospy.spin()
