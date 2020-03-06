#!/usr/bin/env python2
import yaml
import sys
from pathlib2 import Path
import rospy
from nav_msgs.msg import Odometry
import math
import tf2_ros
from geometry_msgs.msg import PoseStamped
import actionlib
from move_base_msgs.msg import MoveBaseAction,MoveBaseGoal


#[""waipoint"][num][position][x/y/z]
#[finish_pose][header/position/orientation][value]
sys.path.append(str(Path('__file__').resolve().parent.parent))
data_raw='../route/'

class File_Reader:
    def __init__(self):
        self.tfBuffer=tf2_ros.Buffer()
        self.listener=tf2_ros.TransformListener(self.tfBuffer)
        self.client=actionlib.SimpleActionClient('move_base',MoveBaseAction)
        self.client.wait_for_server()

        pose=MoveBaseGoal() 
        pose.target_pose.header.frame_id='map'
        self.tfBuffer.lookup_transform('odom','base_link',rospy.Time(),rospy.Duration(4.0))



    def reading0(self,num):
        with open(data_raw+'waypoints%d.yaml'%(num)) as File:
            self.y=yaml.safe_load(File.read())
            rospy.loginfo("file loaded")
        return self.y
     
    def reading1(self,num):
        with open(data_raw+'switch%d.yaml'%(num)) as File:
            self.y=yaml.safe_load(File.read())
            rospy.loginfo("file loaded")
        return self.y
    
    def route0(self,num):

        #pose=PoseStamped()

        y=self.reading0(num)
        wp=y['waypoints']

        for i in range(len(wp)): 
            #print(i)
            self.pose.target_pose.position.x=wp[i]['position']['x']        
            self.pose.target_pose.position.y=wp[i]['position']['y']        
            self.pose.target_pose.position.z=wp[i]['position']['z']        
            self.pose.target_pose.orientation.x=wp[i]['position']['qx']
            self.pose.target_pose.orientation.y=wp[i]['position']['qy']
            self.pose.target_pose.orientation.z=wp[i]['position']['qz']
            self.pose.target_pose.orientation.w=wp[i]['position']['qw']
            #print(pose)
            self.client.send_goal(self.pose)

            while True:
                now=rospy.Time.now() 
                self.tfBuffer.lookup_transform('odom','base_link',now,rospy.Duration(4.0))
                position,quaternion=self.listener.lookupTrandform("map","base_link",now)
                distance=math.sqrt(((position[0]-pose.target_pose.pose.position.x)**2+(position[1]-pose.target_pose.pose.position.y)**2))
                if(distace<=1):
                    ropy.loginfo("next!")
                    break
    
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
