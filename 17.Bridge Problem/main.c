#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include<stdint.h>
#include "bridge.h"



/*
    Todays Agends. 3 bridge problem. 1 bridge. Max 3 cars in 1 direction only.
    if 1 car go outside, then other car if inthis direction if available
    when last car go, then send 1 car from opposite side

    this can be directly applied to reader writer
*/


pthread_t ths[20];
Bridge b;


void * readFn(void * arg){
    int id = (int)(intptr_t)arg;
    printf("Rb %d waiting to enter\n", id);
    bridge_enter(&b, Read);
    printf("Rb %d entered\n", id);
    sleep(5);
    printf("Rb %d exit\n", id);
    bridge_exit(&b);

    return NULL;
}

void * writeFn(void * arg){
    int id = (int)(intptr_t)arg;
    printf("Wb %d waiting to enter\n", id);
    bridge_enter(&b, Write);
    printf("Wb %d entered\n", id);
    // sleep(1);
    printf("Wb %d exit\n", id);
    bridge_exit(&b);

    return NULL;
}

int main(){
    bridge_init(&b);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    int count = 0;
    int nr_sleep = 3;
    // create n read thread
    for (int i = count; i < count + nr_sleep; i++){
        pthread_create(&ths[i], &attr,readFn , (void*)(intptr_t)i);
    }
    count += nr_sleep;
    sleep(2);
    for (int i = count; i < count + 3; i++){
        pthread_create(&ths[i], &attr,writeFn , (void*)(intptr_t)i);
    }
    count += 3;


    pthread_attr_destroy(&attr);
    getchar();
    bridge_destroy(&b);

}