#!/usr/bin/env c-script

#pragma c-script:libs -fopenmp
#include <omp.h>
#include <algorithm>

int 
val(int dat,int place){
  return (dat >> (8*place))&0xff;
}

int
rsort(vector<int>& dat){
  int cnt[257];
  vector<int> r=dat;

  for(int j=0;j<4;j++){
    memset(cnt,0,sizeof(cnt));
    
    for(int i=0;i<dat.size();i++)
      cnt[val(dat[i],j)+1]++;
    
    for(int i=1;i<256;i++)
      cnt[i]+=cnt[i-1];
    
    for(int i=0;i<dat.size();i++)
      r[cnt[val(dat[i],j)]++]=dat[i];

    dat=r;
  }
    
  return 0;
}

int
main(int argc,char** argv){
  vector<int> v;
  vector<int> org;
  int n=atoi(argc==1?"1024":argv[1]);
  for(int i=0;i<n;i++)
    v.push_back(rand());
  org=v;


  rsort(v);
  //sort(org.begin(),org.end());
  // for(int i=0;i<n;i++)
  //   if(org[i]!=v[i])
  //     printf("error:%d,%d\n",v[i],org[i]);
  return  0;
}
