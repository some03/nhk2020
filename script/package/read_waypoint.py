#!/usr/bin/env python2
import yaml
import sys
from pathlib2 import Path
import rospy
from nav_msgs.msg import Odometry
import math
import tf
from geometry_msgs.msg import PoseStamped
from geometry_msgs.msg import Pose2D
from nav_msgs.msg import Odometry
from move_base_msgs.msg import MoveBaseAction,MoveBaseGoal
from geometry_msgs.msg import Twist

#[""waipoint"][num][position][x/y/z]
#[finish_pose][header/position/orientation][value]
sys.path.append(str(Path('__file__').resolve().parent.parent))
data_raw='../route/'


class File_Reader:
    def __init__(self):
       
        self.listener=tf.TransformListener() 
        self.odom_sub=rospy.Subscriber('odom',Odometry,self.odomcb)
        #self.pose.target_pose.header.frame_id='map'
        #self.listener.waitForTransform("map", "base_link", rospy.Time(), rospy.Duration(4.0))
        self.distance=0 
        self.distance2=0 
        self.goal_pub=rospy.Publisher("goal",PoseStamped)
        self.X=0
        self.Y=0
        self.Z=0
        self.reach=False
        self.pose=PoseStamped()
    def reading0(self,num):
        with open(data_raw+'waypoints%d.yaml'%(num)) as File:
            self.y=yaml.safe_load(File.read())
            #rospy.loginfo("file loaded")
        return self.y
     
    def reading1(self,num):
        with open(data_raw+'switch%d.yaml'%(num)) as File:
            self.y=yaml.safe_load(File.read())
            #rospy.loginfo("file loaded")
        return self.y
    def odomcb(self,mg):
        self.X=mg.pose.pose.position.x
        self.Y=mg.pose.pose.position.y
        self.Z=mg.pose.pose.position.z
        
    def route0(self,num):
        #pose=PoseStamped()
        y=self.reading0(num)
        wp=y['waypoints']
        for i in range(len(wp)): 
            #print(i)
            self.pose.pose.position.x=wp[i]['position']['x']        
            self.pose.pose.position.y=wp[i]['position']['y']        
            self.pose.pose.position.z=wp[i]['position']['z']        
            self.goal_pub.publish(self.pose)
            while True:
                now=rospy.Time.now() 
                self.distance=abs((math.hypot(self.pose.pose.position.x,self.pose.pose.position.y))-(math.hypot(self.X,self.Y)))
                
                if(self.pose.pose.position.x==0 and self.pose.pose.position.y==0):
                    self.distance=0
                if(self.distance<=30):
                    rospy.loginfo(self.distance)
                    rospy.loginfo("next!")
                    break
                else:
                    rospy.sleep(0.5)
                    rospy.loginfo(self.distance)
        return True
if __name__=="__main__":
    pass
