#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<geometry_msgs/Pose2D.h>
#include<std_msgs/Float64.h>
#include<nav_msgs/Odometry.h>
#include<std_msgs/Bool.h>
#include<temp.h>
class Cmd{
    public:
        Cmd();
        bool flag=false;
        void calc();
    private:
        ros::NodeHandle nh;

        void goalCb(const geometry_msgs::PoseStamped&  msg);
        ros::Subscriber goal_sub=nh.subscribe("goal",10,&Cmd::goalCb,this);

        void stateCb(const nav_msgs::Odometry&  msg);
        ros::Subscriber sub=nh.subscribe("odom",10,&Cmd::stateCb,this);
        
        ros::Publisher ord_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",10); 
        ros::Publisher result=nh.advertise<std_msgs::Bool>("result",10); 
        
    
        double x,px;
        double y,py;
        double z,pz;
        double now_speed_x,target_speed_x,now_position_x;
        double now_speed_y,target_speed_y,now_position_y;
        double now_speed_z,target_speed_z,now_position_z;
        double kp,td;
        double time,target_time;
        int cnt;
        

};
Cmd::Cmd(){
    x=0;
    y=0;     
    z=0;     
    now_speed_x=0,target_speed_x=0,now_position_x=0;
    now_speed_y=0,target_speed_y=0,now_position_y=0;
    now_speed_z=0,target_speed_z=0,now_position_z=0;
    
    nh.param<double>("kp",kp,90); 
    nh.param<double>("td",td,0); 
    nh.param<double>("target_time",target_time,4); 
    nh.param<int>("cnt",cnt,1);//447300); 


}
void Cmd::goalCb(const geometry_msgs::PoseStamped& msg){
    
    x=msg.pose.position.x;
    y=msg.pose.position.y;
    z=msg.pose.position.z;
    
    flag=true;
    
    ROS_INFO("x:%lf",x);
    ROS_INFO("y:%lf",y);
    ROS_INFO("z:%lf",z);

}
void Cmd::stateCb(const nav_msgs::Odometry& msg){
    
    now_position_x=msg.pose.pose.position.x;
    now_position_y=msg.pose.pose.position.y;
    now_position_z=msg.pose.pose.position.z;
}
void Cmd::calc(){

    geometry_msgs::Twist mg;
    
    if(flag)time=0;
    time+=0.5;
    
    target_speed_x=(x/cnt)/target_time;
    target_speed_y=(y/cnt)/target_time;
    target_speed_z=(z/cnt)/target_time;

    
    //2.5m/s  time=目標時間-経過時間
    if(target_time-time<=0.5){
        mg.linear.x=0;
        mg.linear.y=0;
        mg.linear.z=0;

        ord_pub.publish(mg);
        flag=false;
        while(!flag){
            ;
            
        }
        
        
    }
//

    now_speed_x=((x-now_position_x)/cnt)/(target_time-time);  
    now_speed_y=((y-now_position_y)/cnt)/(target_time-time);
    now_speed_z=((z-now_position_z)/cnt)/(target_time-time);

    double speed_x=target_speed_x+kp*(target_speed_x-now_speed_x);
    double speed_y=target_speed_y+kp*(target_speed_y-now_speed_y);
    double speed_z=target_speed_z+kp*(target_speed_z-now_speed_z);

    Duty<double>duty;
    duty.ret(speed_x,speed_y,speed_z,1);
    mg.linear.x=duty.x;
    mg.linear.y=duty.y;
    mg.angular.z=duty.z;

/*
    px=kp*((x-nowx)/cnt+td*(oldx/0.01));
    py=kp*((y-nowy)/cnt+td*(oldy/0.01));
    pz=kp*((z-nowz)/cnt+td*(oldz/0.01));
   
    oldx=(x-nowx);
    oldy=(x-nowy);
    oldz=(x-nowz);
*/
    ord_pub.publish(mg);
    
    ROS_INFO("now_speed_x:%lf",now_speed_x);
    ROS_INFO("now_speed_y:%lf",now_speed_y);
    ROS_INFO("tx:%lf",mg.linear.x);
    ROS_INFO("ty:%lf",mg.linear.y);
}

int main(int argc,char**argv){
        ros::init(argc,argv,"pub_cmd");
        Cmd cmd;
        ros::Rate loop_rate(2);
        while(ros::ok()){
            cmd.calc();
            ros::spinOnce();
            loop_rate.sleep();
        }
		return 0;
}

