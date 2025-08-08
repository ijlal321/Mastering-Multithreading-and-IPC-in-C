#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<pthread.h>
#include<unistd.h>
#include "bridge.h"

/* fn declarations*/

int getCurDirWait(Bridge * bridge);

int getOppositeDirWait(Bridge * bridge);

void handleBridgeWait(Direction direction, Bridge * bridge);

void incrementBridgeWait(Direction direction, Bridge * bg);

void decrementBridgeWait(Direction direction, Bridge * bg);

// ====================

void bridge_init(Bridge * bridge){
    bridge->nr_read_wait = 0;
    bridge->nr_write_wait = 0;
    bridge->cur_bridge = 0;
    
    pthread_mutex_init(&bridge->mutex, NULL);
    pthread_cond_init(&bridge->cv_read, NULL);
    pthread_cond_init(&bridge->cv_write, NULL);

}

void bridge_destroy(Bridge * bridge){
    bridge->nr_read_wait = 0;
    bridge->nr_write_wait = 0;
    bridge->cur_bridge = 0;
    
    pthread_mutex_destroy(&bridge->mutex);
    pthread_cond_destroy(&bridge->cv_read);
    pthread_cond_destroy(&bridge->cv_write);
}

void bridge_enter(Bridge * bridge, Direction direction){

    pthread_mutex_lock(&bridge->mutex);
    if (bridge->cur_bridge == 0){
        bridge->cur_direction = direction;
    }

    while (bridge->cur_direction != direction || bridge->cur_bridge == 3){
        if (direction == Read){
            bridge->nr_read_wait += 1;
            pthread_cond_wait(&bridge->cv_read, &bridge->mutex);
            bridge->nr_read_wait -= 1;
        }else{
            bridge->nr_write_wait += 1;
            pthread_cond_wait(&bridge->cv_write, &bridge->mutex);
            bridge->nr_write_wait -= 1;
        }
    }

    bridge->cur_bridge ++;
    pthread_mutex_unlock(&bridge->mutex);
    return;
    
}

void bridge_exit(Bridge * bridge){
    pthread_mutex_lock(&bridge->mutex);

    bridge->cur_bridge--;
    if (bridge->cur_bridge == 0){ // if list finished
        if (getOppositeDirWait(bridge) > 0){ // then try to start opposite dir
            if (bridge->cur_direction == Read){
                bridge->cur_direction = Write;
                pthread_cond_broadcast(&bridge->cv_write);
            }else{   
                bridge->cur_direction = Read;
                pthread_cond_broadcast(&bridge->cv_read);
            }
        } else if (getCurDirWait(bridge) != 0){ // if not then try to send from same dir
            if (bridge->cur_direction == Read){
                pthread_cond_broadcast(&bridge->cv_read);
            }else{   
                pthread_cond_broadcast(&bridge->cv_write);
            }
        }
        // else no one is waiting on both side. just do nothing then
        pthread_mutex_unlock(&bridge->mutex);
        return;
    }else if (bridge->cur_bridge < 3){ // if list still remaining
        if (bridge->cur_direction == Read){ // send same dir signal
            pthread_cond_broadcast(&bridge->cv_read);
        }else{   
            pthread_cond_broadcast(&bridge->cv_write);
        }
        pthread_mutex_unlock(&bridge->mutex);
        return;
    }else{
        printf("No else bcz cant be 3 or >3\n");
    }
    printf("unknown reeachdd\n");
}


/*  ================= Helper Functions ============*/

int getCurDirWait(Bridge * bridge){
    if (bridge->cur_direction == Read){
        return bridge->nr_read_wait;
    }else{
        return bridge->nr_write_wait;
    }
}

int getOppositeDirWait(Bridge * bridge){
    if (bridge->cur_direction == Read){
        return bridge->nr_write_wait;
    }else{
        return bridge->nr_read_wait;
    }
}

void handleBridgeWait(Direction direction, Bridge * bridge){
    printf("waiting reached\n");
    while (bridge->cur_direction != direction || bridge->cur_bridge == 3){
        incrementBridgeWait(direction, bridge);
        if (direction == Read){
            pthread_cond_wait(&bridge->cv_read, &bridge->mutex);
        }else{
            pthread_cond_wait(&bridge->cv_write, &bridge->mutex);
        }
        decrementBridgeWait(direction, bridge);
    }
    printf("waiting done\n");
}

void incrementBridgeWait(Direction direction, Bridge * bg){
    if (direction == Read){
        bg->nr_read_wait ++;
    }else{
        bg->nr_write_wait ++;
    }
}

void decrementBridgeWait(Direction direction, Bridge * bg){
    if (direction == Read){
        bg->nr_read_wait --;
    }else{
        bg->nr_write_wait --;
    }
}




