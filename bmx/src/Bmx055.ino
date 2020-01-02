//#define USE_USBCON
#include<Wire.h>
#include<Arduino.h>
#include<ros.h>
#include<MadgwickAHRS.h>
#include<geometry_msgs/Twist.h>
#include<std_msgs/Empty.h>

ros::NodeHandle nh;

Madgwick Md;
//geometry_msgs::TwistStamped imuMsg;
//ros::Publisher imu_pub("imu/raw",&imuMsg);

geometry_msgs::Twist imuMsg;
ros::Publisher imu_pub("imu",&imuMsg);

#define Addr_Accl 0x19 //jp1,2,3 open
#define Addr_Gyro 0x69
#define Addr_Mag  0x13


float xAccl=0.00;
float yAccl=0.00;
float zAccl=0.00;


float xGyro=0.00;
float yGyro=0.00;
float zGyro=0.00;

float xGyroini=0.00;
float yGyroini=0.00;
float zGyroini=0.00;

float xMag=0.00;
float yMag=0.00;
float zMag=0.00;

void resetCb(const std_msgs::Empty &mg){

    xAccl=0;
    yAccl=0;
    zAccl=0;

    xGyro=0;
    yGyro=0;
    zGyro=0;
    }
ros::Subscriber<std_msgs::Empty>set("reset",resetCb);
float count=0;

void setup(){
    Md.begin(1000);
    nh.initNode();
    nh.advertise(imu_pub);
    nh.subscribe(set);
    Wire.begin();
    BMX055_Init();





    delay(100);
}

void loop(){
    
    

    BMX055_Accl();
    BMX055_Gyro();
    BMX055_Mag();


    xGyroini+=0.01;
    yGyroini+=0.01;
    zGyroini+=0.01;
    
    count +=0.01;


    Serial.println("acclx:");
    Serial.println(xAccl);
    Serial.println(" ");
    Serial.println("accly:");
    Serial.println(yAccl);
    Serial.println(" ");
    Serial.println("acclz:");
    Serial.println(zAccl);

    Md.updateIMU(xGyro,yGyro,zGyro,xAccl,yAccl,zAccl);
    float roll=Md.getRollRadians();
    float pitch=Md.getPitchRadians();
    float yaw=-Md.getYawRadians();

    imuMsg.angular.z=yaw;
    imu_pub.publish(&imuMsg);
    nh.spinOnce();
 /*   
    Serial.println(roll);
    Serial.println("  ");
    Serial.println(pitch);
    Serial.println("   ");
    Serial.println(yaw);
    Serial.println("   ");
*/

   /*
    imuMsg.header.stamp=nh.now();
    imuMsg.twist.linear.x=xAccl;
    imuMsg.twist.linear.y=yAccl;
    imuMsg.twist.linear.z=zAccl;

    imuMsg.twist.angular.x=xGyro;
    imuMsg.twist.angular.y=yGyro;
    imuMsg.twist.angular.z=zGyro;

    imu_pub.publish(&imuMsg);
    nh.spinOnce();
*/


}



void BMX055_Init()
{
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Accl);
  Wire.write(0x0F); // Select PMU_Range register
  Wire.write(0x03);   // Range = +/- 2g
  Wire.endTransmission();
  nh.spinOnce();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Accl);
  Wire.write(0x10);  // Select PMU_BW register
  Wire.write(0x08);  // Bandwidth = 7.81 Hz
  Wire.endTransmission();
  nh.spinOnce();
  delay(100);
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Accl);
  Wire.write(0x11);  // Select PMU_LPW register
  Wire.write(0x00);  // Normal mode, Sleep duration = 0.5ms
  Wire.endTransmission();
  nh.spinOnce();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Gyro);
  Wire.write(0x0F);  // Select Range register
  Wire.write(0x04);  // Full scale = +/- 125 degree/s
  Wire.endTransmission();
  nh.spinOnce();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Gyro);
  Wire.write(0x10);  // Select Bandwidth register
  Wire.write(0x07);  // ODR = 100 Hz
  Wire.endTransmission();
  nh.spinOnce();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Gyro);
  Wire.write(0x11);  // Select LPM1 register
  Wire.write(0x00);  // Normal mode, Sleep duration = 2ms
  Wire.endTransmission();
  nh.spinOnce();
  delay(100);
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4B);  // Select Mag register
  Wire.write(0x83);  // Soft reset
  Wire.endTransmission();
  nh.spinOnce();
  delay(100);
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4B);  // Select Mag register
  Wire.write(0x01);  // Soft reset
  Wire.endTransmission();
  nh.spinOnce();
  delay(100);
  //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4C);  // Select Mag register
  Wire.write(0x00);  // Normal Mode, ODR = 10 Hz
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x4E);  // Select Mag register
  Wire.write(0x84);  // X, Y, Z-Axis enabled
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x51);  // Select Mag register
  Wire.write(0x04);  // No. of Repetitions for X-Y Axis = 9
  Wire.endTransmission();
 //------------------------------------------------------------//
  Wire.beginTransmission(Addr_Mag);
  Wire.write(0x52);  // Select Mag register
  Wire.write(0x16);  // No. of Repetitions for Z-Axis = 15
  Wire.endTransmission();
}
//=====================================================================================//
void BMX055_Accl()
{
  int data[6];
  for (int i = 0; i < 6; i++)
  {
    Wire.beginTransmission(Addr_Accl); Wire.write((2 + i));// Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Accl, 1);// Request 1 byte of data
    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data to 12-bits
  xAccl = ((data[1] * 256) + (data[0] & 0xF0)) / 16;
  if (xAccl > 2047)  xAccl -= 4096;
  yAccl = ((data[3] * 256) + (data[2] & 0xF0)) / 16;
  if (yAccl > 2047)  yAccl -= 4096;
  zAccl = ((data[5] * 256) + (data[4] & 0xF0)) / 16;
  if (zAccl > 2047)  zAccl -= 4096;
  xAccl = xAccl * 0.0196;//0.0098; // renge +-2g
  yAccl = yAccl * 0.0196;//0.0098; // renge +-2g
  zAccl = zAccl * 0.0196;//0.0098; // renge +-2g
}
//=====================================================================================//
void BMX055_Gyro()
{
  int data[6];
  for (int i = 0; i < 6; i++)
  {
    Wire.beginTransmission(Addr_Gyro);
    Wire.write((2 + i));    // Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Gyro, 1);    // Request 1 byte of data
    // Read 6 bytes of data
    // xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data
  xGyro = (data[1] * 256) + data[0];
  if (xGyro > 32767)  xGyro -= 65536;
  yGyro = (data[3] * 256) + data[2];
  if (yGyro > 32767)  yGyro -= 65536;
  zGyro = (data[5] * 256) + data[4];
  if (zGyro > 32767)  zGyro -= 65536;

  xGyro = xGyro * 0.0076;//0.0038; //  Full scale = +/- 125 degree/s
  yGyro = yGyro * 0.0076;//0.0038; //  Full scale = +/- 125 degree/s
  zGyro = zGyro * 0.0076;//0.0038; //  Full scale = +/- 125 degree/s
}
//=====================================================================================//
void BMX055_Mag()
{
  int data[8];
  for (int i = 0; i < 8; i++)
  {
    Wire.beginTransmission(Addr_Mag);
    Wire.write((0x42 + i));    // Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Mag, 1);    // Request 1 byte of data
    // Read 6 bytes of data
    // xMag lsb, xMag msb, yMag lsb, yMag msb, zMag lsb, zMag msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data
  xMag = ((data[1] <<8) | (data[0]>>3));
  if (xMag > 4095)  xMag -= 8192;
  yMag = ((data[3] <<8) | (data[2]>>3));
  if (yMag > 4095)  yMag -= 8192;
  zMag = ((data[5] <<8) | (data[4]>>3));
  if (zMag > 16383)  zMag -= 32768;
}    

//=====================================================================================//

void BMX055_AcclInit()
{
  int data[6];
  for (int i = 0; i < 6; i++)
  {
    Wire.beginTransmission(Addr_Accl); Wire.write((2 + i));// Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Accl, 1);// Request 1 byte of data
    // Read 6 bytes of data
    // xAccl lsb, xAccl msb, yAccl lsb, yAccl msb, zAccl lsb, zAccl msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data to 12-bits
  xAccl = ((data[1] * 256) + (data[0] & 0xF0)) / 16;
  if (xAccl > 2047)  xAccl -= 4096;
  yAccl = ((data[3] * 256) + (data[2] & 0xF0)) / 16;
  if (yAccl > 2047)  yAccl -= 4096;
  zAccl = ((data[5] * 256) + (data[4] & 0xF0)) / 16;
  if (zAccl > 2047)  zAccl -= 4096;
  xAccl = xAccl * 0.0196;//0.0098; // renge +-2g
  yAccl = yAccl * 0.0196;//0.0098; // renge +-2g
  zAccl = zAccl * 0.0196;//0.0098; // renge +-2g
}
//=====================================================================================//

void BMX055_GyroInit()
{
  int data[6];
  for (int i = 0; i < 6; i++)
  {
    Wire.beginTransmission(Addr_Gyro);
    Wire.write((2 + i));    // Select data register
    Wire.endTransmission();
    Wire.requestFrom(Addr_Gyro, 1);    // Request 1 byte of data
    // Read 6 bytes of data
    // xGyro lsb, xGyro msb, yGyro lsb, yGyro msb, zGyro lsb, zGyro msb
    if (Wire.available() == 1)
      data[i] = Wire.read();
  }
  // Convert the data
  xGyro = (data[1] * 256) + data[0];
  if (xGyro > 32767)  xGyro -= 65536;
  yGyro = (data[3] * 256) + data[2];
  if (yGyro > 32767)  yGyro -= 65536;
  zGyro = (data[5] * 256) + data[4];
  if (zGyro > 32767)  zGyro -= 65536;

  xGyro = xGyro * 0.0076;//0.0038; //  Full scale = +/- 125 degree/s
  yGyro = yGyro * 0.0076;//0.0038; //  Full scale = +/- 125 degree/s
  zGyro = zGyro * 0.0076;//0.0038; //  Full scale = +/- 125 degree/s
}
