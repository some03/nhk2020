#include<vector>
#include<../include/CubicSpline.h>
#include<../include/matplotlib-cpp-starter/matplotlibcpp.h>

namespace plt=matplotlibcpp;

int main(){
    std::vector<double>sx{-5,-5,-3,2,3,0,-2};
    std::vector<double>sy{6,1,6,7,1,-1,0};


    CubicSpline csy(sy);
    CubicSpline csx(sx);

    std::vector<double>rx;
    std::vector<double>ry;

    for(double i=0.0;i<=10;i+=0.1){
        ry.push_back(csy.Calc(i));
        rx.push_back(csx.Calc(i));
        cout<<rx[i]<<endl;
    }
    
    plt::named_plot("TRUES",sx,sy,"xb");
    plt::named_plot("interporation",rx,ry,"-r");
    plt::legend();
    plt::axis("equal");
    plt::grid(true);
    plt::show();
}
