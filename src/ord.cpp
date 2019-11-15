#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Bool.h>
#include<bits/stdc++.h>

class ord{
	private:
		ros::NodeHandle nh;

			
		void posCb(const geometry_msgs::Twist::ConstPtr&mg);
 		ros::Subscriber pSb=nh.subscribe<geometry_msgs::Twist>("cmd_pos",10,&taskAction::posCb,this);
		void goalCb(const geometry_msgs::Twist::ConstPtr&mg);
		ros::Subscriber gSb=nh.subscribe<geometry_msgs::Twist>("goal",
		
		ros::Publisher pub=nh.advertise<std_msgs::Bool>("reached",10);

	public:
			
		double nowx=0,nowy=0,nowz=0;
		double x=0,y=0,z=0;
		double Dx=0,Dy=0,Dz=0;
		int limit=50;
		bool reach=false;
};

void ord::posCb(const geometry_msgs::Twist::ConstPtr&msg){
 		nowx=msg->linear.x;                  
 		nowy=msg->linear.y;
 		nowz=msg->linear.z;                  
		std::cout<<nowx<<std::endl;
 }
