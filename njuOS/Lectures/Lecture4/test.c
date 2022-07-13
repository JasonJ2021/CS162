#include "thread.h"

int *ptr = NULL;

void Ta(int ppid){
    int a  = 100;
    int *c = &a;
    ptr = c;
    while(1);
}

void Tb(int ppid){
    usleep(100);
    printf("print a int from another thread a = %d \n" , *ptr);
}


int main(){
    create(Ta);
    create(Tb);
    join();
}