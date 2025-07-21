#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>




int main()
{
    printf("100");      
    sleep(1);
    printf("200 \n");
    sleep(1);
    printf("300");
    sleep(1);
}