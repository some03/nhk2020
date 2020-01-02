#!/usr/bin/env python2
from jsk_rviz_plugins.msg import *
from jsk_rviz_plugins.msg import *

from std_msgs.msg import ColorRGBA, Float32
from geometry_msgs.msg import Twist
import rospy
import math
import random, math
rospy.init_node("imfo")


num=0

text_pub = rospy.Publisher("text_sample", OverlayText, queue_size=1)
value_pub = rospy.Publisher("value_sample", Float32, queue_size=1)


loop_rate= rospy.Rate(1)
while not rospy.is_shutdown():

  def odm_cb(mg):
        x=mg.linear.x
        y=mg.linear.y
        z=mg.angular.z
        rospy.loginfo(x)

        text = OverlayText()
        text.width= 500
        text.height = 400
        #text.height = 600
        text.left = 10
        text.top = 10
        text.text_size = 17
        text.line_width = 2
        text.font = "DejaVu Sans Mono"
        text.text = """   TRY ROBOT
        
        Zone:Blue
        Mode:Auto
        Try:%d
        -----switch-----
        ^  catch
        o  auto
        +  emergency(manual)
        {} try
        =====odmetry======
        X : %d
        Y : %d
        Z : %d""" % (num, x, y,z)
        text.fg_color = ColorRGBA(25 / 255.0, 1.0, 240.0 / 255.0, 1.0)
        text.bg_color = ColorRGBA(0.0, 0.0, 0.0, 0.2)
        text_pub.publish(text)

  odm_sub=rospy.Subscriber('cmd_pos',Twist,odm_cb)
  loop_rate.sleep()

