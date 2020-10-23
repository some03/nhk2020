#ifndef ODOMETRY_COMPUTE
#define ODOMETRY_COMPUTE

#include<geometry_msgs/Twist.h>
#include<geometry_msgs/PoseStamped.h>
#include<math.h>
#include<std_msgs/Int32.h>
#include<string>

class OdomCompute{
    public:
        OdomCompute(std::string num_,int r);
        geometry_msgs::Pose Updata_Pose(double *cnt,double yaw,int pl);

        std::string num;
        int R;
        double x,y,z;
};
OdomCompute::OdomCompute(std::string num_,int r){
    num=num_;
    R=r;
}
geometry_msgs::Pose OdomCompute::Updata_Pose(double *cnt,double yaw,int pl){
    //単位　mm 
    double radius=2*M_PI*R;

    if(num=="THREE"){
                /* --1
                 *  |
                   / \
                *2-- 3--*/

        for(int i=0;i<3;i++){
            cnt[i]/=pl;
            cnt[i]*=radius;
        }
        x=-cnt[0]*sin(M_PI/2)+cnt[1]*cos(M_PI/3)+cnt[2]*cos(M_PI/2); 
        y=cnt[0]*cos(M_PI/2)-cnt[1]*sin(M_PI/3)+cnt[2]*sin(M_PI/2); 
        x=x*cos(yaw)+y*sin(yaw);
        y=x*-sin(yaw)+y*cos(yaw);
        z=yaw; 

    }

    else if(num=="FOUR"){
        for(int i=0;i<4;i++){
            cnt[i]/=pl;
            cnt[i]*=radius;
        }

         x=-cnt[0]*cos(M_PI_4)+cnt[1]*cos(M_PI_4)+cnt[2]*cos(M_PI_4)-cnt[3]*cos(M_PI_4);
         y=cnt[0]*sin(M_PI_4)+cnt[1]*sin(M_PI_4)-cnt[2]*sin(M_PI_4)-cnt[3]*sin(M_PI_4);
         x=x*cos(yaw)-y*sin(yaw);
         y=x*sin(yaw)+y*cos(yaw);
         z=yaw;
    }
    geometry_msgs::Pose now_pose;

    now_pose.position.x=x;
    now_pose.position.y=y;
    now_pose.position.z=z;

}
#endif
