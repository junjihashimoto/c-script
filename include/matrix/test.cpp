#include <cstdio>
#include <cstdlib>

template<class T>
struct T1{
  int  n;
  T* dat;
  T1(int n){
    this->n=n;
    dat=(T*)malloc(n*sizeof(T));
  }
  ~T1(){
    free(dat);
  }
  void print(){
    for(int i=0;i<n;i++)
      printf("%d\n",dat[i]);
  }
  
};

template<class T,int N,int M>
struct T1<T[N][M]> :T1<T>{
  //  int  n;
  T dat[N*M];
  T1(){
    this->n=N*M;
  }
  // void print(){
  //   for(int i=0;i<n;i++)
  //     printf("%d\n",dat[i]);
  // }
};


int
main(){
  {
    T1<int> t(3);
    for(int i=0;i<3;i++)
      t.dat[i]=i;
    t.print();
  }
  {
    T1<int[3][1]> t;
    for(int i=0;i<3;i++)
      t.dat[i]=i;
    t.print();
  }
  

  return 0;
}
