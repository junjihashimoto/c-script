#!/usr/bin/env c-script

#pragma c-script:eval_begin
$source_filter=sub{
     $_=$_[0];
     s/Σ/sum/g;
     $_;
}
#pragma c-script:eval_end

int
Σ(int a,int b){
  return a+b;
}

int
main(){
  printf("%d\n",Σ(1,2));
  return 0;
}
