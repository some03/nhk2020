#!/usr/bin/env python2
import yaml
import sys
from pathlib2 import Path
import rospy
from nav_msgs.msg import Odometry
import math
import tf2_ros
from geometry_msgs.msg import PoseStamped
#[""waipoint"][num][position][x/y/z]
#[finish_pose][header/position/orientation][value]
sys.path.append(str(Path('__file__').resolve().parent.parent))
data_raw='../../route/'

class File_Reader:
    y={}
    def reading(self,num):
        
        with open(data_raw+'waypoints%d.yaml'%(num)) as File:
            self.y=yaml.safe_load(File.read())
            rospy.loginfo("file loaded")
        return self.y
     
    def assign(self,num):

        pose=PoseStamped()

        y=self.reading(num)
        wp=y['waypoints']

        for i in range(len(wp)): 
            #print(i)
            pose.pose.position.x=wp[i]['position']['x']        
            pose.pose.position.y=wp[i]['position']['y']        
            pose.pose.position.z=wp[i]['position']['z']        
            pose.pose.orientation.x=wp[i]['position']['qx']
            pose.pose.orientation.y=wp[i]['position']['qy']
            pose.pose.orientation.z=wp[i]['position']['qz']
            pose.pose.orientation.w=wp[i]['position']['qw']
            #print(pose)
        return pose

if __name__=="__main__"
    pass


