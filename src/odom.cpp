#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<nav_msgs/Odometry.h>
#include<sensor_msgs/Imu.h>
#include<tf/transform_broadcaster.h>
#include<Odometry_compute.h>
#include<std_msgs/Int32.h>
#define R 50
#define L 30

double enc[4];

double roll,pitch,yaw;

OdomCompute odc("THREE",R);
ros::NodeHandle nh;

void m0Cb(const std_msgs::Int32::ConstPtr& msg){
		enc[0]=msg->data;
}
void m1Cb(const std_msgs::Int32::ConstPtr& msg){
		enc[1]=msg->data;
}
void m2Cb(const std_msgs::Int32::ConstPtr& msg){
		enc[2]=msg->data;
}
void m3Cb(const std_msgs::Int32::ConstPtr& msg){
		enc[3]=msg->data;
}


void imuCb(const sensor_msgs::Imu&  msg){
   
        geometry_msgs::Quaternion Q;
        
        Q.x=msg.orientation.x;
        Q.y=msg.orientation.y;
        Q.z=msg.orientation.z;
        Q.w=msg.orientation.w;
//-----geometry_msgs::Quaternion-->tf2::Quaternion-----------//
        tf::Quaternion quat(Q.x,Q.y,Q.z,Q.w);
        tf::Matrix3x3(quat).getRPY(roll,pitch,yaw);
//----------------------------------------------------------//
    }


void publish(){


    ros::Publisher odom_pub=nh.advertise<nav_msgs::Odometry>("odom",10);
//-------------------------------------------------// 
    ros::Time current_time=ros::Time::now();
    geometry_msgs::Pose position;
    nav_msgs::Odometry odom_msg;
//-------------------------------------------------//
    position=odc.Updata_Pose(enc,yaw,1800);
//-------------------------------------------------//
    odom_msg.header.stamp=current_time;
    odom_msg.header.frame_id="odom";
    odom_msg.pose.pose=position;
    odom_pub.publish(odom_msg);
   
    ROS_INFO("position: %f",position);
}

int main(int argc,char**argv){
    ros::init(argc,argv,"odometry_publisher");
    ros::Rate loop_rate(10);
//---------------------------------------------------------------------------//    
    ros::Subscriber odom0=nh.subscribe("enc0",10,m0Cb);
    ros::Subscriber odom1=nh.subscribe("enc1",10,m1Cb);
    ros::Subscriber odom2=nh.subscribe("enc2",10,m2Cb);
    ros::Subscriber odom3=nh.subscribe("enc3",10,m3Cb);
    ros::Subscriber imu_sub=nh.subscribe("imu/data",10,imuCb);
//---------------------------------------------------------------------------// 
    while(ros::ok()){
        publish();
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}



