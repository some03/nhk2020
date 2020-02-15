#include<ros/ros.h>
#include<bits/stdc++.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Float32.h>
#include<std_msgs/Float64.h>
#include<std_msgs/Int32.h>
#include<std_msgs/Bool.h>
#include<tf2/LinearMath/Quaternion.h>
#include<tf2_ros/transform_broadcaster.h>
#include<nav_msgs/Odmetry.h>

class Odm{
    public:
        Odm();
        void publish();
    private:
        ros::NodeHandle nh;
          
        void m0(const std_msgs::Int32::ConstPtr& msg);
		void m1(const std_msgs::Int32::ConstPtr& msg);
		void m2(const std_msgs::Int32::ConstPtr& msg);
		void m3(const std_msgs::Int32::ConstPtr& msg);


        ros::Subscriber e0sub=nh.subscribe<std_msgs::Int32>("enc0",10,&Run::m0,this);
		ros::Subscriber e1sub=nh.subscribe<std_msgs::Int32>("enc1",10,&Run::m1,this);
		ros::Subscriber e2sub=nh.subscribe<std_msgs::Int32>("enc2",10,&Run::m2,this);
		ros::Subscriber e3sub=nh.subscribe<std_msgs::Int32>("enc3",10,&Run::m3,this);
		
        long long enc0;
		long long enc1;
		long long enc2;
		long long enc3;
}

Odm::Odm(){
    
}

void Odm::m0(const std_msgs::Int32::ConstPtr& msg){
		enc0=msg->data;
}
void Odm::m1(const std_msgs::Int32::ConstPtr& msg){
		enc1=msg->data;
}
void Odm::m2(const std_msgs::Int32::ConstPtr& msg){
		enc2=msg->data;
}

void Odm::m3(const std_msgs::Int32::ConstPtr& msg){
		enc3=msg->data;
}


void Odm::imuCb(const geometry_msgs::Twist::ConstPtr &imuMsg){
        	rad=imuMsg->angular.z;
}
