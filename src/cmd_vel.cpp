#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<geometry_msgs/Pose2D.h>
#include<std_msgs/Float64.h>
#include<nav_msgs/Odometry.h>
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

        void stateCb(const nav_msgs::Odometry&  msg);
        ros::Subscriber sub=nh.subscribe("odom",10,&Cmd::stateCb,this);
        ros::Publisher ord_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",10); 
        ros::Publisher result=nh.advertise<std_msgs::Bool>("result",10); 
    
        double x,vx;
        double y,vy;
        double z,vz;
        int nowx=0,oldx=0;
        int nowy=0,oldy=0;
        int nowz=0,oldz=0;
        double kp,td;
        int cnt;
        

};
Cmd::Cmd(){
    success=false;
    nh.param<double>("kp",kp,90); 
    nh.param<double>("td",td,0); 
    nh.param<int>("cnt",cnt,447300); 
}
void Cmd::goalCb(const geometry_msgs::PoseStamped& msg){
        
        x=msg.pose.position.x;
        y=msg.pose.position.y;
        z=msg.pose.position.z;

        ROS_INFO("%lf",x);
        ROS_INFO("%lf",y);
        ROS_INFO("%lf",z);

}

void Cmd::stateCb(const nav_msgs::Odometry& msg){
        nowx=msg.pose.pose.position.x;
        nowy=msg.pose.pose.position.y;
        nowz=msg.pose.pose.position.z;

        vx=kp*((x-nowx)/cnt+td*(oldx/0.01));
        vy=kp*((y-nowy)/cnt+td*(oldy/0.01));
        vz=kp*((z-nowz)/cnt+td*(oldz/0.01));
       
        oldx=(x-nowx);
        oldy=(x-nowy);
        oldz=(x-nowz);
}

void Cmd::publish(){
        geometry_msgs::Twist mg;
        mg.linear.x=vx;
        mg.linear.y=vy;
        mg.angular.z=vz;
        ord_pub.publish(mg);
      
    
}



int main(int argc,char**argv){
		ros::init(argc,argv,"pub_cmd");
		Cmd cmd;
        ros::Rate loop_rate(10);
        while(ros::ok()){
            cmd.publish();
            ros::spinOnce();
            loop_rate.sleep();
        }
		return 0;
}

