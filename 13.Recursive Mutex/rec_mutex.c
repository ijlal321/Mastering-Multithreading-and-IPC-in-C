#include <stdio.h>
#include <assert.h>
#include "rec_mutex.h"

void
rec_mutex_init(rec_mutex_t *rec_mutex)  {

    rec_mutex->n = 0;
    rec_mutex->locking_thread = 0;
    pthread_mutex_init(&rec_mutex->state_mutex, NULL);
    pthread_cond_init(&rec_mutex->cv, NULL);
}

void
rec_mutex_lock(rec_mutex_t *rec_mutex) {

    pthread_mutex_lock(&rec_mutex->state_mutex);
    pthread_t cur_thread = pthread_self();

    if (rec_mutex->locking_thread == 0){
        // start of a new lock
        assert(rec_mutex->n == 0);  // Sanity check again
        rec_mutex->locking_thread = cur_thread;
        rec_mutex->n = 1; 
    } else if (rec_mutex->locking_thread == cur_thread){
        // a recursive lock happened
        rec_mutex->n ++;
    }
    else {
        // New thread tries to access already blocked thread.
        while (rec_mutex->locking_thread != 0){
            pthread_cond_wait(&rec_mutex->cv, &rec_mutex->state_mutex);
        }
        // now lock is free, acquire it.
        assert(rec_mutex->n == 0);  // Sanity check again
        rec_mutex->locking_thread = cur_thread;
        rec_mutex->n = 1; 
    }

    pthread_mutex_unlock(&rec_mutex->state_mutex);
    return;
}

void
rec_mutex_unlock(rec_mutex_t *rec_mutex) {
    
    pthread_mutex_lock(&rec_mutex->state_mutex);
    pthread_t cur_thread = pthread_self();

    if (rec_mutex->locking_thread == 0){
        // error: unlock called on an unlocked mutex
        printf("Cannot Call Unlock on a non locked Mutex");
        assert(false);
    } else if (rec_mutex->locking_thread == cur_thread){
        // Decrement the recursive lock counter
        rec_mutex->n--;

        if (rec_mutex->n == 0){
            // free the lock and signal any waiting list
            rec_mutex->locking_thread = 0;
            pthread_cond_signal(&rec_mutex->cv);
        }else if (rec_mutex->n < 0){
            // logic bug: counter went negative
            printf("Current Mutex Lock count (n) cannot be less than 0");
            assert(false);
        }
        // else: partial unlock; still held by current thread — nothing more to do   
    }
    else {
        // Another thread tries to unlock some other thread lock. Throw Error
        printf("Cannot call unlock on Lock already held by other thread.");
        assert(false);
    }

    pthread_mutex_unlock(&rec_mutex->state_mutex);
}

void
rec_mutex_destroy(rec_mutex_t *rec_mutex) {

    assert(!rec_mutex->n);
    assert(!rec_mutex->locking_thread);
    pthread_cond_destroy(&rec_mutex->cv);
    pthread_mutex_destroy(&rec_mutex->state_mutex);
}