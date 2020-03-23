#ifndef TRY_MACHINE_H
#define TRY_MACHINE_H
#include <std_msgs/Int32.h>
#include <std_msgs/String.h>
#include <std_msgs/Bool.h>
class Injection_machine{
    public:
        const int Airball=41;
        const int Airrale=40;
        void inject(int num);       

};
void Injection_machine::inject(int num){
    if( num == 0){//初期位置
       digitalWrite(Airball, LOW);
       digitalWrite(Airrale,LOW);
       //digitalWrite(flag,LOW);
    }else if( num == 1){//tryゾーン到着!!
       digitalWrite(Airball, LOW);
       digitalWrite(Airrale,HIGH);
       //digitalWrite(flag,LOW);
    }else if( num == 2){//ボールを置く
       digitalWrite(Airball, HIGH);
       digitalWrite(Airrale,HIGH);
      // digitalWrite(flag,HIGH);
    }
}
#endif
