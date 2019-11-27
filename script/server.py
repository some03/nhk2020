#!/usr/bin/env python
import rospy
import actionlib
from nhk_2020.msg import taskAction
from nhk_2020.msg import taskFeedback
from nhk_2020.msg import taskResult

class DoDishesServer:

    def __init__(self):

        self.server = actionlib.SimpleActionServer('do_dishes', taskAction, self.execute, False)

        self.server.start()
        rospy.loginfo('server start')

    def execute(self, goal):

    # Do lots of awesome groundbreaking robot stuff here

        self.server.set_succeeded()

if __name__ == '__main__':

    rospy.init_node('do_dishes_server')

    server = DoDishesServer()

    rospy.spin()


