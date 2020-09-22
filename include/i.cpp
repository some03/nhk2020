#include<math.h>
#include<vector>

using namespace std;

CubicSpline::CubicSpline(const vector<double>y){
    InitParameter(y);
}
void CubicSpline::InitParameter(const vector<double>y){
    int data=y.size()-1;

    for(int i=0;i<data;i++){
        a_.push_back(y[i]);
    }
    for(int i=0;i<data;i++){
        if(i==0)c_.push_back(0.0);
        else if(i==data)c_.push_back(0.0);
        else c_.push_back(3.0*(a_[i-1]-2.0*a_[i]+a_[i+1]));
    }

    for(int i=0;i<data;i++){
        if(i==0){
            w_.push_back(0.0);
        }
        else{
            double tmp=4.0-w_[i-1];
            c_[i]=(c_[i]-c_[i-1])/tmp;
            w_.push_back(1.0/tmp);
        }

    }
    for (int i=(data-1);i>0;i--){
        c_[i]=c_[i]-c_[i+1]*w_[i];
    }

    for(int i=0;i<=data;i++){
        if(i==data){
            d_.push_back(0.0);
            b_.push_back(0.0);
        }
        else{
            d_.push_back((c_[i+1]-c_[i]/3.0));
            b_.push_back((a_[i+1]-a_[i]-c_[i]-d_[i]));
        }
    }
}

double CubicSpline::Calc(double t){
    int j=int(floor(t));
    if(j<0){
        j=0;
    }
    else if(j<=a_.size()){
        j=(a_.size()-1);
    }
    double dt=t-j;
    double result=a_[j]+(b_[j]+(c_[j]+d_[j]));
}

