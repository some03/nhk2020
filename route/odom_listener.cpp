#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<nav_msgs/Odometry.h>
#include<bits/stdc++.h>

class Odom_listener{
    public:
        Odom_listener();
        void save();
        int start;
    private:
        ros::NodeHandle nh;

        void odomCb(const nav_msgs::Odometry& msg);
        ros::Subscriber odom_sub=nh.subscribe("odom",1,&Odom_listener::odomCb,this);
        nav_msgs::Odometry odom;
        std::vector<geometry_msgs::Pose>waypoint;
        geometry_msgs::PoseStamped finish_pose_;
        geometry_msgs::Pose pre_pose;
        std::string filename_; 
        std::string world_frame_;
};

Odom_listener::Odom_listener():filename_("waypoints0.yaml"){
    
    start=0;
    pre_pose.position.x=0.0;
    pre_pose.position.y=0.0;
    pre_pose.position.z=0.0;
    pre_pose.orientation.x=0.0;
    pre_pose.orientation.y=0.0;
    pre_pose.orientation.z=0.0;
    pre_pose.orientation.w=0.0;
}

void Odom_listener::odomCb(const nav_msgs::Odometry &msg){
    
    double  distance=pow(msg.pose.pose.position.x-pre_pose.position.x,2.0)+pow(msg.pose.pose.position.y-pre_pose.position.y,2.0)+pow(msg.pose.pose.position.z-pre_pose.position.z,2.0);
    
    if(distance>=0.5){
        if(start==0)start=1;
        waypoint.push_back(msg.pose.pose);

        ROS_INFO_STREAM("save current position");

        pre_pose.position.x=msg.pose.pose.position.x;
        pre_pose.position.y=msg.pose.pose.position.y;
        pre_pose.position.z=msg.pose.pose.position.z;
        pre_pose.orientation.x=msg.pose.pose.orientation.x;
        pre_pose.orientation.y=msg.pose.pose.orientation.y;
        pre_pose.orientation.z=msg.pose.pose.orientation.z;
        pre_pose.orientation.w=msg.pose.pose.orientation.w;
        
    }    

    finish_pose_.header.seq=0;
    finish_pose_.header.frame_id=world_frame_;
    finish_pose_.pose.position.x=msg.pose.pose.position.x;
    finish_pose_.pose.position.y=msg.pose.pose.position.y;
    finish_pose_.pose.position.z=msg.pose.pose.position.z;

    finish_pose_.pose.orientation.x=finish_pose_.pose.orientation.x;
    finish_pose_.pose.orientation.y=finish_pose_.pose.orientation.y;
    finish_pose_.pose.orientation.z=finish_pose_.pose.orientation.z;
    finish_pose_.pose.orientation.w=finish_pose_.pose.orientation.w;
}


void Odom_listener::save(){
    //-----------waypoints.yamlにかきだし----------------//
    std::ofstream ofs("waypoints1.yaml");
    ofs<<"waypoints:"<<std::endl;

    for(int i=0;i<waypoint.size();i++){
       ofs<<"  "<<"- position:"<<std::endl;        
       ofs<<"      x: "<<waypoint[i].position.x<<std::endl;
       ofs<<"      y: "<<waypoint[i].position.y<<std::endl;
       ofs<<"      z: "<<waypoint[i].position.z<<std::endl;
       ofs<<"      qx: "<<waypoint[i].orientation.x<<std::endl;
       ofs<<"      qy: "<<waypoint[i].orientation.y<<std::endl;
       ofs<<"      qz: "<<waypoint[i].orientation.z<<std::endl;
       ofs<<"      qw: "<<waypoint[i].orientation.w<<std::endl;

    }

    ofs<<"finish_pose:"<<std::endl;
    ofs<<"  header: "<<std::endl;
    ofs<<"    seq: "<<finish_pose_.header.seq<<std::endl;
    ofs<<"    stamp: "<<finish_pose_.header.stamp<<std::endl;
    ofs<<"    frame_id: "<<finish_pose_.header.frame_id<<std::endl;
    ofs<<"  pose: "<<std::endl;
    ofs<<"    position: "<<std::endl;
    ofs<<"      x: "<<finish_pose_.pose.position.x<<std::endl;
    ofs<<"      y: "<<finish_pose_.pose.position.y<<std::endl;
    ofs<<"      z: "<<finish_pose_.pose.position.z<<std::endl;
    ofs<<"    orientation: "<<std::endl;
    ofs<<"      x: "<<finish_pose_.pose.orientation.x<<std::endl;
    ofs<<"      y: "<<finish_pose_.pose.orientation.y<<std::endl;
    ofs<<"      z: "<<finish_pose_.pose.orientation.z<<std::endl;
    ofs<<"      w: "<<finish_pose_.pose.orientation.w<<std::endl;

    ofs.close();
    std::cout<<"write success"<<std::endl;
        
    
}


int main(int argc,char**argv){
    
    ros::init(argc,argv,"odom_listener");
    Odom_listener odom_listener;
    ros::Rate loop_rate(2);

    while(ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
    }
    if(odom_listener.start!=0){
        ROS_INFO_STREAM("start write");
        odom_listener.save();

    }
    return 0;
}


