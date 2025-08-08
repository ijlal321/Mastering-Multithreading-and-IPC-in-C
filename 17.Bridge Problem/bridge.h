#ifndef _BRIDGE_H_
#define _BRIDGE_H_

#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>


/*
    Todays Agends. 3 bridge problem. 1 bridge. Max 3 cars in 1 direction only.
    if 1 car go outside, then other car if inthis direction if available
    when last car go, then send 1 car from opposite side

    this can be directly applied to reader writer
*/


typedef enum {
    Read = 1,
    Write,
} Direction;

typedef struct {
    int nr_read_wait;
    int nr_write_wait;
    int cur_bridge;
    Direction cur_direction;
    pthread_mutex_t mutex;
    pthread_cond_t cv_read;
    pthread_cond_t cv_write;
} Bridge;

void bridge_init(Bridge * bridge);
void bridge_destroy(Bridge * bridge);

void bridge_enter(Bridge * bridge, Direction direction);
void bridge_exit(Bridge * bridge);



#endif