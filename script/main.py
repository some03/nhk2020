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

#when ps3 button is pushed, kill manual node--------------------
def kill_node(nodename):
    p2=Popen(['rosnode','list'],stdout=PIPE)
    p2.wait()
    nodelist=p2.communicate()
    nd=nodelist[0]
    nd=nd.split("\n")
    for i in range(len(nd)):
        tmp=nd[i]
        ind=tmp.find(nodename)
        if ind==1:
            call(['rosnode','kill',nd[i]])
            break
def emergency(msg):
    kill_node('pub_cmd')
    call(['rosrun','nhk_2020','ps3'])

def pc_to_ps3(msg):
    kill_node('pub_cmd')
    call(['rosrun','nhk_2020','ps3'])
def ps3_to_pc(msg):
    kill_node('ps3')
    call(['rosrun','nhk_2020','pub_cmd'])
em_sub=rospy.Subscriber('emergency',Bool,emergency)
#--------------------------------------------------------------

class File_Number():
    count=-1;
    def Count(self):
        File_Number.count+=1
        return File_Number.count

class Pre_start(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['yet','go'])
        self.reset_pub=rospy.Publisher('reset',Empty)
        self.sub=rospy.Subscriber('start',Bool,self.subCb) 
        self.start=False
    def subCb(self,msg):
        self.start=msg.data
    def execute(self,data):
        if self.start:
            self.reset_pub.publish()
            return 'go'
        else:
            rospy.sleep(0.5)
            return 'yet'

class Start(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['start','next'])
        self.wp=Wp.File_Reader()
        self.file=File_Number()
        self.number=self.file.Count()


    def execute(self,data):
        rospy.loginfo(self.number)
        self.wp.route0(self.number)
        return 'next'

class Catch(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['catch','next'])
        pc_to_ps3()
        self.sub=rospy.Subscriber('catch',Bool,self.subCb)
        self.reset_pub=rospy.publish('reset',Empty)

    def subCb(self,msg):
        self.result=msg.data
    
    def execute(self,data):
        if  self.result:
            rospy.loginfo(self.number)
            self.reset_pub.publish()
            self.result=False
            return 'next'
        else:
            rospy.sleep(0.5)
            return 'catch'

class Go(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['next'])
        self.wp=Wp.File_Reader()
        self.file=File_Number()
        self.number=self.file.Count()
    def execute(self,data):
        ps3_to_pc()
        self.wp.route0(self.number)
        return 'next'

class Try(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['next'])
        self.launch_pub=rospy.Publisher("try",Int32)

    def execute(self,data):
        rospy.sleep(3)
        self.launch_pub.publish(1)
        rospy.sleep(5)
        self.launch_pub.publish(2)
        rospy.sleep(5)
        self.launch_pub.publish(3)
        rospy.sleep(2)
        self.launch_pub.publish(0)
        return 'next'


class Back(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['back','next'])
        self.wp=Wp.File_Reader()
        self.file=File_Number()
        self.number=self.file.Count()

    def execute(self,data):
        if not self.result:
            rospy.loginfo(self.number)
            self.wp.route0(self.number)
            return 'next'
    
class main():
    rospy.init_node('waypoint_publisher')
    sm=smach.StateMachine(outcomes=['outcomes']) 
    with sm:
    
        smach.StateMachine.add('PRE_START',Pre_start(),
                            transitions={'yet':'PRE_START','go':'START'})
        smach.StateMachine.add('START',Start(),
                            transitions={'start':'START','next':'CATCH'})

        smach.StateMachine.add('CATCH',Catch(),
                            transitions={'catch':'CATCH','next':'GO'})
        smach.StateMachine.add('GO',Go(),
                            transitions={'next':'TRY'})
    
        smach.StateMachine.add('TRY',Try(),
                            transitions={'next':'BACK'})
        
        smach.StateMachine.add('BACK',Back(),
                            transitions={'back':'BACK','next':'TRY'})
        sis=smach_ros.IntrospectionServer('server_name',sm,'/SM_ROOT')
        sis.start()
        outcomes=sm.execute()
        rospy.spin()

if __name__=='__main__':
    main()


