#include<ros/ros.h>
#include<jsk_rviz_plugins/OverlayText.h>
#include<string>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Bool.h>
#include<std_msgs/Int32.h>

class Info{
    public:
        Info();
        void publish();
        ros::NodeHandle nh;
        ros::Publisher text_pub=nh.advertise<jsk_rviz_plugins::OverlayText>("text",1);
    private:
        void odmCb(const geometry_msgs::Twist::ConstPtr &mg);
        ros::Subscriber odm_sub=nh.subscribe<geometry_msgs::Twist>("cmd_vel",10,&Info::odmCb,this);
        void tryCb(const std_msgs::Int32::ConstPtr &msg);
        ros::Subscriber try_sub=nh.subscribe<std_msgs::Int32>("try",10,&Info::tryCb,this);
        int num;
        double x,y,z;
};
Info::Info(){
    num=0;
    x=0;
    y=0;
    z=0;
}


void Info::odmCb(const geometry_msgs::Twist::ConstPtr &mg){
        x=mg->linear.x;
        y=mg->linear.y;
        z=mg->angular.z;
}

void Info::tryCb(const std_msgs::Int32::ConstPtr &msg){
    num=msg->data;
}

void Info::publish(){
    jsk_rviz_plugins::OverlayText text;
    text.action=jsk_rviz_plugins::OverlayText::ADD;
    text.width=500;
    text.height=300;
    text.left=10;
    text.top=0;

    std_msgs::ColorRGBA color1,color2;
    color1.r = 0;
    color1.g = 0;
    color1.b = 0;
    color1.a = 0.4;
    text.bg_color=color1;

    color2.r = 25.0/255;
    color2.g = 255.0/255;
    color2.b = 240.0/255;
    color2.a = 0.8;
    text.fg_color=color2;

    text.line_width=30;
    text.text_size=20;
    text.font="ubuntu";

    text.text=printf(" mode:%d\n",num);
          
          text_pub.publish(text);
}

int main(int argc,char**argv){
    ros::init(argc,argv,"infomation");
    Info info;
    ros::Rate loop_rate(1);
    while(ros::ok()){
        ros::spinOnce();
        loop_rate.sleep();
        info.publish();
    }
    return 0;
}
