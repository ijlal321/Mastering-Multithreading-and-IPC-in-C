#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h>

void *callback_fn(void *args)
{
    char *input_string = (char *)args;

    printf("Thread says %s \n", input_string);
    sleep(2);
    free(input_string);
    return NULL;
}

void create_thread(pthread_t * thread1, bool isJoinable)
{

    char *inputData = malloc(20 * sizeof(char)); // neeeds to be freed inside thread when done
    printf("Input your data Soldier: ");
    scanf("%19s", inputData);

    pthread_attr_t thread1_attr;
    
    pthread_attr_init(&thread1_attr);
    pthread_attr_setdetachstate(&thread1_attr, isJoinable ? PTHREAD_CREATE_JOINABLE : PTHREAD_CREATE_DETACHED);
    
    pthread_create(thread1, &thread1_attr, callback_fn, (void *)inputData);
    pthread_attr_destroy(&thread1_attr);
}

int main()
{
    pthread_t thread1;
    create_thread(&thread1, true);

    pthread_t thread2_detached;
    create_thread(&thread2_detached, false);

    pthread_join(thread1, NULL);

    pthread_exit(0);
}