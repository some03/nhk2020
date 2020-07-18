#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <Arduino.h>
#include "Motor.h"
#include "try_machine.h"
#include<std_msgs/Empty.h>

Motor mo0(PC8,PC6);//9 8
Motor mo1(PB1,PB14);//20 21
Motor mo2(PA15,PB7);//4 5
Motor mo3(PA1,PB0);//4 5

Injection_machine inm;
ros::NodeHandle nh;

void m0cb(const std_msgs::Float32 &mg){
		mo0.run(mg.data);
}
void m1cb(const std_msgs::Float32 &mg){
		mo1.run(mg.data);
}
void m2cb(const std_msgs::Float32 &mg){
		mo2.run(mg.data);
}
void m3cb(const std_msgs::Float32 &mg){
		mo3.run(mg.data);
}

void Actuator(const std_msgs::Int32& msg){
        inm.inject(msg.data);
}

ros::Subscriber<std_msgs::Float32>m0s("mt0",&m0cb);
ros::Subscriber<std_msgs::Float32>m1s("mt1",&m1cb);
ros::Subscriber<std_msgs::Float32>m2s("mt2",&m2cb); 
ros::Subscriber<std_msgs::Float32>m3s("mt3",&m3cb);
ros::Subscriber<std_msgs::Int32>inm_sub("try",&Actuator);


void setup(){   
    nh.initNode();
    nh.subscribe(m0s);
    nh.subscribe(m1s);
    nh.subscribe(m2s);
    nh.subscribe(m3s);
    nh.subscribe(inm_sub);

    inm.init();

}
void loop(){
   nh.spinOnce(); 
   delay(10);
}



