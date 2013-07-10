#ifndef TEXT_H
#define TEXT_H

template<class T>
struct Data{
  int len;
  int mlen;
  T*  dat;
  Data();
  Data(int size);
  Data(const Data& org);
  ~Data();
  T*       init(int size);
  void     clear();
  T*       push_back(const T* ptr,int size);
  T*       push_back(const T& dat);
  Data&    operator =  (const Data& b);
  T&       operator [] (int k);
  const T& operator [] (int k)const;
  int     read(const char* file);
  int     write(const char* file)const;
};

typedef Data<char> DATA;

#define data_for(data) for(int i=0;i<data.len;i++)

template<class T> vector<T> split(const T& str,const char* delim);
template<class T> T*        get_line(FILE* in,Data<T>& dat);

template<class T>
Data<T>::Data(){
  dat=NULL;
  len=0;
  mlen=0;
  init(64);
}

template<class T>
Data<T>::Data(int size){
  dat=NULL;
  len=0;
  mlen=0;
  init(size);
}

template<class T>
Data<T>::Data(const Data& org){
  dat=NULL;
  init(org.mlen);
  len=org.len;
  memcpy(dat,org.dat,sizeof(T)*org.len);
}

template<class T>
Data<T>::~Data(){
  free(dat);
  dat=NULL;
  mlen=0;
  len=0;
}


template<class T>
T*
Data<T>::init(int size){
  if(dat!=NULL)
    free(dat);
  len=0;
  mlen=size;
  dat=(T*)malloc(sizeof(T)*mlen);
  assert(dat!=NULL);
  return dat;
}

template<class T>
void
Data<T>::clear(){
  len=0;
}

template<class T>
T*
Data<T>::push_back(const T* ptr,int size){
  while(1){
    if((mlen-len)>=size){
      memcpy(dat+len,ptr,sizeof(T)*size);
      len+=size;
      return dat;
    }
    int newlen=mlen*2;
    while((newlen-len)<size)
      newlen*=2;
    T* ptr=(T*)realloc(dat,sizeof(T)*newlen);
    if(ptr==NULL)
      return NULL;
    mlen=newlen;
    dat=ptr;
  }
}

template<class T>
T*
Data<T>::push_back(const T& dat){
  return push_back(&dat,1);
}

template<class T>
Data<T>&
Data<T>::operator = (const Data<T>& d){
  init(d.mlen);
  len=d.len;
  memcpy(dat,d.dat,sizeof(T)*d.len);
  return *this;
}

template<class T>
T&
Data<T>::operator [] (int k){
  return dat[k];
}

template<class T>
const T&
Data<T>::operator [] (int k)const{
  return dat[k];
}

template<class T>
int
Data<T>::read(const char* file){
  FILE* f=fopen(file,"rb");
  if(f!=NULL){
    T buf[1024];
    int i;
    while(i=fread(buf,sizeof(T),1024,f))
      push_back(buf,i);
    fclose(f);
    return 1;
  }
  return 0;
}

template<class T>
int
Data<T>::write(const char* file)const{
  FILE* f=fopen(file,"wb");
  if(f!=NULL){
    T*  p=dat;
    int l=len;
    int i;
    do{
      i=fread(p,sizeof(T),l,f);
      p+=i;
      l-=i;
    }while(l>0);
    fclose(f);
    return 1;
  }
  return 0;
}


template<class T>
vector<T>
split(const T& str,const char* delim){
  vector<T> vec;
  T d=delim;
  T buf;
  for(int i=0;i<str.length();i++){
    for(int j=0;j<d.length();j++){
      if(str[i]==d[j]){
	vec.push_back(buf);
	buf="";
	goto SPLIT_NEXT;
      }
    }
    buf+=str[i];
  SPLIT_NEXT:
    ;
  }
  vec.push_back(buf);
  return vec;
}

template<class T>
T*
get_line(FILE* in,Data<T>& dat){
  dat.clear();
  int v;
  while((v=getc(in))!=EOF){
    if(v=='\n'){
      dat.push_back((T)'\0');
      return dat.dat;
    }
    dat.push_back((T)v);
  }
  return NULL;
}


#endif
