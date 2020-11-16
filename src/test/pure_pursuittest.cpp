#include<vector>
#include<../../include/Pure_pursuit.h>
#include<../../include/matplotlib-cpp-starter/matplotlibcpp.h>
#include<math.h>

namespace plt=matplotlibcpp;
/*
void plot_arrow(auto x,auto y,auto yaw){
    double length=1.0;
    double width=0.5;
    char fc='r';
    char ec='k';
    
    double state=dynamic_cast<double>(x);
    
    
    if(state){
        //plt::plot_arrow(x,y,length*cos(yaw),length*sin(yaw),fc,ec,width,width);
        plt::plot(x,y);
    }
    else{
        plt::prot(x,y);
        
    }
    

}
*/

std::vector<double> cx={1,2,3,4};
std::vector<double> cy={1,2,3,4};

double target_speed=10.0/3.6;

double T=100.0;

State state(0.0,0.0,0.0,0.0);
double lastindex=4;
double Time=0;

double dt=0.5;

TargetCourse target_couse(cx,cy,4);


double nowx,nowy,nowyaw; 
double relx,rely;
int Target_ind=target_couse.search_target_index(state);//初期 
int main(){
    std::cout<<Target_ind;

    while(T>=Time && lastindex>Target_ind){
       
        relx,rely,Target_ind=pursuit_control(state,target_couse,Target_ind);
        nowx+=0.1; 
        nowy+=0.1; 
        
        state.update(nowx,nowy,nowyaw);

        Time+=dt;

       // relx/=dt;
       // rely/=dt;

        std::cout<<"X:"<<relx<<std::endl;
        std::cout<<"Y:"<<rely<<std::endl;
        std::cout<<"ind:"<<Target_ind<<std::endl;

    }
    relx=0.0;//停止
    rely=0.0;


/*
        plt::named_plot("course",cx,cy,"-r");
        //plt::named_plot("trajectory",situation.x,situation.y,"-b");
        plt::named_plot("target",cx,cy,"xg");
        plt::axis("equal");
        plt::grid(true);
        plt::pause(0.01);
        
   */ 

}
