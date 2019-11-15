//#define USE_USBCON

#include <ros.h>
#include <std_msgs/Int16.h>
#include <std_msgs/UInt16MultiArray.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <Arduino.h>
#include "Motor.h"
#include "Encoder.h"
#include<std_msgs/Empty.h>
Motor mo0(7,6);//9 8
Motor mo1(9,8);//4 5
Motor mo2(4,5);//7 6
Motor mo3(10,11);

Encoder enc;

ros::NodeHandle nh;

//call back
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
void resetCb(const std_msgs::Empty &mg){
		Encoder::set();
}

//subscriber
ros::Subscriber<std_msgs::Float32>m0s("mt0",&m0cb);
ros::Subscriber<std_msgs::Float32>m1s("mt1",&m1cb);
ros::Subscriber<std_msgs::Float32>m2s("mt2",&m2cb); 
ros::Subscriber<std_msgs::Float32>m3s("mt3",&m3cb);
ros::Subscriber<std_msgs::Empty>reset("reset",&resetCb);


//msg
std_msgs::Int32 enc0Mg;
std_msgs::Int32 enc1Mg;
std_msgs::Int32 enc2Mg;
std_msgs::Int32 enc3Mg;


//publisher

ros::Publisher pub0("enc0", &enc0Mg);
ros::Publisher pub1("enc1", &enc1Mg);
ros::Publisher pub2("enc2", &enc2Mg);
ros::Publisher pub3("enc3", &enc3Mg);

int gnd[4]={27,39,33,32};
int v[4]={31,41,25,24};


void setup(){   
	
	Encoder::set();
    
	nh.initNode();
    nh.advertise(pub0);
    nh.advertise(pub1);
    nh.advertise(pub2);
	nh.advertise(pub3);

	nh.subscribe(m0s);
	nh.subscribe(m1s);
	nh.subscribe(m2s);
	nh.subscribe(m3s);
	nh.subscribe(reset);

	for(int i=0;i<4;i++){
		pinMode(v[i],OUTPUT);
		digitalWrite(v[i],HIGH);
		pinMode(gnd[i],OUTPUT);
		digitalWrite(gnd[i],LOW);
	}


 //Encoder::M1oldencB=digitalRead(Encoder::b[0]);
 //Encoder::M2oldencB=digitalRead(Encoder::b[1]);
 //Encoder::M3oldencB=digitalRead(Encoder::b[2]);
    
}

void loop()
{
    enc0Mg.data=enc.M0encvalue;pub0.publish(&enc0Mg);
    nh.spinOnce();
    delay(10);
    enc1Mg.data=enc.M1encvalue;pub1.publish(&enc1Mg);
    nh.spinOnce();
    delay(10);
    enc2Mg.data=enc.M2encvalue;pub2.publish(&enc2Mg);
    nh.spinOnce();
    delay(10);
    enc3Mg.data=enc.M3encvalue;pub3.publish(&enc3Mg);
    nh.spinOnce();
    delay(10);
}
