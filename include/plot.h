#define B(x,...) boost::bind(x, __VA_ARGS__)
#define X _1
#define Y _2



const double (*NULLFUNC)(double)=(const double(*)(double))NULL;
const double (*NULLFUNC3D)(double,double)=(const double(*)(double,double))NULL;
typedef double (*F1)(double);
typedef double (*F2)(double,double);

#define PLOT2D_PRINT(num)				\
  if(typeid(f##num)!=typeid(NULLFUNC)){			\
    for(double i=start;i<end;i+=step)			\
      fprintf(tf,"%f %f\n",i,(double)f##num(i));	\
    fprintf(tf,"\n");					\
  }

#define SPLOT_PRINT(num)					\
  if(typeid(f##num)!=typeid(NULLFUNC3D)){			\
    for(double x=x_start;x<x_end;x+=x_step){			\
      for(double y=y_start;y<y_end;y+=y_step)			\
	fprintf(tf,"%f %f %f\n",x,y,(double)f##num(x,y));	\
      fprintf(tf,"\n");						\
    }								\
    fprintf(tf,"\n");						\
  }

#define PLOT2D_ARG				\
  double start,double end,			\
    string opt0="",				\
    string opt1="with lines",			\
    double grid=1000

#define SPLOT_ARG				\
  double x_start,double x_end,			\
    double y_start,double y_end,		\
    string opt0="",				\
    string opt1="using 1:2:3 with pm3d",	\
    double grid=200



template<class Func0,
	 class Func1,
	 class Func2,
	 class Func3,
	 class Func4,
	 class Func5,
	 class Func6,
	 class Func7,
	 class Func8
	 >
void 
plot2d(Func0 f0,
       Func1 f1,
       Func2 f2,
       Func3 f3,
       Func4 f4,
       Func5 f5,
       Func6 f6,
       Func7 f7,
       Func8 f8,
       PLOT2D_ARG){
  FILE *gid;
  gid = popen("gnuplot -persist", "w");
  FILE *tf=fopen("my.dat","w+");
  
  double step=(end-start)/grid;
  
  PLOT2D_PRINT(0);
  PLOT2D_PRINT(1);
  PLOT2D_PRINT(2);
  PLOT2D_PRINT(3);
  PLOT2D_PRINT(4);
  PLOT2D_PRINT(5);
  PLOT2D_PRINT(6);
  PLOT2D_PRINT(7);
  PLOT2D_PRINT(8);
  
  fclose(tf);

  fprintf(gid, "set xrange [%f:%f]\n",start,end);
  fprintf(gid, "%s\n",opt0.c_str());
  fprintf(gid, "plot 'my.dat' %s \n",opt1.c_str());
  //  fprintf(gid, "pause -1 Press Return\n");
  fflush(gid);
#ifdef  __CYGWIN__
  cin.get();
#endif
  
  pclose(gid);
  remove("my.dat");

}

template<class Func0,
	 class Func1,
	 class Func2,
	 class Func3,
	 class Func4,
	 class Func5,
	 class Func6,
	 class Func7
	 >
void 
plot2d(Func0 f0,
       Func1 f1,
       Func2 f2,
       Func3 f3,
       Func4 f4,
       Func5 f5,
       Func6 f6,
       Func7 f7,
       PLOT2D_ARG){
  plot2d(f0,
	 f1,
	 f2,
	 f3,
	 f4,
	 f5,
	 f6,
	 f7,
	 NULLFUNC,
	 start,end,opt0,opt1,grid);
}

template<class Func0,
	 class Func1,
	 class Func2,
	 class Func3,
	 class Func4,
	 class Func5,
	 class Func6
	 >
void 
plot2d(Func0 f0,
       Func1 f1,
       Func2 f2,
       Func3 f3,
       Func4 f4,
       Func5 f5,
       Func6 f6,
       PLOT2D_ARG){
  plot2d(f0,
	 f1,
	 f2,
	 f3,
	 f4,
	 f5,
	 f6,
	 NULLFUNC,
	 NULLFUNC,
	 start,end,opt0,opt1,grid);
}

template<class Func0,
	 class Func1,
	 class Func2,
	 class Func3,
	 class Func4,
	 class Func5
	 >
void 
plot2d(Func0 f0,
       Func1 f1,
       Func2 f2,
       Func3 f3,
       Func4 f4,
       Func5 f5,
       PLOT2D_ARG){
  plot2d(f0,
	 f1,
	 f2,
	 f3,
	 f4,
	 f5,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 start,end,opt0,opt1,grid);
}


template<class Func0,
	 class Func1,
	 class Func2,
	 class Func3,
	 class Func4
	 >
void 
plot2d(Func0 f0,
       Func1 f1,
       Func2 f2,
       Func3 f3,
       Func4 f4,
       PLOT2D_ARG){
  plot2d(f0,
	 f1,
	 f2,
	 f3,
	 f4,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 start,end,opt0,opt1,grid);
}

template<class Func0,
	 class Func1,
	 class Func2,
	 class Func3
	 >
void 
plot2d(Func0 f0,
       Func1 f1,
       Func2 f2,
       Func3 f3,
       PLOT2D_ARG){
  plot2d(f0,
	 f1,
	 f2,
	 f3,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 start,end,opt0,opt1,grid);
}

template<class Func0,
	 class Func1,
	 class Func2
	 >
void 
plot2d(Func0 f0,
       Func1 f1,
       Func2 f2,
       PLOT2D_ARG){
  plot2d(f0,
	 f1,
	 f2,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 start,end,opt0,opt1,grid);
}

template<class Func0,
	 class Func1
	 >
void 
plot2d(Func0 f0,
       Func1 f1,
       PLOT2D_ARG){
  plot2d(f0,
	 f1,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 start,end,opt0,opt1,grid);
}


template<class Func0
	 >
void 
plot2d(Func0 f0,
       PLOT2D_ARG){
  plot2d(f0,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 NULLFUNC,
	 start,end,opt0,opt1,grid);
}


template<class Func0,
	 class Func1,
	 class Func2
	 >
void 
splot(Func0 f0,
      Func1 f1,
      Func2 f2,
      SPLOT_ARG
      ){
  FILE *gid;
  gid = popen("gnuplot -persist", "w");
  FILE *tf=fopen("my.dat","w+");
  
  double x_step=(x_end-x_start)/grid;
  double y_step=(y_end-y_start)/grid;
  
  SPLOT_PRINT(0);
  SPLOT_PRINT(1);
  SPLOT_PRINT(2);
  
  fclose(tf);

  fprintf(gid, "set pm3d\n");
  fprintf(gid, "set xrange [%f:%f]\n",x_start,x_end);
  fprintf(gid, "set yrange [%f:%f]\n",y_start,y_end);
  fprintf(gid, "%s\n",opt0.c_str());
  fprintf(gid, "splot 'my.dat' %s \n",opt1.c_str());
  fflush(gid);
#ifdef  __CYGWIN__
  cin.get();
#endif
  pclose(gid);

  remove("my.dat");

}


template<class Func0,
	 class Func1
	 >
void 
splot(Func0 f0,
      Func1 f1,
      SPLOT_ARG
      ){
  splot(f0,
	f1,
	NULLFUNC3D,
	x_start,x_end,
	y_start,y_end,
	opt0,opt1,grid);
}


template<class Func0
	 >
void 
splot(Func0 f0,
      SPLOT_ARG
      ){
  splot(f0,
	NULLFUNC3D,
	NULLFUNC3D,
	x_start,x_end,
	y_start,y_end,
	opt0,opt1,grid);
}


template<class T>
void 
histgram(T* f,int size,string opt0="",string opt1="with boxes"){
  FILE *gid;
  gid = popen("gnuplot -persist", "w");
  FILE *tf=fopen("my.dat","w+");

  for(int i=0;i<size;i++)
    fprintf(tf,"%d\n",(int)f[i]);			\
  
  fclose(tf);

  fprintf(gid, "%s\n",opt0.c_str());
  fprintf(gid, "plot 'my.dat' %s \n",opt1.c_str());
  //  fprintf(gid, "pause -1 Press Return\n");
  fflush(gid);
#ifdef  __CYGWIN__
  cin.get();
#endif
  
  pclose(gid);
  remove("my.dat");

}
