#!/usr/bin/env python
import rospy
from std_msgs.msg import UInt32
from was_pid.srv import *
import sys

rospy.init_node('PID_Tuning_Client')
rospy.wait_for_service('PID_Tuning')

rospy.spin()
