#include <stdio.h>
#include "thread.h"
int sum = 0;

void Ta()
{
    for (int i = 0; i < 100000; i++)
    {
        asm volatile(
            "lock addq $1, %0"
            : "+m"(sum)
            :
            :);
    }
}

int main()
{
    create(Ta);
    create(Ta);
    join();
    printf("sum = %d\n" ,sum);

}