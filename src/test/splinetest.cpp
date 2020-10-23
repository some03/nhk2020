#include<vector>
#include<../../include/CubicSpline.h>
#include<../../include/matplotlib-cpp-starter/matplotlibcpp.h>

namespace plt=matplotlibcpp;

int main(){
    std::vector<double>sy{3,1,0,0};
    std::vector<double>sx{0,0,0,0};


    CubicSpline csy(sy);
    CubicSpline csx(sx);

    std::vector<double>rx;
    std::vector<double>ry;

    for(double i=0.0;i<=4;i+=0.2){
        csy.Calc(i);
        ry.push_back(csy.accl);
        cout<<"y:"<<" "<<csy.accl<<endl;
        csx.Calc(i);
        rx.push_back(csx.accl);
        cout<<"x"<<" "<<csx.accl<<endl;;
    }
    
    plt::named_plot("TRUES",sx,sy,"xb");
    plt::named_plot("interporation",rx,ry,"-r");
    plt::legend();
    plt::axis("equal");
    plt::grid(true);
    plt::show();
    
}
