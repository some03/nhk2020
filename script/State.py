#!/usr/bin/env python

import rospy
import smach
import smach_ros
from std_msgs.msg import Float32
from std_msgs.msg import Int32
from std_msgs.msg import Bool
from geometry_msgs.msg import Twist
from std_msgs.msg import Empty
import actionlib

class Switch(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['auto','manual'])
        self.sw_sub=rospy.Subscriber('switch',Bool,self.swCb)
        self.ord_sub=rospy.Subscriber('order',Twist,self.ordCb)
        self.cmd_pub=rospy.Publisher("cmd_vel",Twist)
        self.switch=False
        self.x=0
        self.y=0
        self.z=0
        self.mg=Twist()
        self.auto_pub=rospy.Publisher('Go',Bool)

    def swCb(self,msg):
        self.switch=msg.data
    def ordCb(self,mg):
        self.x=mg.linear.x
        self.y=mg.linear.y
        self.z=mg.angular.z
    
    def execute(self,data):
        rospy.sleep(1)
        if self.switch:
            return 'auto' 
        else:
            rospy.loginfo('manual state')
            
            self.Mg=False
            self.auto_pub.publish(self.Mg)
            self.mg.linear.x=self.x
            self.mg.linear.y=self.y
            self.mg.angular.z=self.z
            self.cmd_pub.publish(self.mg)
            
            return 'manual'


class Set(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['init'])
        self.set_pub=rospy.Publisher('reset',Empty)
        self.auto_pub=rospy.Publisher('Go',Bool)
        self.Mg=True

    def execute(self,data):
        rospy.loginfo('init state')
        rospy.sleep(1)
        self.set_pub.publish()
        rospy.sleep(0.5)
        self.auto_pub.publish(self.Mg)
        return 'init'

class Pole1(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['progress','next','emergency'])
        self.pub1=rospy.Publisher("cmd_ord",Twist)

        self.sw_sub=rospy.Subscriber('switch',Bool,self.swCb)
        self.switch=True
        self.Ms=Twist()

        self.cnt=0;
        
    def swCb(self,msg):
        self.switch=msg.data
        
    def swCb(self,msg):
        self.switch=msg.data
    def execute(self,data): 
        rospy.sleep(0.5)
        
        if self.cnt>=30:
            return 'next'
        
        if not self.switch:
            return 'emergency'
        else:
            self.Ms.linear.x=500
            self.Ms.linear.y=300
            self.Ms.angular.z=0
            self.pub1.publish(self.Ms)
            self.cnt+=1
            rospy.loginfo('pole1 state')
            return 'progress'

class Pole2(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['progress','next','emergency'])
        self.pub1=rospy.Publisher("cmd_ord",Twist)

        self.sw_sub=rospy.Subscriber('switch',Bool,self.swCb)
        self.switch=True
        self.Ms=Twist()

        self.cnt=0;
        
    def swCb(self,msg):
        self.switch=msg.data
        
    def swCb(self,msg):
        self.switch=msg.data
    def execute(self,data): 
        rospy.sleep(0.5)
        
        if self.cnt>=30:
            return 'next'
        
        if not self.switch:
            return 'emergency'
        else:
            self.Ms.linear.x=-500
            self.Ms.linear.y=300
            self.Ms.angular.z=0
            self.pub1.publish(self.Ms)
            self.cnt+=1
            rospy.loginfo('pole2 state')
            return 'progress'

class Pole3(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['progress','next','emergency'])
        self.pub1=rospy.Publisher("cmd_ord",Twist)

        self.sw_sub=rospy.Subscriber('switch',Bool,self.swCb)
        self.switch=True
        self.Ms=Twist()

        self.cnt=0;
        
    def swCb(self,msg):
        self.switch=msg.data
        
    def swCb(self,msg):
        self.switch=msg.data
    def execute(self,data): 
        rospy.sleep(0.5)
        
        if self.cnt>=60:
            return 'next'
        
        if not self.switch:
            return 'emergency'
        else:
            self.Ms.linear.x=0
            self.Ms.linear.y=-600
            self.Ms.angular.z=0
            self.pub1.publish(self.Ms)
            self.cnt+=1
            rospy.loginfo('pole3 state')
            return 'progress'

class End(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['end','emergency'])
        self.pub1=rospy.Publisher("cmd_ord",Twist)

        self.sw_sub=rospy.Subscriber('switch',Bool,self.swCb)
        self.switch=True
        self.Ms=Twist()

        
    def swCb(self,msg):
        self.switch=msg.data
        
    def swCb(self,msg):
        self.switch=msg.data
    def execute(self,data): 
        rospy.sleep(0.5)
        
        
        if not self.switch:
            return 'emergency'
        else:
            self.Ms.linear.x=0
            self.Ms.linear.y=0
            self.Ms.angular.z=0
            self.pub1.publish(self.Ms)
            rospy.loginfo('end')
            return 'progress'
class main():
    rospy.init_node('robot_order')
    sm=smach.StateMachine(
        outcomes=['outcomes'])
    with sm:
        smach.StateMachine.add('SWITCH',Switch(),
                                transitions={'manual':'SWITCH',
                                             'auto':'SET'})
        smach.StateMachine.add('SET',Set(),
                                transitions={'init':'POLE1'})
        

        smach.StateMachine.add('POLE1',Pole1(),
                                transitions={'progress':'POLE1',
                                             'next':'POLE2',
                                             'emergency':'SWITCH'})
        
        smach.StateMachine.add('POLE2',Pole2(),
                                transitions={'progress':'POLE2',
                                             'next':'POLE3',
                                             'emergency':'SWITCH'})

        smach.StateMachine.add('POLE3',Pole3(),
                                transitions={'progress':'POLE3',
                                             'next':'END',
                                             'emergency':'SWITCH'})

        smach.StateMachine.add('END',End(),
                                transitions={'end':'END',
                                             'emergency':'SWITCH'})
        sis=smach_ros.IntrospectionServer('server_name',sm,'/SM_ROOT')
        sis.start()

        outcome=sm.execute()
        rospy.spin()


if __name__=='__main__':
    main()


                                
