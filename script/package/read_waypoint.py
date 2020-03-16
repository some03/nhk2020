#!/usr/bin/env python2
import yaml
import sys
from pathlib2 import Path
import rospy
from nav_msgs.msg import Odometry
import math
import tf2_ros
import tf
from geometry_msgs.msg import PoseStamped
from nav_msgs.msg import Odometry
import actionlib
from move_base_msgs.msg import MoveBaseAction,MoveBaseGoal
from geometry_msgs.msg import Twist

#[""waipoint"][num][position][x/y/z]
#[finish_pose][header/position/orientation][value]
sys.path.append(str(Path('__file__').resolve().parent.parent))
data_raw='../Route/'

class File_Reader:
    def __init__(self):
       
        self.listener=tf.TransformListener() 
        self.odom_sub=rospy.Subscriber('odom',Odometry,self.odomcb)
        #self.pose.target_pose.header.frame_id='map'

        #self.listener.waitForTransform("map", "base_link", rospy.Time(), rospy.Duration(4.0))
        self.distance=0 
        self.distance2=0 
        self.cmd_pub=rospy.Publisher("goal",PoseStamped)
        self.X=0
        self.Y=0
        self.Z=0
        self.mg=PoseStamped()


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
            self.mg.pose.position.x=wp[i]['position']['x']        
            self.mg.pose.position.y=wp[i]['position']['y']        
            self.mg.pose.position.z=wp[i]['position']['z']        
            rospy.loginfo(i)
            self.cmd_pub.publish(self.mg)
            """  
            self.pose.target_pose.pose.orientation.x=wp[i]['position']['qx']
            self.pose.target_pose.pose.orientation.y=wp[i]['position']['qy']
            self.pose.target_pose.pose.orientation.z=wp[i]['position']['qz']
            self.pose.target_pose.pose.orientation.w=wp[i]['position']['qw']
            self.pose.target_pose.header.stamp=rospy.Time.now()
            self.client.send_goal(self.pose)
            rospy.loginfo(i)
            self.client.wait_for_result(rospy.Duration(50));
            """
             
            while True:
                now=rospy.Time.now() 
                if self.mg.pose.position.x<=1:
                    self.mg.pose.position.x=0
                
                if self.mg.pose.position.y<=1:
                    self.mg.pose.position.y=0
                self.distance=abs(self.mg.pose.position.y-self.Y)

                if(self.distance<=0.5):
                    rospy.loginfo(self.distance)
                    rospy.loginfo("next!")
                    break
                else:
                    rospy.sleep(0.3)
                    rospy.loginfo(self.distance)
             
        return True;
        rospy.sleep(2)
        
    def route1(self,num):

        pose=PoseStamped()

        waypoints=self.reading0(num)
        stoppoint=self.reading1(num)
        wp=waypoints['waypoints']
        sp=stoppoint[0]

        for i in range(sp): 
            #print(sp)
            pose.pose.position.x=wp[i]['position']['x']        
            pose.pose.position.y=wp[i]['position']['y']        
            pose.pose.position.z=wp[i]['position']['z']        
            pose.pose.orientation.x=wp[i]['position']['qx']
            pose.pose.orientation.y=wp[i]['position']['qy']
            pose.pose.orientation.z=wp[i]['position']['qz']
            pose.pose.orientation.w=wp[i]['position']['qw']
            #print(pose)

    def route2(self,num):

        pose=PoseStamped()

        waypoints=self.reading0(num)
        stoppoint=self.reading1(num)
        wp=waypoints['waypoints']
        sp=stoppoint[0]

        for i in range(sp,len(waypoints)+1): 
            #print(len(waypoints))
            pose.pose.position.x=wp[i]['position']['x']        
            pose.pose.position.y=wp[i]['position']['y']        
            pose.pose.position.z=wp[i]['position']['z']        
            pose.pose.orientation.x=wp[i]['position']['qx']
            pose.pose.orientation.y=wp[i]['position']['qy']
            pose.pose.orientation.z=wp[i]['position']['qz']
            pose.pose.orientation.w=wp[i]['position']['qw']
            #print(pose)

if __name__=="__main__":
    pass
