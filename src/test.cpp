#include<ros/ros.h>
#include<bits/stdc++.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Float32.h>
#include<std_msgs/Int32.h>
#include<std_msgs/Bool.h>

#define PI  3.14159265359 
#define GR  1.63636363636
#define PULSE 7000.0
#define R 50.0
class Run{
    public:
        Run();
        void publish();
        void suspend();

		bool go;
	private:
		ros::NodeHandle nh;
        void vsub(const geometry_msgs::Twist::ConstPtr&msg);
		ros::Subscriber ord_sub=nh.subscribe("cmd_ord",10,&Run::vsub,this);
		ros::Publisher ord_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",10);
		ros::Publisher pos_pub=nh.advertise<geometry_msgs::Twist>("cmd_pos",10);
 		void autoCb(const std_msgs::Bool::ConstPtr& Mg);
 		ros::Subscriber auto_sub=nh.subscribe("Go",10,&Run::autoCb,this); 		
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
        
        double v[4]={0,1,2,3};
        double oldth[4]={0,1,2,3};
        //double time;
		
		double nowx,nowy,nowz;
		double kp,r,mxspd;
		double x,y,z;
        double th0,th1,th2,th3;
        double time;

};
Run::Run(){
	nh.param<double>("kp",kp,0.01);
	nh.param<double>("r",r,0.1);
	nh.param<double>("mxspd",mxspd,100);
	

	enc0=0;
	enc1=0;
	enc2=0;
	enc3=0;

    nowx=0;
    nowy=0;
    nowz=0;

    for(int i=0;i<4;i++)v[i]=0,oldth[i]=0;
    
    time=0.0;
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
void Run::vsub(const geometry_msgs::Twist::ConstPtr& Ms){
		x=Ms->linear.x;
		y=Ms->linear.y;
		z=Ms->angular.z;
        std::cout<<x<<std::endl;
}
void Run::autoCb(const std_msgs::Bool::ConstPtr& Mg){
		go=Mg->data;
}


void Run::publish(){


		geometry_msgs::Twist mg;
		geometry_msgs::Twist msg;

        th0=enc0/PULSE*2*PI;
        th1=enc1/PULSE*2*PI;
        th2=enc2/PULSE*2*PI;
        th3=enc3/PULSE*2*PI;

        v[0]=th0-oldth[0]/0.2;
        v[1]=th1-oldth[1]/0.2;
        v[2]=th2-oldth[2]/0.2;
        v[3]=th3-oldth[3]/0.2;

        oldth[0]=th0;
        oldth[1]=th1;
        oldth[2]=th2;
        oldth[3]=th3;
        
        time+=0.2;
       
        //ROS_INFO("time %lf",time);
        ROS_INFO("theta %lf",th0);
        for(int i=0;i<4;i++){
            ROS_INFO("%lf",v[i]);
        }
        //std::cout<<time<<std::endl;


}
//void Run::suspend(){}

int main(int argc,char**argv){
		ros::init(argc,argv,"Test_Run");
		Run run;
		ros::Rate loop_rate(5);
		while(ros::ok()){
		/*	if(run.go)*/
            run.publish();
			//else run.suspend();
			ros::spinOnce();
			loop_rate.sleep();
		}
		return 0;
}

