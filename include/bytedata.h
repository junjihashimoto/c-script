#ifndef BYTEVEC_H
#define BYTEVEC_H


template<class T>
struct ByteVec{
  T*  dat;
  int len;
  int buflen;
  ByteVec(int initsize=128){
    buflen=128;
    len=0;
    dat=(T*)malloc(sizeof(T)*buflen);
    if(dat==NULL){
      printf("Can't allocate memory\n");
      exit(EXIT_FAILURE);
    }
  }
  ~ByteVec(){
    free(dat);
    dat=NULL;
  }
  ByteVec(ByteVec& copy){
  }
  int pushzero(int size){
  }
  int push(T* p,int size=1){
  }
  int push(T p){
  }
  int pop(int size=1){
  }
  int read(const char* file){
    FILE* fin = fopen(file,"rb");
  }
  int write(const char* file){
    FILE* fout = fopen(file,"wb");
  }
  operator T*(){
    return dat;
  }
};

typedef ByteVec<unsigned char> Data;

#endif
