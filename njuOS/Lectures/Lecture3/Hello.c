#include "thread.h"

void Ta(){
    while(1){
        printf("1");
    }
}
void Tb(){
    while(1){
        printf("2");
    }
}
void main(){
    create(Ta);
    create(Tb);
    while(1){
        printf("3");
    }
}