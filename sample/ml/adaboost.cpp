#!/usr/bin/env c-script
template<class Func,class Inputs>
struct AdaBoost{
    vector<Inputs> HX;
    vector<double> Y;
    vector<double> D;
    vector<Func>   h;
    vector<double> a;
    void update(){
        //err=sum(D (ht(x)^Y)) && err < 0.5
        //at=0.5*ln((1-err)/err)
        //D=x(D,exp(-x(HX,Y)*at))/sum(D)
        //H=sign(sum(at*h(x)))
        
    }
    
};
int
main(){
    return 0;
}
