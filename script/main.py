#!/usr/bin/env python2
from package import read_waypoint as Wp
import smach
import smach_ros
from std_msgs.msg import Float32
from std_msgs.msg import Int32
from std_msgs.msg import Bool
from std_msgs.msg import Empty
from subprocess import *
import rospy


class File_Number():
    count=-1;
    def Count(self):
        File_Number.count+=1
        return File_Number.count



def emergency(msg):
    rospy.loginfo("a")
    kill_node(local_planner)
    kill_node(main.py)
emsub=rospy.Subscriber('switch',Bool,emergency)


class Start(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['start','next'])
    
        self.wp=Wp.File_Reader()
        self.file=File_Number()
        self.number=self.file.Count()
        self.result=False
        self.next=False
    
     
    def execute(self,data):
        rospy.sleep(0.5)
        #result=self.wp.route0(self.number)
        rospy.loginfo(self.number)
        return 'next'

        
class main():
    rospy.init_node('waypoint_publisher')
    sm=smach.StateMachine(outcomes=['outcomes']) 
    with sm:
        smach.StateMachine.add('START',Start(),
                                transitions={'start':'START','next':'outcomes'})

        sis=smach_ros.IntrospectionServer('server_name',sm,'/SM_ROOT')
        sis.start()
        outcomes=sm.execute()
        rospy.spin()

if __name__=='__main__':
    main()
    """
    f=Wp.File_Reader()
    b=File_Number()
    num=b.Count()
    f.reading0(num)
    print(f.reading0(num))
    """


