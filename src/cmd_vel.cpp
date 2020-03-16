#include<ros/ros.h>
#include<actionlib/server/simple_action_server.h>
#include<nhk_2020/taskAction.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<std_msgs/Bool.h>
#include<std_msgs/Float64.h>
#include<bits/stdc++.h>
#include<tf2/LinearMath/Quaternion.h>
#include<tf2/LinearMath/Matrix3x3.h>
#include<tf2_geometry_msgs/tf2_geometry_msgs.h>
#include<tf2_ros/transform_broadcaster.h>
#include<nav_msgs/Odometry.h>
#include<sensor_msgs/Imu.h>
#include<tf/transform_broadcaster.h>
#include<tf/transform_listener.h>
#include<std_msgs/Bool.h>
class Cmd{
    public:
        Cmd();
        void publish();
        bool success=false;
    
    private:
        ros::NodeHandle nh;

        void goalCb(const geometry_msgs::PoseStamped&  msg);
        ros::Subscriber goal_sub=nh.subscribe("goal",10,&Cmd::goalCb,this);

        void publish(const nav_msgs::Odometry&  msg);
        ros::Subscriber sub=nh.subscribe("odom",10,&Cmd::publish,this);
        ros::Publisher ord_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",10); 
        ros::Publisher result=nh.advertise<std_msgs::Bool>("result",10); 
    
        tf::TransformListener ln;
        double x;
        double y;
        double z;
        double nowx=0;
        double nowy=0;
        double nowz=0;

        double count=0;
        double  mxspd=70;
        

};
Cmd::Cmd(){
    success=false;
}

void Cmd::goalCb(const geometry_msgs::PoseStamped& msg){
        
        geometry_msgs::Twist mg;
        x=msg.pose.position.x;
		y=msg.pose.position.y;
		z=msg.pose.position.z;
        //if((x>0&&x<1)||(x<&&x>-5))x=0;
        //if(y<0)y=0;
        
        mg.linear.x=50*(x-(nowx*0.1))/count;
        mg.linear.y=70*(y-nowy)/count;
        mg.angular.z=70*(z-nowz)/count;
       
        std::cout<<x<<std::endl;
        std::cout<<nowx<<std::endl;
        /*
        double distance=sqrt(pow((x-nowx),2)+(pow((y-nowy),2)));
        if(distance<0.1){
            mg.linear.x=0;
            mg.linear.y=0;
            mg.angular.z=0;
        }*/
        if(mg.linear.x>=0)mg.linear.x=std::min(mg.linear.x,mxspd);
       	else mg.linear.x=std::max(mg.linear.x,-mxspd);


        if(mg.linear.y>=0)mg.linear.y=std::min(mg.linear.y,mxspd);
        else mg.linear.y=std::max(mg.linear.y,-mxspd);

        if(mg.linear.z>=0)mg.angular.z=std::min(mg.angular.z,mxspd);
        else mg.angular.z=std::max(mg.angular.z,-mxspd);



        ord_pub.publish(mg);
        count+=0.1;

	    
      
        /*catch(...){
            ROS_INFO("tf error");
        }*/
        
        
/*
        double distance=sqrt(pow(x-target_pose.getOrigin().x(),2)+(pow(target_pose.getOrigin().y(),2)));
        if(distance<1){
            std_msgs::Bool msg;
            msg.data=true;
            result.publish(msg);

        }
        */


}

void Cmd::publish(const nav_msgs::Odometry& msg){
        
        nowx=msg.pose.pose.position.x;
        nowy=msg.pose.pose.position.y;
        nowz=msg.pose.pose.position.z;


}



int main(int argc,char**argv){
		ros::init(argc,argv,"pub_cmd");
		Cmd cmd;
        ros::Rate loop_rate(10);
		while(ros::ok()){
			ros::spinOnce();
			loop_rate.sleep();
		}
		return 0;
}



