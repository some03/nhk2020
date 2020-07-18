#ifndef MA_H
#define MA_H
#include<ros/ros.h>
#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<vector>
#include<math.h>
#include<std_msgs/Int32.h>
class Odom{
    public:
        Odom();
        
        geometry_msgs::Twist Odom_Invert(double *v,int yaw);
        geometry_msgs::Pose Updata_Pose(geometry_msgs::Twist speed);
        double Caluc(long theta,double L);

        double data[3][4];
        double vx,vy,vz;
        double buf;

};


Odom::Odom(){
    vx=0.0,vy=0.0,vz=0.0;
}

double Odom::Caluc(long theta,double L){
    double A[4][3],AT[3][4],ATA[3][3],Inv[3][3];
//+++++++++++++++++++++++++++++++++++++++++++++++++
    A[0][0]=-sin(theta+M_PI_4);
    A[0][1]=cos(theta+M_PI_4);
    A[0][2]=L;
    A[1][0]=sin(theta+M_PI_4);
    A[1][1]=cos(theta+M_PI_4);
    A[1][2]=L;
    A[2][0]=sin(theta+M_PI_4);
    A[2][1]=-cos(theta+M_PI_4);
    A[2][2]=L;
    A[3][0]=-sin(theta+M_PI_4);
    A[3][1]=-cos(theta+M_PI_4);
    A[3][2]=L;

    for(int i=0;i<4;i++){
        for(int j=0;j<3;j++){
       //cout<<A[i][j]<<" ";
        }
    //    cout<<endl;
    }
    //cout<<"A"<<endl;
    
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
            AT[i][j]=A[j][i];
           // cout<<AT[i][j]<<" ";
        }
     //   cout<<endl;

    }
    //cout<<"AT"<<endl;

    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
           ATA[i][j]=AT[i][j]*A[i][j]; 
           for(int i=0;i<3;i++){
                for(int j=0;j<3;j++){
                    double sum=0;
                    for(int k=0;k<4;k++){
                        sum=sum+AT[i][k]*A[k][j];
                        ATA[i][j]=sum;
                    }
                }
            }
           //cout<<ATA[i][j]<<" ";
        }
        //cout<<"ATA"<<endl;
    }
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            Inv[i][j]=(i==j)?1.0:0.0;
        }
    }
//+++++++++++++++++++++++++++++++++++++++++++++++++++
    for(int i=0;i<3;i++){
        buf=1/ATA[i][i];
        for(int j=0;j<3;j++){
            ATA[i][j]*=buf;
            Inv[i][j]*=buf;
        }
        for(int j=0;j<3;j++){
            if(i!=j){
                buf=ATA[i][j];
                for(int k=0;k<3;k++){
                    ATA[j][k]-=ATA[i][k]*buf;
                    Inv[j][k]-=Inv[i][k]*buf;
                }
            }
        }
    }
    /*
    for(int i=0;i<3;i++){
        for(int j=0;j<3;j++){
            //cout<<Inv[i][j]<<" ";
        }
        //cout<<endl;
    }
    cout<<" inv"<<endl;
    */
//+++++++++++++++++++++++++++++++++++++++++++++++++++
    for(int i=0;i<3;i++){
        for(int j=0;j<4;j++){
           data[i][j]=Inv[i][j]*AT[i][j]; 
           for(int i=0;i<3;i++){
                for(int j=0;j<4;j++){
                    double sum=0;
                    for(int k=0;k<3;k++){
                        sum=sum+Inv[i][k]*AT[k][j];
                        data[i][j]=sum;
                    }
                }
            }
           //cout<<data[i][j]<<" ";
        }
        //cout<<"data"<<endl;
    }

}
  
geometry_msgs::Twist Odom::Odom_Invert(double *v,int yaw){
    geometry_msgs::Twist odom;

    for(int n=0;n<4;n++)vx=data[0][n]*v[n],vy=data[1][n]*v[n],vz=data[2][n];
    
    odom.linear.x=vx;
    odom.linear.y=vy;
    odom.angular.z=vz;

    return odom;

}

geometry_msgs::Pose Odom::Updata_Pose(geometry_msgs::Twist speed){
    geometry_msgs::Pose next_pose;

    next_pose.position.x=speed.linear.x;
    next_pose.position.y=speed.linear.y;
    next_pose.position.z=speed.angular.z;
}
#endif
