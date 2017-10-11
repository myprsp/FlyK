#!/usr/bin/env python
# -*- coding: UTF-8 -*-


import rospy
from geometry_msgs.msg import Twist
from std_msgs.msg import String
from math import radians
from myroboot.msg import myRobootMsg

from tf.transformations import euler_from_quaternion
from math import degrees

from sensor_msgs.msg import Imu
from geometry_msgs.msg import Quaternion

newAngle=0.0
oldAngle=0.0
angleDistance=0.0

def cmdCallback(data):
	if data.cmd=="NaviInit":
		scmd_vel = rospy.Publisher('cmd_vel_mux/input/navi', Twist, queue_size=10)
		initPub = rospy.Publisher('myroboot/naviInit',String)
		r = rospy.Rate(10)
		global newAngle
		global oldAngle
		global angleDistance

		angleDistance=0.0
		oldAngle=newAngle

		while angleDistance<360.0: 
			if (newAngle>0 and oldAngle<0)or(newAngle<0 and oldAngle>0):
				angleDistance=angleDistance+360.0-abs(newAngle)-abs(oldAngle)
			else:
				angleDistance=angleDistance+abs(newAngle-oldAngle)
			oldAngle=newAngle
			move_cmd = Twist()
			move_cmd.linear.x = 0
			move_cmd.angular.z = radians(10)
			scmd_vel.publish(move_cmd)
			r.sleep()
		if angleDistance>360.0:
			initPub.publish("naviInit_ok")


def ImuCallback(data):
	quat = data.orientation
	q = [quat.x, quat.y, quat.z, quat.w]
	roll, pitch, yaw = euler_from_quaternion(q)
	global newAngle
	newAngle=float(degrees(yaw))
	

	
	#sys.stdout.write("\r\033[1mGyro Angle\033[0m: [" + "{0:+.4f}".format(yaw) + " rad]  ["\
	#	 + "{0: >+7.2f}".format(degrees(yaw)) + " deg]"\
	#	 + "            \033[1mRate\033[0m: [" + "{0:+.4f}".format(data.angular_velocity.z) + " rad/s]  ["\
	#	 + "{0: >+7.2f}".format(degrees(data.angular_velocity.z)) + " deg/s] ")
	# http://docs.python.org/library/string.html#formatexamples
	#sys.stdout.flush()

if __name__ == '__main__':
	rospy.init_node('Navi_Init')
	rospy.Subscriber("my_roboot_cmd_topic", myRobootMsg, cmdCallback)
	rospy.Subscriber("/mobile_base/sensors/imu_data", Imu, ImuCallback)
	rospy.spin()



