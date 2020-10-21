#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<std_msgs/Float64.h>
#include<nav_msgs/Odometry.h>
#include<std_msgs/Bool.h>
#include<CubicSpline.h>
#include<my_msg/Position.h>
class Cmd{
    public:
        Cmd();
        bool flag=false;
        void calc();
    private:
        ros::NodeHandle nh;

        void goalCb(const my_msg::Position&  msg);
        ros::Subscriber goal_sub=nh.subscribe("goal",10,&Cmd::goalCb,this);

        void stateCb(const nav_msgs::Odometry&  msg);
        ros::Subscriber sub=nh.subscribe("odom",10,&Cmd::stateCb,this);
        
        ros::Publisher ord_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",10); 
        ros::Publisher result=nh.advertise<std_msgs::Bool>("result",10); 
        
    
        double now_position_x;
        double now_position_y;
        double now_position_z;
        double kp,ki;
        double time,target_time;
        double endpoint;
        double point,i;
        
        double goalz;
        std::vector<double>goalx;
        std::vector<double>goaly;

        CubicSpline csx;
        CubicSpline csy;
        
        

};
Cmd::Cmd(){
    
    now_position_x=0;
    now_position_y=0;
    now_position_z=0;
    point=0.0; 


}
void Cmd::goalCb(const my_msg::Position& msg){
   
    int len=msg.x.size();
    for(int i=0;i<len;i++){
        goalx.push_back(msg.x[i]);
        goaly.push_back(msg.y[i]);
        goalz=msg.z;
    }
    
    csx.InitParameter(goalx);
    csy.InitParameter(goaly);
    i=0;

}
void Cmd::stateCb(const nav_msgs::Odometry& msg){
    
    now_position_x=msg.pose.pose.position.x;
    now_position_y=msg.pose.pose.position.y;
    now_position_z=msg.pose.pose.position.z;
}
void Cmd::calc(){

    geometry_msgs::Twist mg;
    
    double error_x=abs(goalx[i]-now_position_x);  
    double error_y=abs(goaly[i]-now_position_y);
    double error_z=(goalz-now_position_z);
    
    double scale_x=1-(1.0/error_x)*10;
    double scale_y=1-(1.0/error_y)*10;

    double parameter=((scale_x+scale_y)/2);
    
    floor(parameter);
    
    parameter/=10;

    if(parameter>=0.9)i++,point++;

    parameter+=point;

    csx.Calc(parameter);
    csy.Calc(parameter);

    

    mg.linear.x=csx.accl;
    mg.linear.y=csy.accl;
    mg.angular.z=goalz;
    ord_pub.publish(mg);
    
}

int main(int argc,char**argv){
        ros::init(argc,argv,"pub_cmd");
        Cmd cmd;
        ros::Rate loop_rate(10);
        while(ros::ok()){
            cmd.calc();
            ros::spinOnce();
            loop_rate.sleep();
        }
		return 0;
}


