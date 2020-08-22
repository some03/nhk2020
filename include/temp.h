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
    x=100+(x_/10)*10*5;
    y=100+(y_/10)*10*5;
    z=100+(z_/10)*10*5;

    
    if(x>170)x=170,if(x<-170)x=-170;
    if(y>170)y=170,if(y<-170)y=-170;
    if(z>170)z=170,if(z<-170)y=-170;

    if(x_<1&&x_>-1)x=0;
    if(y_<1&&y_>-1)y=0;
    if(z_<1&&z_>-1)z=0;
    
}

