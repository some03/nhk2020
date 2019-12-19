#ifndef ENCODER_H
#define ENCODER_H
#include<ros.h>
#include<std_msgs/Int32.h>
#include<Arduino.h>

class Encoder{
    public:
        Encoder();
        static int a[];
        static int b[];
        
	    static volatile int M0encB;
	    static volatile long long M0encvalue;
        static volatile int M1encB;
        static volatile long long M1encvalue;
        static volatile int M2encB;
        static volatile long long M2encvalue;
        static volatile int M3encB;
        static volatile long long M3encvalue;
        static void Encm0();
        static void Encm1();
        static void Encm2();
	    static void Encm3();
	    static void set();

};
	int Encoder::b[4]={29,35,37,30};
    int Encoder::a[4]={18,19,20,21};


	volatile int Encoder::M0encB;
	volatile long long Encoder::M0encvalue=0;
	volatile int Encoder::M1encB;
	volatile long long Encoder::M1encvalue=0;
	volatile int Encoder::M2encB;
	volatile long long Encoder::M2encvalue=0;
	volatile int Encoder::M3encB;
	volatile long long Encoder::M3encvalue=0;

//0==A,1==B
Encoder::Encoder(){
    for(int i=0;i<4;i++){
		pinMode(a[i],INPUT_PULLUP);
		pinMode(b[i],INPUT_PULLUP);	
	}
  	

    attachInterrupt(5,Encm0,RISING);
    attachInterrupt(4,Encm1,RISING);
    attachInterrupt(3,Encm2,RISING);
    attachInterrupt(2,Encm3,RISING);
}

void Encoder::Encm0(){
     
  	M0encB=digitalRead(b[0]);
  	if(M0encB)--M0encvalue;
  	else ++M0encvalue;  
		
}

void Encoder::Encm1(){
	M1encB=digitalRead(b[1]);
	if(M1encB)--M1encvalue;
	else ++M1encvalue;
    
}
void Encoder::Encm2(){
	M2encB=digitalRead(b[2]);
    if(M2encB)--M2encvalue;
	else ++M2encvalue;
}

void Encoder::Encm3(){
	M3encB=digitalRead(b[3]);
    if(M3encB)--M3encvalue;
	else ++M3encvalue;
}

void Encoder::set(){
	M0encvalue=0;
	M1encvalue=0;
	M2encvalue=0;
	M3encvalue=0;
}
#endif


