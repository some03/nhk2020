#include<vector>
#include<../../include/pure_pursit.h>
#include<../../include/matplotlib-cpp-starter/matplotlibcpp.h>
#include<math.h>

namespace plt=matplotlibcpp;
#define show_animation

void plot_arrow(auto x,auto y,auto yaw){
    double length=1.0;
    double width=0.5;
    char fc='r';
    char ec='k';
    
   /* double state=dynamic_cast<double>(x);
    
    
    if(state){
        //plt::plot_arrow(x,y,length*cos(yaw),length*sin(yaw),fc,ec,width,width);
        plt::plot(x,y);
    }
    else{
        plt::prot(x,y);
        
    }
    */

}

int main(){
    std::vector<double> cx={3,3.5,4,4.5};
    std::vector<double> cy={0,2,3,1};

    double target_speed=10.0/3.6;

    double T=100.0;

    State state(0.0,0.0,0.0,0.0);
    double lastindex=3;
    double time=0;

    Situation situation;

    situation.append(state,time);
    TargetCourse target_course(cx,cy,4);
    int target_ind,_=target_course.search_target_index(state);

    while(T>=time && lastindex>target_ind){
        double ai=p_control(target_speed,state.v);
        int di,target_ind=pursuit_control(state,target_course,target_ind);
        state.update(ai,di);

        time+=dt;
        situation.append(state,time);

        plt::named_plot("course",cx,cy,"-r");
        plt::named_plot("trajectory",situation.x,situation.y,"-b");
        plt::named_plot("target",cx,cy,"xg");
        plt::pause(0.01);


    }

}
