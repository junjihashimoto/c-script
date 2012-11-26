#ifndef C_SCRIPT_UTIL
#define C_SCRIPT_UTIL

//#define MAIN(...) int main(int argc,char** argv){__VA_ARGS__}


template<class T>
struct Process{
  FILE *gid;
  Process(const char* cmd,const char*mode){
    gid=popen(cmd,mode);
  }
  ~Process(){
    if(gid!=NULL){
      fflush(gid);
      fclose(gid);
    }
  }
  operator FILE* (){
    return gid;
  }
};

#define POPEN(gid,rw,cmd,...) \
    FILE *gid=NULL;{ \
    char buf[1024]; \
    sprintf(buf,cmd,##__VA_ARGS__); \
    gid = popen(buf,rw); \
    if(gid==NULL) return -1; \
    }

#define PCHECK(gid,seq) if(!(seq)){pclose(gid);return -1;}

#define systemf(...) do{ \
 char systemf_buffer[1024]; \
 snprintf(systemf_buffer,sizeof(systemf_buffer),__VA_ARGS__); \
 system(systemf_buffer); \
}while(0)


#endif

