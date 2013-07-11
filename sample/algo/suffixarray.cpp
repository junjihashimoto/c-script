#!/usr/bin/env c-script
#include <algorithm>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int
_e(const char* file){
  struct stat buf;
  return stat(file, &buf)==0 ? 1: 0;
}

struct Less{
  const Data<char>& dat;
  Less(const Data<char>& p):dat(p){}
  bool operator()(const int& l,const int& r){
    return strncmp(dat.dat+l, dat.dat+r, 100)<0;
  }
};


struct SuffixArray{
  Data<char>  dat;
  Data<int>   ptr;
  
  Data<int>   fileptr;
  Data<char>  filenames;

  void
  setFile(const char* file){
    int p=dat.len;
    fileptr.push_back(p);
    filenames.push_back(file,strlen(file)+1);
    dat.read(file);
  }

  
  void mkIndex(){
    data_for(dat){
      if(dat[i]=='\n')
	dat[i]=0;
      if(dat[i]!=0   &&
	 dat[i]!='\n'&&
	 dat[i]!='\r'&&
	 dat[i]!='\t'&&
	 dat[i]!=' ')
	ptr.push_back(i);
    }
    sort(ptr.dat,ptr.dat+ptr.len,Less(dat));
  }
  void print(){
    data_for(ptr)
      printf("%2d:%2d:'%s'\n",i,ptr[i],dat.dat+ptr[i]);
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
  int search(const char* key,int& head){
    int len=strlen(key);
    int n=search(key,len,0,ptr.len,1);
    int p=search(key,len,0,ptr.len,0);
    //    printf("%d\n",n);
    //    printf("%d\n",p);
    if(n<0){
      head=-1;
      return 0;
    }
    head=n;
    return p-n+1;
  }
  void
  search(const char* key){
    int head;
    int n=search(key,head);
    printf("%s,%d,%d\n",key,n,head);
  }
};

int
main(){
  if(0){
    SuffixArray sa;
    char str[]="abrakatabra\n\n\nabrakatabra";
    sa.dat.push_back(str,sizeof(str));
    sa.mkIndex();
    sa.print();
    sa.search("bra");
    sa.search("ab");
    sa.search("atab");
    sa.search("bbbb");
    sa.search("tab");
    sa.search("zzz");
  }
  //  return 0;
  {
    SuffixArray sa;
    sa.dat.read("docs/dat.txt");
    if(!_e("docs/ptr.bin")){
      sa.mkIndex();
      sa.ptr.write("docs/ptr.bin");
    }else
      sa.ptr.read("docs/ptr.bin");
      
    sa.search("href");
    char buf[1024];
    while(gets(buf)){
      sa.search(buf);
    }
  }
  
  return 0;
}
