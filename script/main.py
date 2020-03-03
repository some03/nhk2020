#!/usr/bin/env python2
from package import read_waypoint as Wp
import smach
import smach_ros
from std_msgs.msg import Float32
from std_msgs.msg import Int32
from std_msgs.msg import Bool
from std_msgs.msg import Empty
import rospy
if __name__=="__main__":
    rospy.init_node('waypoint')
    #wp=Wp.File_Reader()
    #a=wp.route0(0)
    #print(a)
"""
class File_Number():
    count=0;
    def Count(self):
        File_Number.count+=1
        return File_Number.count

class Publish(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['auto','emergency'])
"""
