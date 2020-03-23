#include <ros/ros.h>
#include<geometry_msgs/Twist.h>
   
 
geometry_msgs::Twist mg;
void Cb(const geometry_msgs::Twist& msg){
        mg.linear.x=msg.linear.x;
        mg.linear.y=msg.linear.y;
        mg.angular.z=msg.angular.z;
     }
int main(int argc, char** argv){
    ros::init(argc, argv, "ps3");
     
    ros::NodeHandle nh;

    ros::Publisher ord_pub=nh.advertise<geometry_msgs::Twist>("cmd_vel",10); 

    ord_pub.publish(mg);
    ros::Subscriber sub=nh.subscribe("ps3_cmd_vel",10,&Cb);  
    ros::Rate loop_rate(1.0);
    ros::spin();
   }
