#!/home/junji/c-script/c-script -nomain

struct test{
  static void myfunc(){
    printf ("call %s\n",__func__);
  }
};
//aa
printf("hello\n");
test::myfunc();
