#include<PS3BT.h>
#include<PS3USB.h>
#include<usbhub.h>
#include<ros.h>
#include<std_msgs/Bool.h>
#include<std_msgs/Int32.h>
#include<Arduino.h>
#include<geometry_msgs/Twist.h>


USB Usb;
BTD Btd(&Usb);
PS3BT PS3(&Btd);

int direction(int x, int y) {
  double deg , rad;
  int pattern = 0;
  x -= 128;
  y = abs(y - 256) - 128;
  rad = atan2((double)y, (double)x);
  deg = rad * 180.0 / (atan(1.0) * 4.0);
  if (abs(x) > 16 || abs(y) > 16) {
    deg += 180.0;
    for (int i = 0; i < 8; i++) {
      if (deg >= 0.0 + (i * 22.5) && deg < 22.5 + (i * 22.5)) {//分割数を指定
        pattern = i + 1;
      }
    }
  }
  return pattern;

}
ros::NodeHandle nh;

bool manual;
int count;
double r=1.3;

void ml_Cb(const std_msgs::Bool &Mg){
    manual=Mg.data;
}

geometry_msgs::Twist mg;
std_msgs::Bool msg;
std_msgs::Bool Msg;
std_msgs::Bool Mg;
std_msgs::Int32 ms;
ros::Publisher go_pub("Go",&Mg);
ros::Publisher sw_pub("switch",&msg);
ros::Publisher ord_pub("cmd_vel",&mg);
ros::Publisher try_pub("success",&Msg);
ros::Publisher ct_pub("catch_ball",&Msg);
ros::Publisher ml_pub("order",&ms);

ros::Subscriber<std_msgs::Bool>manu("manual",ml_Cb);

void setup(){
    #if !defined(__MIPSEL__)
    	#ifdef DEBUG
    		while (!Serial);
     	#endif
        

	#endif
    if (Usb.Init() == -1) {
      
     #ifdef DEBUG
      Serial.print(F("\r\nOSC did not start"));
      #endif

      

      while (1);
    
    }
    manual=false;
    count=0;
    nh.initNode();
    nh.advertise(sw_pub);
    nh.advertise(ord_pub);
    nh.advertise(try_pub);
    nh.advertise(ct_pub);
    nh.advertise(go_pub);
    nh.advertise(ml_pub);
    nh.subscribe(manu);

    mg.linear.x=0;
    mg.linear.y=0;
    mg.angular.z=0;
}
void loop(){
	Usb.Task();
  if(PS3.PS3Connected||PS3.PS3NavigationConnected){

		double lx = PS3.getAnalogHat(LeftHatX);
		double ly = PS3.getAnalogHat(LeftHatY);
        double rx = PS3.getAnalogHat(RightHatX);
        double ry = PS3.getAnalogHat(RightHatY);
		double cwx=(lx-140)*r;//(50+(lx-128))*0.6;
		double cwy=-(ly-140)*r;//-(127-(abs(ly-256)));
		double ccwx=(lx-115)*r;//abs(-50+(lx-128))*0.6;
		double ccwy=abs(ly-115)*r;//abs(-127-(abs(ly-235)))*0.5;
        double fr=sqrt(pow(cwx,2)+pow(cwy,2));
        double fl=sqrt(pow(ccwx,2)+pow(cwy,2));
        double bl=sqrt(pow(ccwx,2)+pow(ccwy,2));
        double br=sqrt(pow(cwx,2)+pow(ccwy,2));

        double rcwx=(rx-140)*r;
        double rccwx=abs(rx-115)*r;
            
                     
            
    
		if (PS3.getAnalogHat(LeftHatX) > 137||PS3.getAnalogHat(LeftHatX) < 117|| PS3.getAnalogHat(LeftHatY) > 137|| PS3.getAnalogHat(LeftHatY) < 117){  
				switch (direction(PS3.getAnalogHat(LeftHatX), PS3.getAnalogHat(LeftHatY))){
					case 0:
						mg.linear.x=0;
						mg.linear.y=0;
						mg.angular.z=0;
                        if(!manual)ord_pub.publish(&mg);
						break;
					case 1:
					case 16:
                        //left
						mg.linear.x=ccwx;
						mg.linear.y=0;//sqrt(pow(cwx,2)+pow(cwy,2));
						mg.angular.z=0;
						if(!manual)ord_pub.publish(&mg);
						break;
					case 2:
					case 3:
                        //left
						mg.linear.x=bl;
						mg.linear.y=bl;//sqrt(pow(cwx,2)+pow(cwy,2));
						mg.angular.z=0;
						if(!manual)ord_pub.publish(&mg);
						break;
					case 4:
					case 5:
                        //right
						mg.linear.x=0;
						mg.linear.y=ccwy;
						mg.angular.z=0;
						if(!manual)ord_pub.publish(&mg);
						break;

					case 6:
					case 7:
						mg.linear.x=br;
						mg.linear.y=br;
						mg.angular.z=0;
						if(!manual)ord_pub.publish(&mg);
						break;
					case 8:
					case 9:
						mg.linear.x=cwx;
						mg.linear.y=0;
						mg.angular.z=0;
						if(!manual)ord_pub.publish(&mg);
						break;
					case 10:
					case 11:
						mg.linear.x=fr;
						mg.linear.y=fr;
						mg.angular.z=0;
						if(!manual)ord_pub.publish(&mg);
						break;
					case 12:
					case 13:
						mg.linear.x=0;
						mg.linear.y=cwy;
						mg.angular.z=0;
						if(!manual)ord_pub.publish(&mg);
						break;
					case 14:
					case 15:
						mg.linear.x=fl;
						mg.linear.y=fl;
						mg.angular.z=0;
						if(!manual)ord_pub.publish(&mg);
						break;
                    default:
                        break;
				}
			}
            else if (PS3.getAnalogHat(RightHatX) > 137 || PS3.getAnalogHat(RightHatX) <117 || PS3.getAnalogHat(RightHatY) > 137 || PS3.getAnalogHat(RightHatY) < 117){
//if(direction(PS3.getAnalogHat(LeftHatX), PS3.getAnalogHat(LeftHatY))==0)
    			switch (direction(PS3.getAnalogHat(RightHatX), PS3.getAnalogHat(RightHatY))) {
           				case 0:
			    			mg.linear.x=0;
						    mg.linear.y=0;
						    mg.angular.z=0;
						    if(!manual)ord_pub.publish(&mg);
						    break;
                
           				case 1:
           				case 8:
               				mg.linear.x=0;
                			mg.linear.y=0;
                			mg.angular.z=-rccwx*0.003;
                			if(!manual)ord_pub.publish(&mg);
                			break;
            			case 4:
            			case 5:
                			mg.linear.x=0;
                			mg.linear.y=0;
                			mg.angular.z=rcwx*0.003;
               				if(!manual) ord_pub.publish(&mg);
                			break;
                		}
            }
            
        

	  else{
                
			mg.linear.x=0;
			mg.linear.y=0;
			mg.angular.z=0;
            if(!manual)ord_pub.publish(&mg);

			if(PS3.getButtonPress(CIRCLE)){
				Msg.data=true;
                Mg.data=true;
                ms.data=0;
                ml_pub.publish(&ms);
				sw_pub.publish(&Msg);
			}
			else if(PS3.getButtonPress(CROSS)){
				msg.data=false;
                Mg.data=false;

				mg.linear.x=0;
				mg.linear.y=0;
				mg.angular.z=0;
                ord_pub.publish(&mg);
                        
				sw_pub.publish(&msg);
                go_pub.publish(&Mg);
			}
            else if(PS3.getButtonPress(TRIANGLE)){
                Msg.data=true;
				msg.data=true;
                Mg.data=true;
                ms.data=0;
                ml_pub.publish(&ms);
				sw_pub.publish(&msg);
                go_pub.publish(&Mg);
                ct_pub.publish(&Msg);
            }

            else if(PS3.getButtonPress(SQUARE)){
                Msg.data=true;
				msg.data=true;
                Mg.data=true;
                ms.data=0;
                ml_pub.publish(&ms);
				sw_pub.publish(&msg);
                go_pub.publish(&Mg);
                try_pub.publish(&Msg);
            }
               
                    
            else{
                    Msg.data=false;
                    ct_pub.publish(&Msg);
                    try_pub.publish(&Msg);
                    sw_pub.publish(&Msg);
            }

		}

			nh.spinOnce();
			
	}
    
    
    
}




