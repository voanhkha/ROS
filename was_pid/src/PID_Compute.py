#!/usr/bin/env python
import rospy
from std_msgs.msg import UInt8, UInt32, String
from was_pid.msg import *

global Kp, Kd, Ki, step1, step2, posi_error, sum_posi_error, last_posi_error, MaxLeftSpeed, MaxRightSpeed, BaseLeftSpeed, BaseRightSpeed,PID_Enable, LeftWheel_pub, RightWheel_pub, LeftBrake_pub, RightBrake_pub
Kp = 6
Kd = 20
Ki = 0
step1 = 1
step2 = 2
decrease = 0
posi_error = sum_posi_error = last_posi_error = 0
MaxLeftSpeed = MaxRightSpeed = 200
BaseLeftSpeed = 52
BaseRightSpeed = 55
PID_Enable = 0
def IR_callback(msg):
	global LeftWheel_pub, RightWheel_pub, LeftBrake_pub, RightBrake_pub, PID_Enable, Kp, Kd, Ki, step1, step2, posi_error, last_posi_error, sum_posi_error, MaxLeftSpeed, MaxRightSpeed, BaseLeftSpeed, BaseRightSpeed
	if PID_Enable == 1:
		IR_values = format(msg.data, "05b") 
		posi_error = int(IR_values[4])*(-1)*step1 +int(IR_values[3])*(-1)*step2 + int(IR_values[1])*step2 + int(IR_values[0])*step1
		sum_posi_error = sum_posi_error + posi_error
		tuning = Kp*posi_error + Kd*(posi_error - last_posi_error) + Ki*sum_posi_error
		last_posi_error = posi_error
		LeftSpeed = BaseLeftSpeed - tuning
		RightSpeed = BaseRightSpeed + tuning
	#	if posi_error == 0:
	#		LeftBrake = 0
	#		RightBrake = 0
	#	else: 
	#		LeftBrake = 38
	#		RightBrake = 38
		if LeftSpeed > MaxLeftSpeed:
			LeftSpeed = MaxLeftSpeed
		if RightSpeed > MaxRightSpeed:
			RightSpeed = MaxRightSpeed
		if LeftSpeed < 0:
			LeftSpeed = 0
		if RightSpeed < 0:
			RightSpeed = 0
	        LeftWheel_pub.publish(LeftSpeed)
    	        RightWheel_pub.publish(RightSpeed)
	#	LeftBrake_pub.publish(LeftBrake)
	#	RightBrake_pub.publish(RightBrake)
	else:
		LeftSpeed = 0
		RightSpeed = 0
	#print "L: %d " % LeftSpeed
	#print "R: %d" % RightSpeed
def PID_Tuning_callback(msg):
	global PID_Enable, Kp, Kd, Ki, step1, step2, MaxLeftSpeed, MaxRightSpeed, BaseLeftSpeed, BaseRightSpeed
	if msg.PID_Type == "Kp" or msg.PID_Type == "kp":
		Kp = msg.PID_Coef
        elif msg.PID_Type == "Kd" or msg.PID_Type == "kd":
		Kd = msg.PID_Coef
	elif msg.PID_Type == "Ki" or msg.PID_Type == "ki":
		Ki = msg.PID_Coef
	elif msg.PID_Type == "step2":
		step2 = msg.PID_Coef
	elif msg.PID_Type == "step1":
		step1 = msg.PID_Coef
	elif msg.PID_Type == "MLS" or msg.PID_Type == "mls":
		MaxLeftSpeed = msg.PID_Coef
	elif msg.PID_Type == "MRS" or msg.PID_Type == "mrs":
		MaxRightSpeed = msg.PID_Coef
        elif msg.PID_Type == "BLS" or msg.PID_Type == "bls":
                BaseLeftSpeed = msg.PID_Coef
        elif msg.PID_Type == "BRS" or msg.PID_Type == "brs":
                BaseRightSpeed = msg.PID_Coef
	elif msg.PID_Type == "en" or msg.PID_Type == "enable":
		PID_Enable = msg.PID_Coef
	else:
		print 'PID_Type error'		

def Control_callback(msg):
        global PID_Enable
	if msg.data == "Stop":
		PID_Enable = 0
	elif msg.data == "PID_en":
		PID_Enable = 1


rospy.init_node('IR_subscriber')
IR_sub = rospy.Subscriber('was_sensor/lineTracking', UInt8, IR_callback)
PID_sub = rospy.Subscriber('PID_Tuning_Topic', PID_msg, PID_Tuning_callback)
Control_sub = rospy.Subscriber('was_teleop/movement', String, Control_callback)
LeftWheel_pub = rospy.Publisher('LW',UInt8)
RightWheel_pub = rospy.Publisher('RW',UInt8)
LeftBrake_pub = rospy.Publisher('LB',UInt8)
RightBrake_pub = rospy.Publisher('RB',UInt8)

rospy.spin()
