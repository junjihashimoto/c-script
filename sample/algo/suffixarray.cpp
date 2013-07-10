#!/usr/bin/env c-script
#include <algorithm>

struct Less{
  const Data<char>& dat;
  Less(const Data<char>& p):dat(p){}
  bool operator()(const int& l,const int& r){
    return strncmp(dat.dat+l, dat.dat+r, 100)<0;
  }
};

struct SuffixArray{
  Data<char> dat;
  Data<int>  ptr;
  
  void mkIndex(){
    ptr.init(dat.len);
    for(int i=0;i<dat.len;i++)
      ptr.push_back(i);
    sort(ptr.dat,ptr.dat+ptr.len,Less(dat));
  }
  void print(){
    data_for(ptr)
      printf("%2d:%2d:%s\n",i,ptr[i],dat.dat+ptr[i]);
  }
  const char* str(int i){
    return dat.dat+ptr[i];
  }
  int search(const char* key,int len,int begin,int end,int flag){
  SEARCH:
    int   pos    =(begin+end)/2;
    
    if(begin==end)
      return -1;
    int v1;
    int v2;
    int v;
    if(flag){
      v1=pos==0 ? -1 : strncmp(str(pos-1),key,len);
      v2=strncmp(str(pos),key,len);
      v =
	v1<0 && v2==0 ?  0 :
	v1<0 && v2<0  ? -1 :
	1;
    }else{
      v1=strncmp(str(pos),key,len);
      v2=pos==ptr.len-1 ? 1 : strncmp(str(pos+1),key,len);
      v =
	v1==0 && v2>0 ?  0 :
	v1>0 &&  v2>0 ?  1 :
	-1;
    }

    if(pos==begin)
      return v==0 ? pos:-1;

    if(v==0){
      return pos;
    }else if(v<0)
      begin=pos;
    else
      end=pos;
    goto SEARCH;
      
  }
  void search(const char* key){
    int len=strlen(key);
    int n=search(key,len,0,dat.len,1);
    int p=search(key,len,0,dat.len,0);
    printf("%d\n",n);
    printf("%d\n",p);
  }
};

int
main(){
  SuffixArray sa;
  char str[]="abrakatabra";
  sa.dat.push_back(str,sizeof(str));
  sa.mkIndex();
  sa.print();
  sa.search("bra");
  sa.search("ab");
  sa.search("atab");
  sa.search("bbbb");
  sa.search("tab");
  sa.search("zzz");
  
  return 0;
}
