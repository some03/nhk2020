#include<math.h>
template <typename N>class Duty{
    public:
        double x,y,z;
        void ret(N x_,N y_,N z_,N d);
        Duty(){
            x=0;
            y=0;
            z=0;
        }
};
template <class N>void Duty<N>::ret(N x_,N y_,N z_,N d){
    x=d*x_;
    y=d*y_;
    z=d*z_;
}

