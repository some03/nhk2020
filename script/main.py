#!/usr/bin/env python2
from package import read_waypoint as Wp
import math
import rospy
import actionlib
from nhk_2020.msg import *
import smach
import smach_ros
from std_msgs.msg import Float32
from std_msgs.msg import Int32
from std_msgs.msg import Bool
from geometry_msgs.msg import Twist
from std_msgs.msg import Empty


wp=Wp.File_Reader()
a=wp.assign(1)
print(a)

