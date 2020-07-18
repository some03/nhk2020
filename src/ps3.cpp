#include <ros/ros.h>
#include<geometry_msgs/Twist.h>
   
 
geometry_msgs::Twist mg;
void Cb(const geometry_msgs::Twist& msg){
        double x=msg.linear.x;
        double y=msg.linear.y;
        double z=msg.angular.z;
        mg.linear.x=x;
        mg.linear.y=y;
        mg.linear.z=z;
        ROS_INFO("%lf",mg.linear.x);
     }
int main(int argc, char** argv){
    ros::init(argc, argv, "ps3");
     
    ros::NodeHandle nh;

    ros::Publisher ord_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",10); 
    ros::Subscriber sub=nh.subscribe("ps3_cmd_vel",10,&Cb);  
    ros::Rate loop_rate(1.0);
    while(ros::ok()){
       ord_pub.publish(mg); 
       ros::spinOnce();
       loop_rate.sleep();
    }

}
