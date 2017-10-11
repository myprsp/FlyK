#!/usr/bin/env python
# -*- coding: UTF-8 -*-

import rospy
#import time
import os
import signal
import subprocess
from myroboot.msg import myRobootMsg

mapping=None
rviz=None
navi=None
naviInit=None


def cmdCallback(data):
	if data.cmd=="start_mapping":
		global navi
		if navi is not None:
			global navi
			os.killpg(navi.pid, signal.SIGINT)
		global mapping
		mapping=subprocess.Popen(['gnome-terminal','--disable-factory','-e','roslaunch turbot_slam laser_gmapping_demo.launch'],preexec_fn=os.setpgrp)
	elif data.cmd=="stop_mapping":
		global mapping
		if mapping is not None:
			global mapping
			os.killpg(mapping.pid, signal.SIGINT)
	elif data.cmd=="start_navi":
		global mapping
		if mapping is not None:
			global mapping
			os.killpg(mapping.pid, signal.SIGINT)
		global navi
		navi=subprocess.Popen(['gnome-terminal', '--disable-factory','-e','roslaunch turbot_slam laser_amcl_tim561.launch map_file:=/home/myprsp/map/'+data.args[0]+'.yaml'],preexec_fn=os.setpgrp)
	elif data.cmd=="stop_navi":
		global navi
		if navi is not None:
			global navi
			os.killpg(navi.pid, signal.SIGINT)
	elif data.cmd=="start_rviz":
		global rviz
		rviz=subprocess.Popen(['gnome-terminal', '--disable-factory','-e','roslaunch turbot_rviz nav.launch --screen'],preexec_fn=os.setpgrp)
	elif data.cmd=="stop_rviz":
		global rviz
		if rviz is not None:
			global rviz
			os.killpg(rviz.pid, signal.SIGINT)
	elif data.cmd=="save_map":
		saveMap=subprocess.Popen(['gnome-terminal', '--disable-factory','-e','rosrun turbot_map saver '+data.args[0]],preexec_fn=os.setpgrp)
		#os.killpg(saveMap.pid, signal.SIGINT)

def listener():

	rospy.init_node('my_roboot')
	#rospy.init_node('listener', anonymous=True)
	rospy.Subscriber("my_roboot_cmd_topic", myRobootMsg, cmdCallback)
	# spin() simply keeps python from exiting until this node is stopped
	rospy.spin()
       

if __name__ == '__main__':

	subprocess.Popen(['gnome-terminal', '--disable-factory','-e','roslaunch turbot_bringup minimal.launch'],preexec_fn=os.setpgrp)
	listener()

