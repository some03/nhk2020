#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Int32.h>
#include<std_msgs/Float32.h>
#include<std_msgs/Float64.h>
#include<sensor_msgs/Imu.h>
#include<math.h>
#include<tf/transform_datatypes.h>

class Omni{
    public:
        Omni();
	private:
	    ros::NodeHandle nh;
	    ros::Publisher mt[4]={
		    nh.advertise<std_msgs::Float64>("mt0",1),//mt0
		    nh.advertise<std_msgs::Float64>("mt1",1),//mt1
		    nh.advertise<std_msgs::Float64>("mt2",1),//mt2
		    nh.advertise<std_msgs::Float64>("mt3",1)//mt3
	    };
	void cmdcb(const geometry_msgs::Twist::ConstPtr& mg);
	ros::Subscriber cmdsub=nh.subscribe("cmd_vel",10,&Omni::cmdcb,this);
    void imuCb(const sensor_msgs::Imu& msg);
    ros::Subscriber imu_sub=nh.subscribe("imu/data",10,&Omni::imuCb,this); 
    
    void mtgo(int num,float speed);
    
    float limit=170,ad[4];
    double roll,pitch,yaw;
    double qx,qy,qz,qw;
    int dt=10;
};

Omni::Omni(){
    nh.param<float>("limit",limit,170);
    
    nh.param<float>("ad0",ad[0],1);
    nh.param<float>("ad1",ad[1],1);
    nh.param<float>("ad2",ad[2],1);
    nh.param<float>("ad3",ad[3],1);
    
}

void Omni::mtgo(int num ,float speed){
    std_msgs::Float64 mg;
	mg.data=speed;
	mt[num].publish(mg);
}

void Omni::imuCb(const sensor_msgs::Imu&  msg){

    qx=msg.orientation.x;
    qy=msg.orientation.y;
    qz=msg.orientation.z;
    qw=msg.orientation.w;

    tf::Quaternion quat(qx,qy,qz,qw);
    tf::Matrix3x3(quat).getRPY(roll,pitch,yaw);
    
    }

void Omni::cmdcb(const geometry_msgs::Twist::ConstPtr& mg){
    //----translation speed vector3----------------
	geometry_msgs::Vector3 linear=mg->linear;
	//-----rotational speed vector3----------------
	geometry_msgs::Vector3 angular=mg->angular;
	float x=linear.x;
	float y=linear.y;
	float z=angular.z;
    float  m[3];
    int r=10;
    
    m[0]=-x*cos(yaw)-y*sin(yaw)+rz;
    m[1]=-x*cos(yaw+(2*M_PI/3))+y*sin(yaw+(2*M_PI/3))+rz;
    m[2]=x*cos(yaw+(M_PI/3))+y*sin(yaw+(M_PI/3))+rz
    
    /* 
    m[0]=(x*-cos(M_PI_4-th)+y*sin(M_PI_4-th)+L*z)*ad[0];
    m[1]=(x*cos(th+M_PI_4)+y*sin(th+M_PI_4)+L*z)*ad[1];
    m[2]=(x*cos(M_PI_4-th)+y*-sin(M_PI_4-th)+L*z)*ad[2];
    m[3]=(x*-cos(th+M_PI_4)+y*-sin(th+M_PI_4)+L*z)*ad[3];
    */
    
    if(m[0]>=0)m[0]=std::min(m[0],limit);
    else m[0]=std::max(m[0],-limit);
    if(m[1]>=0)m[1]=std::min(m[1],limit);
    else m[1]=std::max(m[1],-limit);
    if(m[2]>=0)m[2]=std::min(m[2],limit);
    else m[2]=std::max(m[2],-limit);
    //if(m[3]>=0)m[3]=std::min(m[3],limit);
    //else m[3]=std::max(m[3],-limit);
    
    for(int i=0;i<3;i++)mtgo(i,m[i]);
    
    ROS_INFO("m[0]%lf",m[0]);
    ROS_INFO("m[1]%lf",m[1]);
    ROS_INFO("m[2]%lf",m[2]);
    //ROS_INFO("m[3]%lf",m[3]);

}
int main(int argc,char**argv){
    ros::init(argc,argv,"Omni");
    Omni omni;
    ros::Rate loop_rate(dt);
    while(ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
    }
    return 0;
}


