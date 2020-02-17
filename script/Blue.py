#!/usr/bin/env python2
#355.3mm 28.1 x4.4
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

class Distance():
    count=2
    def Count(self):
        Distance.count-=1
        return Distance.count
class Way_Back():
    count=2
    def Count(self):
        Way_Back.count-=1
        return Way_Back.count

class Switch(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['auto','manual'])
        self.sw_sub=rospy.Subscriber('switch',Bool,self.swcb)
        #self.ord_sub=rospy.Subscriber('order',Twist,self.ordcb)
        self.cmd_pub=rospy.Publisher("cmd_vel",Twist)
        self.switch=False
        self.x=0
        self.y=0
        self.z=0
        self.count=0
        self.mg=Twist()
        self.auto_pub=rospy.Publisher('manual',Bool)

    def swcb(self,msg):
        self.switch=msg.data
    def ordcb(self,mg):
        self.x=mg.linear.x
        self.y=mg.linear.y
        self.z=mg.angular.z
    
    def execute(self,data):
        rospy.sleep(0.1)
        if  self.switch:
            self.Mg=True
            self.auto_pub.publish(self.Mg)
            return 'auto' 
        else:
            rospy.loginfo('manual state')
            
            self.Mg=False
            self.auto_pub.publish(self.Mg)
            return 'manual'


class Set(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['init'])
        self.set_pub=rospy.Publisher('reset',Empty)
        self.auto_pub=rospy.Publisher('Go',Bool)
        self.mg=True

    def execute(self,data):
        rospy.loginfo('init state')
        rospy.sleep(1)
        self.set_pub.publish()
        rospy.sleep(0.5)
        self.auto_pub.publish(self.mg)
        return 'init'

class Start(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['start','next','emergency'])
        self.sw_sub=rospy.Subscriber('switch',Bool,self.swcb)
        self.client=actionlib.SimpleActionClient('action',taskAction)
        self.reset_pub=rospy.Publisher('reset',Empty)
        self.switch=True
        self.action=False
        self.result=False

        
    def swcb(self,msg):
        self.switch=msg.data
    
    def execute(self,data):
        rospy.sleep(0.1)
        
        if not self.switch:
            self.client.cancel_goal();
            Switch.count=0
            return 'emergency'
        else:
            if not self.action:
                self.client.wait_for_server()
                goal=taskGoal()

                goal.Goal.linear.x=0
        
                goal.Goal.linear.y=5000#12810#5900

                goal.Goal.angular.z=0
                self.client.send_goal(goal)
                self.action=True
                return 'start'
            if not self.result and self.action:
                self.result=self.client.get_result()
                rospy.loginfo(self.result)
                return 'start'
            if self.result and self.action:
                #self.reset_pub.publish()
                return 'next'

class Wait(smach.State):
    def __init__(self):
        
        smach.State.__init__(self,outcomes=['wait','next'])
        self.ord_sub=rospy.Subscriber('order',Int32,self.ordcb)
        self.try_sub=rospy.Subscriber('catch_ball',Bool,self.ctcb)
        self.cmd_pub=rospy.Publisher("cmd_vel",Twist)
        self.reset_pub=rospy.Publisher('reset',Empty)
        self.go_pub=rospy.Publisher('Go',Bool)
        self.x=0
        self.y=0
        self.z=0
        self.catch=False
        self.count=0
        self.mg=Twist()
        self.auto_pub=rospy.Publisher('manual',Bool)

    def ordcb(self,ms):
        #self.x=mg.linear.x
        #self.y=mg.linear.y
        #self.z=mg.angular.z
        self.count+=ms.data 
    def ctcb(self,Msg):
        self.catch=Msg.data
    
    def execute(self,data):
        rospy.sleep(0.1)
            
        if self.catch:
            self.Mg=True
            self.reset_pub.publish()
            rospy.sleep(0.5)
            self.auto_pub.publish(self.Mg)
            
            #self.go_pub.publish(self.Mg);
            
            self.catch=False
            return 'next'
            
        else:
            rospy.loginfo('wait state')
            self.Mg=False
            self.auto_pub.publish(self.Mg)
            
            #self.Mg=False
            
            self.auto_pub.publish(self.Mg)
            self.go_pub.publish(self.Mg)
            return 'wait'

class Position(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['position','next','emergency'])
        self.sw_sub=rospy.Subscriber('switch',Bool,self.swcb)
        self.reset_pub=rospy.Publisher('reset',Empty)
        self.client=actionlib.SimpleActionClient('action',taskAction)     
        self.go_pub=rospy.Publisher('Go',Bool)
        self.auto_pub=rospy.Publisher('manual',Bool)
        self.switch=True
        self.action=False
        self.result=False

        
    def swcb(self,msg):
        self.switch=msg.data
    
    def execute(self,data):
        rospy.sleep(0.5)
        
        if not self.switch:
            self.client.cancel_goal();
            return 'emergency'
        else:
            if not self.action:
                self.reset_pub.publish()
                self.client.wait_for_server()
                goal=taskGoal()

                goal.Goal.linear.x=2596*0.6
                goal.Goal.linear.y=-4827*0.6
                goal.Goal.angular.z=0
                self.client.send_goal(goal)

                self.action=True
                return 'position'
            if not self.result and self.action:
                self.Mg=True

                self.go_pub.publish(self.Mg);
                self.result=self.client.get_result()
                rospy.loginfo(self.result)
                return 'position'
            if self.result and self.action:
                
                self.result=False
                self.action=False
                self.reset_pub.publish()
                return 'next'
class Try_Phase0(smach.State):
    
    def __init__(self):
        smach.State.__init__(self,outcomes=['phase0','next','emergency'])
        self.sw_sub=rospy.Subscriber('switch',Bool,self.swcb)
        self.reset_pub=rospy.Publisher('reset',Empty)
        self.client=actionlib.SimpleActionClient('action',taskAction)
        self.switch=True
        self.action=False
        self.result=False
        
    def swcb(self,msg):
        self.switch=msg.data
    
    def execute(self,data):
        rospy.sleep(0.1)
        
        if not self.switch:
            self.client.cancel_goal();
            return 'emergency'
        else:
            if not self.action:
                self.reset_pub.publish()
                self.client.wait_for_server()
                goal=taskGoal()
                

                self.distance=Distance()
                self.number=self.distance.Count()

                goal.Goal.linear.x=1413.6
                goal.Goal.linear.y=843.6*self.number
                goal.Goal.angular.z=0
                self.client.send_goal(goal)
                self.action=True
                return 'phase0'
            
            if not self.result and self.action:
                self.result=self.client.get_result()
                rospy.loginfo(self.result)
                return 'phase0'
            
            if self.result and self.action:
                rospy.loginfo(str(self.number)+':boll is tried')
                self.action=False
                self.result=False
                return 'next'

class Try_Phase1(smach.State):
    def __init__(self):
        
        smach.State.__init__(self,outcomes=['phase1','next'])
        self.ord_sub=rospy.Subscriber('order',Int32,self.ordcb)
        self.try_sub=rospy.Subscriber('success',Bool,self.trycb)
        self.cmd_pub=rospy.Publisher("cmd_vel",Twist)
        self.reset_pub=rospy.Publisher('reset',Empty)
        self.go_pub=rospy.Publisher('Go',Bool)
        self.x=0
        self.y=0
        self.z=0
        self.count=0
        self.success=False
        self.mg=Twist()
        self.auto_pub=rospy.Publisher('manual',Bool)

    def ordcb(self,ms):
        #self.x=mg.linear.x
        #self.y=mg.linear.y
        #self.z=mg.angular.z
        self.count+=ms.data 
    def trycb(self,Msg):
        self.success=Msg.data
    
    def execute(self,data):
        rospy.sleep(0.1)
            
        if self.success:
            self.Mg=True
            self.reset_pub.publish()
            self.auto_pub.publish(self.Mg)
            
            #self.go_pub.publish(self.Mg)
            
            self.success=False
            return 'next'
            
        else:
            rospy.loginfo('phase1 state')
            
            self.Mg=False
            self.auto_pub.publish(self.Mg)
            
            #self.Mg=False
            
            self.auto_pub.publish(self.Mg)
            self.go_pub.publish(self.Mg)
            
            
            return 'phase1'

class Back_Phase0(smach.State):
        
    def __init__(self):
        smach.State.__init__(self,outcomes=['phase0','next','emergency'])
        self.sw_sub=rospy.Subscriber('switch',Bool,self.swcb)
        self.reset_pub=rospy.Publisher('reset',Empty)
        self.client=actionlib.SimpleActionClient('action',taskAction)
        self.go_pub=rospy.Publisher('Go',Bool)
        self.switch=True
        self.action=False
        self.result=False

    def swcb(self,msg):
        self.switch=msg.data
    
    def execute(self,data):
        rospy.sleep(0.5)
        
        if not self.switch:
            self.client.cancel_goal();
            return 'emergency'
        else:
            if not self.action:
                self.reset_pub.publish()
                self.client.wait_for_server()
                goal=taskGoal()

                self.distance=Way_Back()
                self.number=self.distance.Count()
                goal.Goal.linear.x=-1413.6
                goal.Goal.linear.y=843.6*self.number
                goal.Goal.angular.z=0
                self.client.send_goal(goal)
                self.action=True
                return 'phase0'
            
            if not self.result and self.action:
                self.Mg=True

                self.go_pub.publish(self.Mg)
                self.result=self.client.get_result()
                rospy.loginfo(self.result)
                return 'phase0'
            
            if self.result and self.action:
                rospy.loginfo(str(self.number)+':boll is tried')
                
                #self.reset_pub.publish()
                
                self.action=False
                self.result=False
                return 'next'


class Back_Position(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['position','next','emergency'])
        self.sw_sub=rospy.Subscriber('switch',Bool,self.swcb)
        self.reset_pub=rospy.Publisher('reset',Empty)
        self.client=actionlib.SimpleActionClient('action',taskAction)
        self.switch=True
        self.action=False
        self.result=False

        
    def swcb(self,msg):
        self.switch=msg.data
    
    def execute(self,data):
        rospy.sleep(0.5)
        
        if not self.switch:
            self.client.cancel_goal();
            return 'emergency'
        else:
            if not self.action:
                self.reset_pub.publish()
                self.client.wait_for_server()
                goal=taskGoal()

                goal.Goal.linear.x=-2596*0.6
                goal.Goal.linear.y=4827*0.6
                goal.Goal.angular.z=0
                self.client.send_goal(goal)
                self.action=True
                return 'position'
            if not self.result and self.action:
                self.result=self.client.get_result()
                rospy.loginfo(self.result)
                return 'position'
            if self.result and self.action:
                
                #self.reset_pub.publish()
                
                self.action=False
                self.result=False
                return 'next'



class main():
    rospy.init_node('robot_order')
    sm=smach.StateMachine(
        outcomes=['outcomes'])
    with sm:
        smach.StateMachine.add('SWITCH',Switch(),
                                transitions={'manual':'SWITCH',
                                             'auto':'SET'})
        smach.StateMachine.add('SET',Set(),
                                transitions={'init':'START'})
        

        smach.StateMachine.add('START',Start(),
                                transitions={'start':'START',
                                             'next':'WAIT',
                                             'emergency':'SWITCH'})

        smach.StateMachine.add('WAIT',Wait(),
                                transitions={'wait':'WAIT',
                                             'next':'POSITION'})

        smach.StateMachine.add('POSITION',Position(),
                                transitions={'position':'POSITION',
                                             'next':'TRY_PHASE0',
                                             'emergency':'SWITCH'})
        smach.StateMachine.add('TRY_PHASE0',Try_Phase0(),
                                transitions={'phase0':'TRY_PHASE0',
                                             'next':'TRY_PHASE1',
                                             'emergency':'SWITCH'})

        smach.StateMachine.add('TRY_PHASE1',Try_Phase1(),
                                transitions={'phase1':'TRY_PHASE1',
                                             'next':'BACK_PHASE0'})

        smach.StateMachine.add('BACK_PHASE0',Back_Phase0(),
                                transitions={'phase0':'BACK_PHASE0',
                                             'next':'BACK_POSITION',
                                             'emergency':'SWITCH'})

        smach.StateMachine.add('BACK_POSITION',Back_Position(),
                                transitions={'position':'BACK_POSITION',
                                             'next':'WAIT',
                                             'emergency':'SWITCH'})
        sis=smach_ros.IntrospectionServer('server_name',sm,'/SM_ROOT')
        sis.start()

        outcome=sm.execute()
        rospy.spin()


if __name__=='__main__':
    main()

