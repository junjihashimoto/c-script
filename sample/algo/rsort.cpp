#!/usr/bin/env c-script

#pragma c-script:use benchmark

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
rsort2(vector<int>& dat){
  int cnt[257][4];
  vector<int> r=dat;

  memset(cnt,0,sizeof(cnt));
    
  for(int i=0;i<dat.size();i++)
    for(int j=0;j<4;j++)
      cnt[val(dat[i],j)+1][j]++;
    
  for(int i=1;i<256;i++)
    for(int j=0;j<4;j++)
      cnt[i][j]+=cnt[i-1][j];
    
  for(int j=0;j<4;j++){
    for(int i=0;i<dat.size();i++)
      r[cnt[val(dat[i],j)][j]++]=dat[i];
    dat=r;
  }
    
  return 0;
}

int
rsort3(vector<int>& dat){
  int cnt[0x10001][2];
  vector<int> r=dat;

  memset(cnt,0,sizeof(cnt));
    
  for(int i=0;i<dat.size();i++)
    for(int j=0;j<2;j++)
      cnt[val(dat[i],j)+1][j]++;
    
  for(int i=1;i<0x10000;i++)
    for(int j=0;j<2;j++)
      cnt[i][j]+=cnt[i-1][j];
    
  for(int j=0;j<2;j++){
    for(int i=0;i<dat.size();i++)
      r[cnt[val(dat[i],j)][j]++]=dat[i];
    dat=r;
  }
    
  return 0;
}

int
main(int argc,char** argv){
  vector<int> org;
  vector<int> v;
  int t=10;
  int n=atoi(argc==1?"1024":argv[1]);
  for(int i=0;i<n;i++)
    v.push_back(rand());
  org=v;

  benchmark.reset();
  for(int i=0;i<t;i++){
    v=org;
    benchmark.begin();
    rsort(v);
    benchmark.end();
  }
  benchmark.print();
  
  benchmark.reset();
  for(int i=0;i<t;i++){
    v=org;
    benchmark.begin();
    rsort2(v);
    benchmark.end();
  }
  benchmark.print();
  
  benchmark.reset();
  for(int i=0;i<t;i++){
    v=org;
    benchmark.begin();
    rsort3(v);
    benchmark.end();
  }
  benchmark.print();
  
  benchmark.reset();
  for(int i=0;i<t;i++){
    v=org;
    benchmark.begin();
    sort(v.begin(),v.end());
    benchmark.end();
  }
  benchmark.print();
  


  return  0;
}
