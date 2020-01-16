#include<ros/ros.h>
#include<bits/stdc++.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Float32.h>
#include<std_msgs/Float64.h>
#include<std_msgs/Int32.h>
#include<std_msgs/Bool.h>

class Run{
	public:
		Run();
		void publish();
		void suspend();
		bool go;
	private:
		ros::NodeHandle nh;
        void vsub(const geometry_msgs::Twist::ConstPtr  &msg);
		ros::Subscriber ord_sub=nh.subscribe("cmd_ord",10,&Run::vsub,this);
 		
        void autoCb(const std_msgs::Bool::ConstPtr  &Mg);
 		ros::Subscriber auto_sub=nh.subscribe("Go",10,&Run::autoCb,this); 		
        
        void imuCb(const geometry_msgs::Twist::ConstPtr &imuMsg);
        ros::Subscriber imu_sub=nh.subscribe("imu",10,&Run::imuCb,this);
        
        void accelCb(const std_msgs::Float64::ConstPtr &msg);
        ros::Subscriber accel_sub=nh.subscribe("accel",10,&Run::accelCb,this);


		ros::Publisher ord_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",10);
		ros::Publisher pos_pub=nh.advertise<geometry_msgs::Twist>("cmd_pos",10);
		
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
		
		double nowx,nowy,nowz;
		double kp,r,mxspd;
		double x,y,z;
        double rad;
        double accel;
};
Run::Run(){
	nh.param<double>("kp",kp,0.1);
	nh.param<double>("r",r,1);
	nh.param<double>("mxspd",mxspd,1000);
	

	enc0=0;
	enc1=0;
	enc2=0;
	enc3=0;

	x=0,y=0,z=0;

	go=false;

    


}

void Run::m0(const std_msgs::Int32::ConstPtr& msg){
		enc0=msg->data;
}
void Run::m1(const std_msgs::Int32::ConstPtr& msg){
		enc1=msg->data;
}
void Run::m2(const std_msgs::Int32::ConstPtr& msg){
		enc2=msg->data;
}

void Run::m3(const std_msgs::Int32::ConstPtr& msg){
		enc3=msg->data;
}

void Run::imuCb(const geometry_msgs::Twist::ConstPtr &imuMsg){
        rad=imuMsg->angular.z;
}
void Run::vsub(const geometry_msgs::Twist::ConstPtr &Ms){
		x=Ms->linear.x;
		y=Ms->linear.y;
		z=Ms->angular.z;
}
void Run::autoCb(const std_msgs::Bool::ConstPtr &Mg){
		go=Mg->data;
}
void Run::accelCb(const std_msgs::Float64::ConstPtr &msg){
        accel=msg->data;
}

void Run::publish(){
		geometry_msgs::Twist mg;
		geometry_msgs::Twist msg;
		nowx= 0.01*(-enc0+enc1+enc2-enc3);
		nowy= 0.01*(enc0+enc1-enc2-enc3);
		nowz=r*rad;

      
		mg.linear.x=(kp*accel)*(x-nowx);
		mg.linear.y=(kp*accel)*(y-nowy);
		mg.angular.z=(z-nowz);

		msg.linear.x=nowx;
		msg.linear.y=nowy;
		msg.angular.z=nowz;
	
		if(mg.linear.x>=0)mg.linear.x=std::min(mg.linear.x,mxspd);
        else mg.linear.x=std::max(mg.linear.x,-mxspd);


        if(mg.linear.y>=0)mg.linear.y=std::min(mg.linear.y,mxspd);
        else mg.linear.y=std::max(mg.linear.y,-mxspd);

        if(mg.linear.z>=0)mg.angular.z=std::min(mg.angular.z,mxspd);
        else mg.angular.z=std::max(mg.angular.z,-mxspd);


		std::cout<<"x"<<mg.linear.x<<" "<<"y"<<mg.linear.y<<" "<<"z"<<mg.angular.z<<std::endl;	
	//	std::cout<<nowx<<" "<<nowy<<" "<<nowz<<std::endl;	
	    std::cout<<kp+accel;	
		pos_pub.publish(msg);
		ord_pub.publish(mg);	

}
void Run::suspend(){
    std::cout<<"suspend"<<std::endl;
}


int main(int argc,char**argv){
		ros::init(argc,argv,"Run");
		Run run;
		ros::Rate loop_rate(5);
		while(ros::ok()){
			if(run.go)run.publish();
			else run.suspend();
			ros::spinOnce();
			loop_rate.sleep();
		}
		return 0;
}


