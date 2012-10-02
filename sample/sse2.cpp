#!/usr/bin/env c-script

#include <emmintrin.h>

short v[8]={0,1,2,3,4,5,6,7};

int
main(){
  __m128i a,b,c;
  a=_mm_loadu_si128((__m128i*)v);
  b=_mm_set1_epi16(2);
  c=_mm_mullo_epi16(a,b);
  _mm_storeu_si128 ((__m128i*)v,c);

  for(int i=0;i<8;i++)
    printf("%d,",(int)v[i]);
  printf("\n");
  
  return 0;
}
