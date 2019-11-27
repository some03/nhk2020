#!/usr/bin/env python

import rospy
import actionlib

from nhk_2020.msg import taskAction
from nhk_2020.msg import taskGoal
from nhk_2020.msg import taskResult

from nhk_2020.msg import *

def action():
    rospy.loginfo("go")
    client=actionlib.SimpleActionClient('Action',taskAction)
    rospy.loginfo("a")
    client.wait_for_server()
    rospy.loginfo("start")

    goal=taskGoal()
    
    goal.Goal.linear.x=100
    goal.Goal.linear.y=0
    goal.Goal.angular.z=0
    
    client.send_goal(goal)
    client.wait_for_result()
    
    result=client.get_result()

    rospy.loginfo("progress")
    
    if  result:
        rospy.loginfo('ok')
    else:
        rospy.loginfo('faild')
     
if __name__=='__main__':
    try:
        rospy.init_node('test_client')
        action()
        
    except rospy.ROSInterruptException:
        pass
