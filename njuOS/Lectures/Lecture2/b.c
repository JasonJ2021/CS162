#include <stdio.h>
#include <windows.h>



int main(){
    syscall(_exit,42);
}