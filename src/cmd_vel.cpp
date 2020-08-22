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
        
    
        double x,xi;
        double y,yi;
        double z,zi;
        double now_speed_x,now_position_x;
        double now_speed_y,now_position_y;
        double now_speed_z,now_position_z;
        double kp,ki;
        double time,target_time;
        int cnt;
        

};
Cmd::Cmd(){
    x=0;
    y=0;     
    z=0;     
    now_speed_x=0,now_position_x=0;
    now_speed_y=0,now_position_y=0;
    now_speed_z=0,now_position_z=0;
    
    nh.param<double>("kp",kp,1.5); 
    nh.param<double>("ki",ki,0); 
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

    double error_x=((x-now_position_x)/cnt);  
    double error_y=((y-now_position_y)/cnt);
    double error_z=((z-now_position_z)/cnt);


    xi+=ki*0.5*error_x;
    yi+=ki*0.5*error_y;
    zi+=ki*0.5*error_z;

    double mv_x=kp*(error_x)+xi;
    double mv_y=kp*(error_y)+yi;
    double mv_z=kp*(error_z)+zi;

    Duty<double>duty;
    duty.ret(mv_x,mv_y,mv_z,1.0);

    mg.linear.x=duty.x;
    mg.linear.y=duty.y;
    mg.angular.z=duty.z;
    ord_pub.publish(mg);
    
    ROS_INFO("now_speed_x:%lf",mv_x);
    ROS_INFO("now_speed_y:%lf",mv_y);
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

