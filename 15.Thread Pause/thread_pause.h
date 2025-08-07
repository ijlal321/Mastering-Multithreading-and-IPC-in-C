#ifndef __TH_PAUSE__
#define __TH_PAUSE__

#include <stdbool.h>
#include <pthread.h>

typedef struct thread_pause_struct_ {

    pthread_mutex_t state_mutex;
    pthread_cond_t pause_cv; // (thread to be paused - will wait using this cv)
    bool should_pause;  // (when a thread wants to pause this thread, it will set it to true and wait till that thread gets paused.)
    pthread_cond_t notif_cv; //(thread who is pausing will wait on this, pausing thread will send a signal to it, saying i am paused)
} thread_pause_struct_t;

void
thread_pause_struct_init (thread_pause_struct_t *thread_pause_struct);

void
thread_pause(thread_pause_struct_t *thread_pause_struct);

void
thread_resume(thread_pause_struct_t *thread_pause_struct);

void
thread_test_and_pause(thread_pause_struct_t *thread_pause_struct);

void
thread_destroy_pause_struct(thread_pause_struct_t *thread_pause_struct);

#endif 