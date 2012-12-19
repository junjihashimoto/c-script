#!/usr/bin/env c-script

#pragma c-script:opt -I/usr/include/suitesparse
#pragma c-script:libs -lcsparse
#pragma c-script:libs -lcxsparse
#pragma c-script:libs -lumfpack

#define EXTERN extern "C"

#include <cs.h>
#include <umfpack.h>

#define set(A,xd,yd,v)  cs_entry (A,((x)+(y)*w),((xd)+(yd)*w), (v)) ;

int
limit(double v){
  return
        v<0 ? 0 :
	v> 255 ? 255 : v;
}

void usolve(cs* A,
	   const double* b,
	   double* x);



cs*
mkcoeff(const BMP& in){
  cs* AR;
  cs* A;
  int w=in.w;
  int h=in.h;
  int n=in.h*in.w;
  A=cs_spalloc(n,n,n*5,1,1);

  for(int y=0;y<in.h;y++)
    for(int x=0;x<in.w;x++){
      if(y==0||x==0||x==in.w-1||y==in.h-1){
	set(A, x, y, 1.0);
      }else{
	set(A, x, y-1, 1.0/6);
	set(A, x-1, y, 1.0/6);
	set(A, x,   y, 2.0/6);
	set(A, x+1, y, 1.0/6);
	set(A, x, y+1, 1.0/6);
      }
    }
  AR = cs_compress (A) ;             /* Eye = speye (m) */

  cs_spfree(A);

  return AR;
}

cs*
mkeyec(const BMP& in,double c){
  cs* AR;
  cs* A;
  int w=in.w;
  int h=in.h;
  int n=in.h*in.w;
  A=cs_spalloc(n,n,n*5,1,1);

  for(int y=0;y<in.h;y++)
    for(int x=0;x<in.w;x++)
      set(A, x, y, c);
  AR = cs_compress (A) ;             /* Eye = speye (m) */
  cs_spfree(A);

  return AR;
}


int
main(int argc,char** argv){
  BMP in2("testimg.bmp");
  BMP in(3,3);

  // bmp_for(in)
  //   in(x,y)=in2(x+500,y+200);
  bmp_for(in)
    in(x,y)=in2(x,y);
  in.write("in.png");


  BMP out=in;
  int w=in.w;
  int h=in.h;
  int n=in.h*in.w;

  

  for(int y=1;y<in.h-1;y++)
    for(int x=1;x<in.w-1;x++)
      for(int z=0;z<3;z++)
	out(x,y,z)=
	  (in(x,y-1,z)+
	   in(x-1,y,z)+in(x,y,z)*2+in(x+1,y,z)+
	   in(x,y+1,z))/6;

  out.write("low.png");


  //min  |ax-b|+c|x|
  //min   xtatax-2xtatb+btb+cxtx
  //min   xt(ata+cE)x-2xtatb+btb
  //    (ata+cE)x=atb
  double* img= (double*)malloc(sizeof(double)*n);
  double* rhs= (double*)malloc(sizeof(double)*n);
  double* lhs= (double*)malloc(sizeof(double)*n);

  cs* A      = mkcoeff(out);
  cs* AT     = cs_transpose (A, 1) ;          /* AT = A' */
  cs* ATA    = cs_multiply (AT, A) ;
  cs_di_print(ATA,0);

  double c   = 0.01;
  cs* EyeC   = mkeyec(out,c);
  cs* ATA_EyeC = cs_add(ATA,EyeC,1.0,1.0);
  // cs* E = cs_compress(ATA_EyeC);

  // cs_di_print(cs_compress(EyeC) ,0);
  // return 0;

  cs_spfree(ATA);
  cs_spfree(EyeC);
  
  for(int k=0;k<3;k++){

    for(int i=0;i<n;i++)
      img[i]=out(i%in.w,i/in.w,k);
  
    for(int i=0;i<n;i++)
      rhs[i]=0;

    cs_gaxpy(AT,img,rhs);
  
    //    usolve(ATA_EyeC,rhs,lhs);
    for(int i=0;i<n;i++)
      out(i%in.w,i/in.w,k)=limit(lhs[i]);
  }
  out.write("restore.png");
  return 0;
}

/* Sample program for the UMFPACK sparse matrix solver

   Based on illustrative sample from UMFPACK's QuickStart.pdf
   with slight stylistic changes by RR.

   Solves the system Ax=b, where:

   A = 
       2   3   0   0   0
       3   0   4   0   6
       0  -1  -3   2   0
       0   0   1   0   0
       0   4   2   0   1

   b = (8, 45, -3, 3, 19)

   The solution x is:

   x = (1, 2, 3, 4, 5)

   RR, November 2003
*/

void
usolve(cs* A,
      const double* b,
      double* x){
  void *Symbolic=NULL, *Numeric=NULL;
  int i;
  // cs* A2=cs_compress(A);
  // cs_spfree(A2);
  

  for(int j=0;j<A->m;j++)
    assert(A->p>=0);
  for(int j=0;j<A->p[A->m];j++)
    assert(A->i>=0);

  /* symbolic analysis */
  int v;
  v=umfpack_di_symbolic(A->m,
		      A->n,
		      A->p,
		      A->i,
		      A->x,
		      &Symbolic,
		      NULL,
		      NULL);
  double Control [UMFPACK_CONTROL] ;
  Control[UMFPACK_PRL]=200;
  umfpack_di_report_matrix(A->m,
			   A->n,
			   A->p,
			   A->i,
			   A->x,
			   0,
			   Control);
  assert(v==0);

  /* LU factorization */
  v=umfpack_di_numeric(A->p,
		     A->i,
		     A->x,
		     Symbolic,
		     &Numeric,
		     NULL,
		     NULL);
  assert(v==0);
  umfpack_di_free_symbolic(&Symbolic);

  /* solve system */
  umfpack_di_solve(UMFPACK_A, A->p, A->i, A->x, x, b, Numeric, NULL, NULL);
  umfpack_di_free_numeric(&Numeric);
}


int
test_main(int argc,char** argv){
  cs* A=cs_spalloc(5,5,25,1,1);
  cs_entry(A,0,0,2);
  cs_entry(A,0,1,3);
  cs_entry(A,1,0,3);
  cs_entry(A,1,2,4);
  cs_entry(A,1,4,6);
  cs_entry(A,2,1,-1);
  cs_entry(A,2,2,-3);
  cs_entry(A,2,3,2);
  cs_entry(A,3,2,1);
  cs_entry(A,4,1,4);
  cs_entry(A,4,2,2);
  cs_entry(A,4,4,1);
  
  cs* AR = cs_compress (A);
  cs_spfree(A);

  double* b=(double*)malloc(sizeof(double)*5);
  double* x=(double*)malloc(sizeof(double)*5);
  b[0]=8;
  b[1]=45;
  b[2]=-3;
  b[3]=3;
  b[4]=19;

  usolve(AR,b,x);
  for(int i=0;i<5;i++)
    printf("%f\n",x[i]);
  
    
  return 0;
}

