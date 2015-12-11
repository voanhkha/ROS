#!/usr/bin/env python
import rospy
from std_msgs.msg import UInt32
from was_pid.srv import *
from was_pid.msg import *

def set_PID_value(request):
	msg.PID_Type = request.PID_component
	msg.PID_Coef = request.PID_value
	pub.publish(msg)
	print "set value %s = %s"%(request.PID_component, request.PID_value)
	return ["Successfully set value"]

rospy.init_node('PID_Tuning_Server')
pub = rospy.Publisher('PID_Tuning_Topic',PID_msg)
msg = PID_msg()

service = rospy.Service('PID_Tuning',PID_Tuning,set_PID_value)
print "PID Tuning Service ready"
rospy.spin()
