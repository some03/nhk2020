#include<ros/ros.h>
#include<bits/stdc++.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<std_msgs/Float32.h>
#include<std_msgs/Float64.h>
#include<std_msgs/Int32.h>
#include<std_msgs/Bool.h>
#include<tf2/LinearMath/Quaternion.h>
#include<tf2/LinearMath/Matrix3x3.h>
#include<tf2_geometry_msgs/tf2_geometry_msgs.h>
#include<tf2_ros/transform_broadcaster.h>
#include<nav_msgs/Odometry.h>
#include<sensor_msgs/Imu.h>
#include<tf/transform_broadcaster.h>

#define PI  3.14159265359 
#define GR  1.63636363636
#define PULSE 7100.0
#define R 0.05 

class Odom{
    public:
        Odom();
        void publish();
    private:
        ros::NodeHandle nh;
          
        void m0(const std_msgs::Int32::ConstPtr& msg);
        void m1(const std_msgs::Int32::ConstPtr& msg);
        void m2(const std_msgs::Int32::ConstPtr& msg);
        void m3(const std_msgs::Int32::ConstPtr& msg);
        void imuCb(const sensor_msgs::Imu& msg);

        ros::Subscriber e0sub=nh.subscribe<std_msgs::Int32>("enc0",10,&Odom::m0,this);
        ros::Subscriber e1sub=nh.subscribe<std_msgs::Int32>("enc1",10,&Odom::m1,this);
        ros::Subscriber e2sub=nh.subscribe<std_msgs::Int32>("enc2",10,&Odom::m2,this);
        ros::Subscriber e3sub=nh.subscribe<std_msgs::Int32>("enc3",10,&Odom::m3,this);
        ros::Subscriber imu_sub=nh.subscribe("imu/data",10,&Odom::imuCb,this); 

	
        ros::Publisher odom_pub=nh.advertise<nav_msgs::Odometry>("odom",10); 
       
        tf::TransformBroadcaster odom_broadcaster;
        geometry_msgs::PoseStamped ps_msg;
        ros::Time current_time,last_time;
        geometry_msgs::Quaternion odom_quat; 
        long long enc0;
        long long enc1;
        long long enc2;
        long long enc3;

        long long old_enc0;
        long long old_enc1;
        long long old_enc2;
        long long old_enc3;
        
        double x,y,th;
        double vx,vy,vth;
        double qx,qy,qz,qw;
        double roll,pitch,yaw;
        int v0,v1,v2,v3;
    
        double dt;
};


Odom::Odom(){
    
    //current_time=ros::Time::now();
    last_time=ros::Time::now();
    
    
    old_enc0=0.0;
    old_enc1=0.0;
    old_enc2=0.0;
    old_enc3=0.0;

    enc0=0.0;
    enc1=0.0;
    enc2=0.0;
    enc3=0.0;
    
    x=0; 
    y=0;
    th=0;
    
    vx=0;
    vy=0;
    vth=0;

    dt=0;

    yaw=0;
}

void Odom::m0(const std_msgs::Int32::ConstPtr& msg){
		enc0=msg->data;
}
void Odom::m1(const std_msgs::Int32::ConstPtr& msg){
		enc1=msg->data;
}
void Odom::m2(const std_msgs::Int32::ConstPtr& msg){
		enc2=msg->data;
}

void Odom::m3(const std_msgs::Int32::ConstPtr& msg){
		enc3=msg->data;
}


void Odom::imuCb(const sensor_msgs::Imu&  msg){
        qx=msg.orientation.x;
        qy=msg.orientation.y;
        qz=msg.orientation.z;
        qw=msg.orientation.w;

        
        //ps_msg.pose.orientation=msg.orientation;
//-----geometry_msgs::Quaternion-->tf2::Quaternion-----------//
        tf2::Quaternion quat(qx,qy,qz,qw);
        tf2::Matrix3x3(quat).getRPY(roll,pitch,yaw);
       
        odom_quat=tf::createQuaternionMsgFromYaw(yaw);
//----------------------------------------------------------//
    }


void Odom::publish(){
    current_time=ros::Time::now();

//--各オムニホイールの角速度----------------------------------//
    enc0-=7100;
    enc1+=7100;
    enc2+=7100;
    enc3-=7100;

    v0=((enc0*2*PI*R)/(PULSE*dt*GR));
    v1=((enc1*2*PI*R)/(PULSE*dt*GR));
    v2=((enc2*2*PI*R)/(PULSE*dt*GR));
    v3=((enc3*2*PI*R)/(PULSE*dt*GR));
//----------------------------------------------------------//

//------オドメトリの計算------------------------------------//
   double A=0.35*(-v0+v1+v2-v3);
   double B=0.35*(v0+v1-v2-v3);

   
   
   th=yaw;
   
   vx=A*cos(th)-B*sin(th);
   vy=A*sin(th)+B*cos(th);
   vth=th;

    x+=vx;
    y+=vy;
    
    dt+=0.2;
   //なんで？

   
   /*****デバッグ**********/
   ROS_INFO("vx: %lf",vx);
   ROS_INFO("vy: %lf",vy);
   ROS_INFO("vth: %lf",vth);

   ROS_INFO("X: %lf",x);
   ROS_INFO("Y: %lf",y);
   
   /***********************/

//---------------------------------------------------------//
//===----tf--------------------------------=====//
    geometry_msgs::TransformStamped odom_trans; 
    odom_trans.header.stamp=current_time;
    odom_trans.header.frame_id="odom";
    odom_trans.child_frame_id="base_link";

    odom_trans.transform.translation.x=x;//0x;
    odom_trans.transform.translation.y=y;//y;
    odom_trans.transform.translation.z=0.0;
    odom_trans.transform.rotation=odom_quat;

    odom_broadcaster.sendTransform(odom_trans);
    

    nav_msgs::Odometry odom;
    odom.header.stamp=current_time;
    odom.header.frame_id="odom";
//=====-----------------------------========//
//-------set position----------------//
    odom.pose.pose.position.x=x;
    odom.pose.pose.position.y=y;
    odom.pose.pose.position.z=0.0;
    odom.pose.pose.orientation=odom_quat;

//-----------------------------------//
//--------set velocity----------------//
    odom.child_frame_id="base_link";
    odom.twist.twist.linear.x=vx;
    odom.twist.twist.linear.y=vy;
    odom.twist.twist.angular.z=vth;

    odom_pub.publish(odom);

}

int main(int argc,char**argv){
    ros::init(argc,argv,"odometry_publisher");
    Odom odom;
    ros::Rate loop_rate(5);
    while(ros::ok()){
        odom.publish();
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}

