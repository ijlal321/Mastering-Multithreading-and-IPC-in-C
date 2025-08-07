#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "thread_pause.h"
#include <unistd.h>

#if 0

th_p is thread which will wait on request of thread on main.

#endif

thread_pause_struct_t thread_pauser;

void * thread_p_fn(void *){
    printf("Thread_p started. Begin waiting\n");
    sleep(2);
    printf("Thread_p wait finished. testing pause\n");

    thread_test_and_pause(&thread_pauser);

    printf("Thread_p got resumed\n");

    return NULL;
}

int main (){
    thread_pause_struct_init(&thread_pauser);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
    pthread_t th_p; // this will be paused.
    pthread_create(&th_p, &attr, thread_p_fn, NULL);

    // asking thread to wait.
    printf("Main_p: waiting for th_p to pause\n");
    thread_pause(&thread_pauser);
    printf("Main_p: Thread paused. 5s Waiting\n");

    sleep(2);

    printf("Main_p: Asking th_p to resume \n");
    thread_resume(&thread_pauser);
    printf("Main_p: Thread_p resumed \n");

    pthread_exit(0);

}
