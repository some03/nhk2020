#include<ros/ros.h>
#include<bits/stdc++.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Int32.h>
#include<std_msgs/Float32.h>

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
			ros::Subscriber cmdsub=nh.subscribe<geometry_msgs::Twist>("cmd_vel",10,&Omni::cmdcb,this);

			void mtgo(int num,float speed);
			float r=0.03;
			float limit=150;
};
void Omni::mtgo(int num ,float speed){
		std_msgs::Float32 mg;
		mg.data=speed;
		mt[num].publish(mg);
}

void Omni::cmdcb(const geometry_msgs::Twist::ConstPtr& mg){
		//translation speed vector3
		geometry_msgs::Vector3 linear=mg->linear;
		//rotational speed vector3
		geometry_msgs::Vector3 angular=mg->angular;
		float x=linear.x;
		float y=linear.y;
		float z=angular.z;
		float m[4];
		m[0]=-sqrt(2)/2*x+sqrt(2)/2*y+z;
		m[1]=sqrt(2)/2*x+sqrt(2)/2*y+z;
		m[2]=sqrt(2)/2*x-sqrt(2)/2*y+z;
		m[3]=-sqrt(2)/2*x-sqrt(2)/2*y+z;

		//m[0]*=10,m[1]*=10,m[2]*=10;

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
