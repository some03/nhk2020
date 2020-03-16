#include<ros/ros.h>
#include<actionlib/server/simple_action_server.h>
#include<nhk_2020/taskAction.h>
#include<geometry_msgs/Twist.h>
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
class taskAction{
	private:
		ros::NodeHandle nh;

		actionlib::SimpleActionServer<nhk_2020::taskAction> Task;
		std::string order_action;

		nhk_2020::taskFeedback feedback_;
		nhk_2020::taskResult result_;
   		ros::Publisher pub=nh.advertise<geometry_msgs::Twist>("cmd_ord",10);
        //---
        ros::Publisher kp_pub=nh.advertise<std_msgs::Float64>("accel",10);
        //---
		void posCb(const geometry_msgs::Twist::ConstPtr&mg);
 		ros::Subscriber pSb=nh.subscribe<geometry_msgs::Twist>("cmd_pos",10,&taskAction::posCb,this);


        //-----tf----------------------------------------//
        tf::TransformListener ln;

	public:
		taskAction(std::string name):
				Task(nh,name,boost::bind(&taskAction::exeCb,this,_1),false),
				order_action(name){
		 			Task.start();
					std::cout<<"start"<<std::endl;
		 		}
		~taskAction(void){}
		void exeCb(const nhk_2020::taskGoalConstPtr &goal);
		double nowx=1,nowy=1,nowz=1;
		double x=0,y=0,z=0;
		double Dx=0,Dy=0,Dz=0;
		int limit=650;//450;//1700;
       	double ac=-1;

};


void taskAction::exeCb(const nhk_2020::taskGoalConstPtr &goal){
		ros::Rate loop_rate(5);
        bool success=true;
        
        x=goal->Goal.linear.x;
		y=goal->Goal.linear.y;
		z=goal->Goal.angular.z;	
	   //-----listening odom----------------------- 
        geometry_msgs::PoseStamped source_pose;
        source.header.frame_id="odom";
        source.header.stamp=ros::Time::now()
	    
        geometry_msgs::PoseStamped target_pose;
        std::string target_frame="base_link";
        try{
            ln.waitForTransform(source_pose.header.frame_id, target_frame, ros::Time::now(), ros::Duration(1.0));
            ln.transformPose(target_frame, source_pose, target_pose);

      ROS_INFO("x:%+5.2f, y:%+5.2f,z:%+5.2f",target.pose.position.x,target.pose.position.y,target.pose.position.z);
        }
        catch(...){
            ROS_INFO("tf error");
        }
        //-----------------------------------------------------
        




	while(Dx>limit || Dy>limit||Dx<-limit||Dy<-limit){

    	if(Task.isPreemptRequested()||!ros::ok()){
		   	Task.setPreempted();
	   	   	success=false;
		   	break;
		}

		loop_rate.sleep();
		std::cout<<"execute"<<std::endl;
	}

	if(success){
		
			result_.reached=1;
			nowx=0;
			nowy=0;
			std::cout<<"true"<<std::endl;
         	loop_rate.sleep();
			Task.setSucceeded(result_);
	}
}	

int main(int argc,char** argv){
		ros::init(argc,argv,"action");

		taskAction task(ros::this_node::getName());
		ros::spin();

		return 0;
}
