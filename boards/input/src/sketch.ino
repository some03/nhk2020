//#define USE_USBCON
#include <ros.h>
#include<geometry_msgs/Twist.h>
#include <sensor_msgs/Imu.h>
#include <std_msgs/Int32.h>
#include <std_msgs/Bool.h>
#include <std_msgs/Float32.h>
#include <Arduino.h>
#include <Encoder.h>
#include <Bmx055.h>
#include<std_msgs/Empty.h>

ros::NodeHandle nh;
Encoder enc;

//msg
std_msgs::Int32 enc0Mg;
std_msgs::Int32 enc1Mg;
std_msgs::Int32 enc2Mg;
std_msgs::Int32 enc3Mg;

sensor_msgs::Imu imuMg;


//publisher

ros::Publisher pub0("enc0", &enc0Mg);
ros::Publisher pub1("enc1", &enc1Mg);
ros::Publisher pub2("enc2", &enc2Mg);
ros::Publisher pub3("enc3", &enc3Mg);

ros::Publisher imuPb("imu/data_raw",&imuMg);

void resetCb(const std_msgs::Empty &mg){
		Encoder::set();
}

ros::Subscriber<std_msgs::Empty>reset("reset",&resetCb);

void setup(){   

	nh.initNode();
    nh.advertise(pub0);
    nh.advertise(pub1);
    nh.advertise(pub2);
	nh.advertise(pub3);
    nh.advertise(imuPb);
    nh.subscribe(reset);

    Wire.begin();
    BMX055_Init();
    delay(300);
    
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

    
    BMX055_Accl();
    BMX055_Gyro();

    imuMg.header.stamp=nh.now();
    imuMg.header.frame_id="imu_link";
    imuMg.linear_acceleration.x=xAccl;
    imuMg.linear_acceleration.y=yAccl;
    imuMg.linear_acceleration.z=zAccl;

    imuMg.angular_velocity.x=xGyro;
    imuMg.angular_velocity.y=yGyro;
    imuMg.angular_velocity.z=zGyro;


    imuPb.publish(&imuMg);
    
    
    nh.spinOnce();
    delay(1);
    
}
