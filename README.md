c-script
========

c-script provides the function of script language for c++.

This is made for throwaway, short, and fast programs.  
This gives weight to runtime speed for solving image processing problems.

You needs only script to run your program.  
Makefile and other build tool are not needed.

This supports following functions.

1. Shebang
2. Cache    : When compiled once, executable cache is used the next time.
3. Debugger(gdb,ddd,emacs,VC++) and reasonable compile error message.
4. #pragma  : Compilation option is controlled by #pragma.
5. Image manipulation : This can read and write bmp, jpg and png.
6. Matrix function like eigen: This uses blas and lapack.
7. Drawing graph funtion by gnuplot
8. c++, cuda, c, obj-c and c# are supported.

Install
-------
Set path for root directory of this program.  
export PATH=$PATH:program_root

How to run script
-----------------
Case1:
>c-script my_cpp_program.cpp

Case2:  
1.Write script  
For example,
~~~~~~
#!/usr/bin/env c-script
int
main(){
  printf("hello world\n");
  return 0;
}
~~~~~~

2.Chmod
Type
>chmod +x above_cpp_program.cpp

3.Run
Type
>./above_cpp_program.cpp

Command line options
--------------------
~~~~~~
Usage: c-script [options] script-file [script-options]
 -nomain  insert main-function automatically
 -nocache compile the program everytime
 -d       run the program under the gdb
 -gdb     run the program under the gdb
 -ddd     run the program under the ddd
 -vc      run the program under the visual studio
 -show    output code after preprocess
 -cmd     output compiler-options
 -clean   remove cache of both exe and source
~~~~~~


Pragmas
-------
~~~~~~
#pragma c-script:src  "file1" "file2" ..
 compile with "file1" "file2" ..
#pragma c-script:opt  "additional compile options"
 add compile options 
#pragma c-script:eval "Run perl-expression"
 
#pragma c-script:do   "Run perl-script"
#pragma c-script:cc   "compiler command name(default:g++)"
#pragma c-script:libs "liked library options"
#pragma c-script:no_default_header
~~~~~~


Files for this system
---------------------
~~~~~~
init/init.pl
init/suffix_cu.pl
init/suffix_cs.pl
init/suffix_m.pl
$HOME/init.pl
$HOME/.c-script/cache
~~~~~~



Compilation and execution flow
------------------------------

This runs following steps.
1. Read initialization script for reading compilation paramater.
2. Generate compiled code (For example, header like '#include' is added.)
3. Compile 
4. Run complied binary
