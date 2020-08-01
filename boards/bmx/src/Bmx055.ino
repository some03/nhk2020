#include<Wire.h>
#include<ros.h>
#include<geometry_msgs/TwistStamped.h>
#include<sensor_msgs/Imu.h>
#include<sensor_msgs/MagneticField.h>
#include<geometry_msgs/Vector3Stamped.h>
#include<geometry_msgs/Vector3.h>
#include<geometry_msgs/Twist.h>

ros::NodeHandle nh;
geometry_msgs::Twist imuMsg;
//grometry_msgs::Vector3 imuMg;
sensor_msgs::Imu imuMg;
sensor_msgs::MagneticField  magMg;

//geometry_msgs::Vector3Stamped magMg;
ros::Publisher imuPb("imu/data_raw",&imuMg);
ros::Publisher magPb("imu/mag",&magMg);

ros::Publisher MdPb("imu",&imuMsg);
//ros::Publisher magPb("imu/mag",&magMg);

#define Addr_Accl 0x19  // (JP1,JP2,JP3 = Open)
#define Addr_Gyro 0x69  // (JP1,JP2,JP3 = Open)
#define Addr_Mag 0x13   // (JP1,JP2,JP3 = Open)

float xAccl = 0.00;
float yAccl = 0.00;
float zAccl = 0.00;
float xGyro = 0.00;
float yGyro = 0.00;
float zGyro = 0.00;
int   xMag  = 0;
int   yMag  = 0;
int   zMag  = 0;

void setup()
{
  nh.initNode();
  nh.advertise(imuPb);
  nh.advertise(magPb);
  nh.advertise(MdPb);
  //nh.advertise(magPb);
  Wire.begin();
  BMX055_Init();
  delay(300);
}

void loop()
{
  BMX055_Accl();
  BMX055_Gyro();
  //BMX055_Mag();
 /*   
  imuMg.header.stamp=nh.now();
  imuMg.twist.linear.x=xAccl;
  imuMg.twist.linear.y=yAccl;
  imuMg.twist.linear.z=zAccl;
  imuMg.twist.angular.x=xGyro;
  imuMg.twist.angular.y=yGyro;
  imuMg.twist.angular.z=zGyro;
  imuPb.publish(&imuMg);
  nh.spinOnce();
 */ 
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

/*--if use magnetic_field------------
  
  BMX055_Mag();
 
  magMg.header.stamp=nh.now();
  magMg.header.frame_id="imu";
  magMg.magnetic_field.x=xMag;
  magMg.magnetic_field.y=yMag;
  magMg.magnetic_field.z=zMag;
  magPb.publish(&magMg);
  nh.spinOnce();
  delay(1);
----------------------------------------*/
/*--when madgwick filter is used-------------------------
  Md.updateIMU(xGyro,yGyro,zGyro,xAccl,yAccl,zAccl);
  float roll=Md.getRollRadians();
  float pitch=Md.getPitchRadians();
  float yaw=-Md.getYawRadians();

   imuMsg.angular.z=yaw;
   MdPb.publish(&imuMsg);
   nh.spinOnce();
 -------------------------------------------------------*/ 
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
  xAccl = xAccl * 0.0098*0.102; // renge +-2g
  yAccl = yAccl * 0.0098*0.102; // renge +-2g
  zAccl = zAccl * 0.0098*0.102; // renge +-2g
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

  xGyro = xGyro * 0.0038*(PI/180); //  Full scale = +/- 125 degree/s
  yGyro = yGyro * 0.0038*(PI/180); //  Full scale = +/- 125 degree/s
  zGyro = zGyro * 0.0038*(PI/180); //  Full scale = +/- 125 degree/s
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

