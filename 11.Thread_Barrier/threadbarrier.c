
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "threadbarrier.h"

void thread_barrier_print(th_barrier_t *th_barrier)
{

    printf("th_barrier->threshold_count = %u\n", th_barrier->threshold_count);
    printf("th_barrier->curr_wait_count = %u\n", th_barrier->curr_wait_count);
    printf("th_barrier->is_ready_again = %s\n", th_barrier->is_ready_again ? "true" : "false");
}

void thread_barrier_init(th_barrier_t *barrier, uint32_t threshold_count)
{

    barrier->threshold_count = threshold_count;
    barrier->curr_wait_count = 0;
    pthread_cond_init(&barrier->cv, NULL);
    pthread_mutex_init(&barrier->mutex, NULL);
    barrier->is_ready_again = true;
    pthread_cond_init(&barrier->busy_cv, NULL);
}

void thread_barrier_wait(th_barrier_t *barrier, char * id)
{
    printf("%s entered\n", id);

    pthread_mutex_lock(&barrier->mutex);

    while (barrier->is_ready_again != true)
    {
        printf("%s is waiting for new list \n", id);
        pthread_cond_wait(&barrier->busy_cv, &barrier->mutex);
        printf("%s started in new list\n", id);
    }

    barrier->curr_wait_count++;

    if (barrier->curr_wait_count > barrier->threshold_count)
    {
        fprintf(stderr, "Error: curr_wait_count exceeded threshold_count. Should not have reached this point.\n");
        exit(0);
    }
    else if (barrier->curr_wait_count < barrier->threshold_count)
    {
        printf("%s is waiting in old list\n", id);
        pthread_cond_wait(&barrier->cv, &barrier->mutex);
        printf("%s is started moving in old list\n", id);
    }
    else
    {
        barrier->is_ready_again = false; // make new threads wait.
        printf("%s stopped new threads from entering list \n", id);
    }

    barrier->curr_wait_count--;
    printf("Current wait count is %d\n", barrier->curr_wait_count);

    if (barrier->curr_wait_count == 0)
    {
        // time to make a new wait
        barrier->is_ready_again = true;
        pthread_cond_broadcast(&barrier->busy_cv); // v imp. Need to broadcast here, bcz no relay here.
                        // need to tell every thread in busycv waiting to move forward.
        printf("%s signaled to make new list\n", id);
    }
    else
    {
        pthread_cond_signal(&barrier->cv);
        printf("%s signaled to continue old list\n", id);
    }
    pthread_mutex_unlock(&barrier->mutex);
}
