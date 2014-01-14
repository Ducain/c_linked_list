
@ECHO OFF

ECHO Compiling program...

gcc -std=c99 -o linked_list.exe linked_list.c

ECHO Compile complete

ECHO Press any key to run program
Pause > nul

::run
hello.exe

cmd