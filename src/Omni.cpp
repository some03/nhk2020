#include<ros/ros.h>
#include<bits/stdc++.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Int32.h>
#include<std_msgs/Float32.h>
#include<tf2/LinearMath/Quaternion.h>
#include<tf2/LinearMath/Matrix3x3.h>
#include<sensor_msgs/Imu.h>
#define PI  3.14159265359 
#define GR  1.63636363636
#define PULSE 7100.0
#define R 50.0
#define l 70

class Omni{
		private:
			ros::NodeHandle nh;
			ros::Publisher mt[4]={
				nh.advertise<std_msgs::Float32>("mt0",1),
				nh.advertise<std_msgs::Float32>("mt1",1),
				nh.advertise<std_msgs::Float32>("mt2",1),
				nh.advertise<std_msgs::Float32>("mt3",1)
			};

			void cmdcb(const geometry_msgs::Twist::ConstPtr& mg);
			ros::Subscriber cmdsub=nh.subscribe("cmd_vel",10,&Omni::cmdcb,this);

            void imuCb(const sensor_msgs::Imu& msg);
            ros::Subscriber imu_sub=nh.subscribe("imu/data",10,&Omni::imuCb,this); 
			
            void mtgo(int num,float speed);
			float r=0.05;
			float limit=170;
            int th=0;

            double qx,qy,qz,qw;
            double roll,pitch,yaw;
};

void Omni::imuCb(const sensor_msgs::Imu&  msg){
        
        qx=msg.orientation.x;
        qy=msg.orientation.y;
        qz=msg.orientation.z;
        qw=msg.orientation.w;

        
        tf2::Quaternion quat(qx,qy,qz,qw);
        tf2::Matrix3x3(quat).getRPY(roll,pitch,yaw);
}       

void Omni::mtgo(int num ,float speed){
		std_msgs::Float32 mg;
		mg.data=speed;
		mt[num].publish(mg);
}

void Omni::cmdcb(const geometry_msgs::Twist::ConstPtr& mg){
        //----translation speed vector3----------------
		geometry_msgs::Vector3 linear=mg->linear;
		//-----rotational speed vector3----------------
		geometry_msgs::Vector3 angular=mg->angular;
		float x=linear.x;
		float y=linear.y;
		float z=angular.z;
        float  m[4];
        if(x<8||y<8){
            float  x=linear.x*100;
            float  y=linear.y*100;
            float  z=yaw;
        }
        if(x>=8||y>=8){
		    float x=linear.x;
		    float y=linear.y;
		    float z=angular.z;
        }
		m[0]=-sqrt(2)/2*x+sqrt(2)/2*y+r*z;
		m[1]=sqrt(2)/2*x+sqrt(2)/2*y+r*z;
		m[2]=sqrt(2)/2*x-sqrt(2)/2*y+r*z;
		m[3]=-sqrt(2)/2*x-sqrt(2)/2*y+r*z;
       /* 
        th=0;

        m[0]=-r*x*cos(th*PI/4)+r*y*sin(th*PI/4)+r*z;
        m[1]=r*x*cos(th*PI*3/4)+r*y*sin(th*PI/4)+r*z;
        m[2]=r*x*cos(th*PI*3/4)+r*y*sin(th*PI*3/4)+r*z;
        m[3]=-r*x*cos(th*PI/4)+r*y*sin(th*PI*3/4)+r*z;

        */
		if(m[0]>=0)m[0]=std::min(m[0],limit);
		else m[0]=std::max(m[0],-limit);
		if(m[1]>=0)m[1]=std::min(m[1],limit);
		else m[1]=std::max(m[1],-limit);
		if(m[2]>=0)m[2]=std::min(m[2],limit);
		else m[2]=std::max(m[2],-limit);
		if(m[3]>=0)m[3]=std::min(m[3],limit);
		else m[3]=std::max(m[3],-limit);


		for(int i=0;i<4;i++)mtgo(i,m[i]);

}
int main(int argc,char**argv){
		ros::init(argc,argv,"Omni");
		Omni omni;
		ros::spin();
		return 0;
}


