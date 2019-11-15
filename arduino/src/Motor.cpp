#include"Motor.h"
#include<Arduino.h>
Motor::Motor(int pin1,int pin2){
		pin1_=pin1;
		pin2_=pin2;
		pinMode(pin1_,OUTPUT);
		pinMode(pin2_,OUTPUT);
}
void Motor::run(int speed){
	if(speed>=0){

        analogWrite(pin1_,speed);

        analogWrite(pin2_,0);

    }

    if(speed<=0){

        analogWrite(pin1_,0);

        analogWrite(pin2_,-speed);

    }
}
