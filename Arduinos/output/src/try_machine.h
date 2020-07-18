#ifndef TRY_MACHINE_H
#define TRY_MACHINE_H
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
    class Injection_machine{
        public:
            Injection_machine();
            const int Airball=41;
            const int Airrale=40;
            const int Airpush=42;

            int pin1=30;
            int pin2=31;
            
            void inject(int num);       
            void motor(int num);
            void init();
    };
    Injection_machine::Injection_machine(){
        pinMode(Airball, OUTPUT);
        pinMode(Airrale, OUTPUT);    
        pinMode(Airpush, OUTPUT);    

        pinMode(pin1, OUTPUT);
        pinMode(pin2, OUTPUT);    
    }
    void Injection_machine::init(){
        digitalWrite(Airball, HIGH);
        digitalWrite(Airrale,LOW);
    }
    void Injection_machine::motor(int num){
        if(num==1){
            digitalWrite(pin1,HIGH);
            digitalWrite(pin2,LOW);
            delay(1);
        }
        if(num==0){
            digitalWrite(pin1,LOW);
            digitalWrite(pin2,HIGH);
            delay(1);
        }
    }
    void Injection_machine::inject(int num){
        if( num == 0){//初期位置
            motor(num);
            digitalWrite(Airball, HIGH);
            digitalWrite(Airrale,LOW);
            //digitalWrite(flag,LOW);
        }
        else if( num == 1){
            digitalWrite(Airball, LOW);
            digitalWrite(Airpush,LOW);
            //digitalWrite(flag,LOW);
        }
        else if( num == 2){//ボールを置く
            motor(num);
            digitalWrite(Airrale,HIGH);
            digitalWrite(Airball, HIGH);
            delay(5000);
            digitalWrite(Airrale,LOW);

            // digitalWrite(flag,HIGH);
        }
        else if( num == 3){
            digitalWrite(Airrale,HIGH);
            digitalWrite(Airball, HIGH);
            delay(5000);
            digitalWrite(Airrale,LOW);
            // digitalWrite(flag,HIGH);
        }
    }
#endif
