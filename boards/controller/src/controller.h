#ifndef BTD_H
#define BTD_H

#include<SoftwareSerial.h>
#include<string>
#define MYRX 12
#define MYTX 11

SoftwareSerial mySerial(MYRX,MYTX);

class Btd{
    public:
        Btd();
        int i;
        unsigned char c[8];
        unsigned long chksum;
        int Left_x;
        int Left_y;
        int Right_x;         
        int Right_y;
        
        void Task();
        bool getButtonPress(std::string name);
        bool Check(int c,int num);
};
Btd::Btd(){
    c[0]=0x80;
}
bool Btd::Check(int c,int num){
    
    if((c&num)==num)return true;
    else return false;
    
}
bool Btd::getButtonPress(std::string name){
    bool rep;
    if(name=="CROSS") rep=this->Check(c[2],0x20);
    else if(name=="CIRCLE") rep=this->Check(c[2],0x40);
    else if(name=="TRIANGLE") rep=this->Check(c[2],0x10);
    else if(name=="SQUARE") rep=this->Check(c[1],0x01);
    else rep=false; 
    return rep;
}
void Btd::Task(){
    if(mySerial.available()>=8){
        if(mySerial.read()==0x80){
            for(chksum=c[0],i=1;i<8;i++){
                c[i]=mySerial.read();
                if(i<7)chksum+=c[i];
            }
            //if(c[7]!=(chksum & 0x7F))throw "check sum EROOR";
            
            String leftstickx=String(c[3],DEC);
            String leftsticky=String(c[4],DEC);
            String rightstickx=String(c[5],DEC);
            String rightsticky=String(c[6],DEC);

            Left_x=leftstickx.toInt();
            Left_y=leftsticky.toInt();
            Right_x=rightsticky.toInt();
            Right_y=rightsticky.toInt();
            #if DEBUG
                Serial.println("left_x: "leftstickx);
                Serial.println("left_y: "leftsticky);
                Serial.println("right_x: "rightstickx);
                Serial.println("right_y: "rightsticky);
            #endif
        }
    }
}
#endif
