#ifndef PUREPURSUIT
#define PUREPURSUIT
#include<math.h>
#include<iostream>
#include<vector>
#include<stdio.h>
#include<algorithm>
double k=0.1;
double Lfc=2.0;
double kp=1.0;
double dt=0.1;
double wb=0.5;
class State {
    public:
        State(double initx,double inity,double inityaw,double initv);
        void update(double a,double delta);
        double calc_distance(double point_x,double point_y);
        double x,y,yaw,v;
        double rear_x,rear_y;
};
class Situation{
    public:
        std::vector<double>x;
        std::vector<double>y;
        std::vector<double>yaw;
        std::vector<double>t;
        std::vector<double>v;

        void append(State state,double t);

};

class TargetCourse{
    public:
        TargetCourse(std::vector<double>x,std::vector<double>y,int num);
        std::vector<double>cx;
        std::vector<double>cy;
        double search_target_index(State state);
        int old_point_index=INT8_MAX;
        int ind;

};

//state-----------------------------------------------------------
State::State(double initx,double inity,double inityaw,double initv)
{
    this->x=initx;
    this->y=inity;
    this->v=initv;
    this->yaw=inityaw;

    rear_x=this->x-((wb/2)*cos(yaw));
    rear_y=this->y-((wb/2)*sin(yaw));
}

void State::update(double a,double delta){
   this->x+=this->v*dt; 
   this->y+=this->v*dt; 
   this->yaw+=delta; 

   this->v+=a*dt;

    rear_x=this->x-((wb/2)*cos(yaw));
    rear_y=this->y-((wb/2)*sin(yaw));


}
double State::calc_distance(double point_x,double point_y){
    double dx=rear_x-point_x;
    double dy=rear_y-point_y;

    return hypot(dx,dy);
}

//-------------------------------------------------------------
//situation---------------------------------------------------

//Situation::Situation(){;}

void Situation::append(State state,double t){
    this->x.push_back(state.x);
    this->y.push_back(state.y);
    this->yaw.push_back(state.yaw);
    this->v.push_back(state.v);
    this->t.push_back(t);
    
}
//-------------------------------------------------------------
//targetcorse-------------------------------------------------

TargetCourse::TargetCourse(std::vector<double>x,std::vector<double>y,int num){
    for(int i=0;i<num;i++){
        cx.push_back(x[i]);
        cy.push_back(y[i]);
    }
    
}

double TargetCourse::search_target_index(State state){
//初期位置から最短のポイントを探す---------------
    
    if(old_point_index==INT8_MAX){
        double dx;
        double dy;
        std::vector<int>d;

       for(int i=0;i<cx.size();i++){
           dx=state.rear_x-cx[i];
           dy=state.rear_y-cy[i];

           d.push_back(hypot(dx,dy));
       }
       ind=*std::min_element(d.begin(),d.end());
       old_point_index=ind;
    }
//-----------------------------------------------------
    else{
        ind=old_point_index;
        double distance_this_index=state.calc_distance(cx[ind],cy[ind]);
        while(true){
            double distance_next_index=state.calc_distance(cx[ind+1],cy[ind+1]);

            if(distance_this_index<distance_next_index)break;
            if(ind+1<cx.size()){
                ind+=1;
            }
            distance_this_index=distance_next_index;
        }
        old_point_index=ind;
    }
    double Lf=k*state.v+Lfc;
    while(Lf>state.calc_distance(cx[ind],cy[ind])){
                if(ind+1>cx.size())break;
                ind++;
            }
    return ind,Lf;
    
}


//--------------------------------------------------------------

double p_control(double target,double current){
    double a=kp*(target-current);
    return a;
}

double pursuit_control(State state, TargetCourse trajectory,double pind){
    int ind,Lf=trajectory.search_target_index(state);
    double tx,ty;
    if(pind>=ind)ind=pind;
    if(ind<trajectory.cx.size()){
        tx=trajectory.cx[ind];
        ty=trajectory.cy[ind];
    }
    else{
        tx=trajectory.cx[trajectory.cx.size()-1];
        ty=trajectory.cy[trajectory.cy.size()-1];
        ind=trajectory.cx.size()-1;
    }
    double alpha=atan2(ty-state.rear_y,tx-state.rear_x)-state.yaw;
    double dist=hypot(ty-state.rear_y,tx-state.rear_x);

    int relx=sin(alpha)*dist;
    int rely=cos(alpha)*dist;

    return relx,rely;
}
#endif
