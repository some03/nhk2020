#include"controller.h"
#include<Arduino.h>
#include<ros.h>
#include<std_msgs/Bool.h>
#include<geometry_msgs/Twist.h>

Btd btd;
double theta(double x_,double y_){

    double x=(x_-64)*4;
    double y=-1*(y_-64)*4;
    float rad=atan2(y,x);
    float deg=rad/PI*180;

    return deg;
}

ros::NodeHandle nh;
geometry_msgs::Twist mg;
std_msgs::Bool msg;

ros::Publisher pub_cmd("cmd_vel",&mg);
ros::Publisher pub_button("catch",&msg);
ros::Publisher pub_emerge("emergency",&msg);


void setup(){
    //mySerial.begin(2400);
    //Serial.begin(19200);
    nh.initNode(); 
    nh.advertise(pub_cmd);
    nh.advertise(pub_button);
    nh.advertise(pub_emerge);
    
}

void loop(){
    btd.Task();
    
    int x=(btd.Left_x-64)*4;
    int y=-1*(btd.Left_y-64)*4;
    if(theta(x,y)>=70&&theta(x,y)<=110){
        //front
        mg.linear.x=0;
        mg.linear.y=y;
        mg.angular.z=0;
        pub_cmd.publish(&mg);
        nh.spinOnce();
        delay(1);
        #ifdef DEBUG 
        Serial.println("y");
        Serial.println(y);
        delay(1000);
        #endif
    }

    else if(theta(x,y)>=-110&&theta(x,y)<=-70){
        //back;
        
        mg.linear.x=0;
        mg.linear.y=y;
        mg.angular.z=0;

        pub_cmd.publish(&mg);
        nh.spinOnce();
        delay(1);
        

        #ifdef DEBUG 
        Serial.println("y");
        Serial.println(y);
        delay(1000);
        #endif

    }

    else if(theta(x,y)>=-20&&theta(x,y)<=20){
        //right;
        
        mg.linear.x=x;
        mg.linear.y=0;
        mg.angular.z=0;

        pub_cmd.publish(&mg);
        nh.spinOnce();
        delay(1);
        

        #ifdef DEBUG 
        Serial.println("x");
        Serial.println(x);
        delay(1000);
        #endif
    }
    
    else if(theta(x,y)>=160&&theta(x,y)<180.1){
        //left
        
        mg.linear.x=x;
        mg.linear.y=0;
        mg.angular.z=0;

        pub_cmd.publish(&mg);
        nh.spinOnce();
        delay(1);

        #ifdef DEBUG 
        Serial.println("x");
        Serial.println(x);
        delay(1000);
        #endif

    }
    else if(theta(x,y)==0){
        //Serial.println(0);
    }
    else if(btd.getButtonPress("CIRCLE")){
        //button;
        msg.data=true;
        pub_button.publish(&msg);
        nh.spinOnce();
        delay(1);

    }

    else if(btd.getButtonPress("SQUARE")){
        //button

    }

    else if(btd.getButtonPress("CROSS")){
        //button
        msg.data=true;

        pub_emerge.publish(&msg);
        nh.spinOnce();
        delay(1);

    }

    else if(btd.getButtonPress("TRIANGLE")){
        //button

    }
    else {

        mg.linear.x=0;
        mg.linear.y=0;
        mg.angular.z=0;
        pub_cmd.publish(&mg);

        msg.data=false;
        pub_emerge.publish(&msg);
        pub_button.publish(&msg);
        nh.spinOnce();
        delay(1);
        
    }
    
}
