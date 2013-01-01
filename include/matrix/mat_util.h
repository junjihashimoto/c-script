#ifndef MATRIX_UTIL_H
#define MATRIX_UTIL_H
template<class T> void      read_csv(const char* file,Matrix<T>& dat,const char* format=NULL);
template<class T> void      write_csv(const char* file,const Matrix<T>& dat,const char* format=NULL);

template<class T> string    default_read_fomat();
template<class T> string    default_write_fomat();

template<>
string
default_read_fomat<double>(){
  return "%lf";
}

template<>
string
default_read_fomat<float>(){
  return "%f";
}

template<>
string
default_read_fomat<int>(){
  return "%d";
}

template<>
string
default_write_fomat<double>(){
  return "%f";
}

template<>
string
default_write_fomat<float>(){
  return "%f";
}

template<>
string
default_write_fomat<int>(){
  return "%d";
}


template<class T>
void
read_csv(const char* file,Matrix<T>& mat,const char* format){
  FILE* in=fopen(file,"rb");
  DATA dat;
  vector<vector<string> > vec;
  char* str;
  string fmat;
  if(format==NULL){
    fmat=default_read_fomat<T>();
  }else{
    fmat=format;
  }
  while((str=get_line(in,dat)))
    vec.push_back(split((string)str,","));

  fclose(in);
  
  mat.init(vec.size(),vec[0].size());
  
  mat_for(mat){
    T v;
    sscanf(vec[r][c].c_str(),fmat.c_str(),&v);
    mat(r,c)=v;
  }

}

template<class T>
void
write_csv(const char* file,const Matrix<T>& mat,const char* format=NULL){
  FILE* out=fopen(file,"wb");
  string fmat;
  if(format==NULL){
    fmat=default_write_fomat<T>();
  }else{
    fmat=format;
  }
  for(int r=0;r<mat.nr;r++)
    for(int c=0;c<mat.nc;c++){
      fprintf(out,fmat.c_str(),(T)mat(r,c));
      if(c!=mat.nc-1)
	fprintf(out,",");
      else
	fprintf(out,"\n");
    }
  fclose(out);
}

#endif

