#ifndef PUREPURSUIT
#define PUREPURSUIT

#include<math.h>
#include<iostream>
#include<vector>
#include<stdio.h>
#include<algorithm>

#define Lfc 0
class State{
    public:
        State(double initx,double inity,double inityaw,double innitv);
        void update(double x_,double y_,double yaw_);
        double calc_distance(double point_x,double point_y);
        double x,y,yaw,v;
};

class TargetCourse{
    public:
        TargetCourse(std::vector<double>x,std::vector<double>y,int num);
        int search_target_index(State state);
        int old_point_index;
        int ind;
        std::vector<double>cx;
        std::vector<double>cy;

};

State::State(double initx,double inity,double inityaw,double initv){
    
    this->x=initx;
    this->y=inity;
    this->v=initv;
    this->yaw=inityaw;
}

void State::update(double x_,double y_,double yaw_){
    x=x_;
    y=y_;
    yaw=yaw_;
}
double State::calc_distance(double point_x,double point_y){
    return hypot(point_x,point_y);
}

TargetCourse::TargetCourse(std::vector<double>x,std::vector<double>y,int num){
    for(int i=0;i<num;i++){

        std::cout<<x[i]<<" "<<y[i]<<std::endl;
        cx.push_back(x[i]);
        cy.push_back(y[i]);

    }
    old_point_index=INT8_MAX;
}

int TargetCourse::search_target_index(State state){
    //最短点を探す
   if(old_point_index==INT8_MAX){
        double dx;
        double dy;
        std::vector<int>d;

        for(int i=0;i<cx.size();i++){
            dx=state.x-cx[i];
            dy=state.y-cy[i];

            d.push_back(hypot(dx,dy));

        }
        std::vector<int>::iterator minIt=std::min_element(d.begin(),d.end());
        ind=std::distance(d.begin(),minIt);
        old_point_index=ind;
    } 
    else{
       double distance_this_index=state.calc_distance(cx[ind],cy[ind]); 

       double distance_next_index=state.calc_distance(cx[ind+1],cy[ind+1]);

       while(true){
            if(distance_this_index<distance_next_index)break;
            else{
                if(ind+1<cx.size()){
                    ind++;
                    distance_next_index=state.calc_distance(cx[ind],cy[ind]);
                }
            }
       }
    }

    double lf=hypot(state.x,state.y)+Lfc;
    while(lf>state.calc_distance(cx[ind],cy[ind])){
        if(ind+1>=cx.size())break;
        ind++;

    }
    return ind,lf;
}

double pursuit_control(State state,TargetCourse targetcorse,int pind){
    int ind=targetcorse.search_target_index(state);
    double tx,ty;

    if(pind>=ind)ind=pind;
    if(ind<targetcorse.cx.size()){
        tx=targetcorse.cx[ind];    
        ty=targetcorse.cy[ind];    


    }
    else{
        tx=targetcorse.cx[targetcorse.cx.size()-1];
        ty=targetcorse.cy[targetcorse.cy.size()-1];

    }

    double alpha=atan2(ty-state.y,tx-state.x)-state.yaw;
    double dist=hypot(tx-state.x,ty-state.y);

    double relx=dist*cos(alpha*(180/M_PI));
    double rely=dist*sin(alpha*(180/M_PI));

    std::cout<<relx<<std::endl;

    return relx,rely,ind;
}
#endif

