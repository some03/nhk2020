#!/usr/bin/env python2
from package import read_waypoint as Wp
import smach
import smach_ros
from std_msgs.msg import Float32
from std_msgs.msg import Int32
from std_msgs.msg import Bool
from std_msgs.msg import Empty
import rospy


class File_Number():
    count=0;
    def Count(self):
        File_Number.count+=1
        return File_Number.count

class Emergency(smach.State):

    def __init__(self):
        smach.State.__init__(self,outcomes=['emergency'])
    def execute(self,data):
        pass

class Start(smach.State):
    def __init__(self):
        smach.State.__init__(self,outcomes=['start','emergency','next'])
    
        self.wp=Wp.File_Reader()
        self.emsub=rospy.Subscriber('switch',Bool,self.emCb)
        self.em=True
        self.number=File_Number()
        self.result=False
    def emCb(self,msg):
        self.em=msg.data
     
    def execute(self,data):
        rospy.sleep(0.1)
        return 'start'

        """ 
        if not self.em:
            wp.shutdown()
            return 'emergency'
        else :
            return 'start'
            wp.route0(self.number)
            result=wp.route0(self.number)

            if result:
                return 'next'
            else:
                return 'start'
        """
class main():
    rospy.init_node('waypoint_publisher')
    sm=smach.StateMachine(outcomes=['outcomes']) 
    with sm:
        smach.StateMachine.add('START',Start(),
                                transitions={'emergency':'EMERGENCY',
                                            'start':'START','next':'outcomes'})

        smach.StateMachine.add('EMERGENCY',Emergency(),
                                transitions={'emergency':'EMERGENCY'})
        sis=smach_ros.IntrospectionServer('server_name',sm,'/SM_ROOT')
        sis.start()
        outcomes=sm.execute()
        rospy.spin()

if __name__=='__main__':
    main()



