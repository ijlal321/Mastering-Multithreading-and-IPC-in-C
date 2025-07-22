#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>
#include <unistd.h>

#define Nr_threads 3

void clear_malloc(void* args){
    free(args);
}

void closeFile(void* args){
    fclose(args);
}

void *write_to_file(void *args)
{
    pthread_setcanceltype(PTHREAD_CANCEL_ASYNCHRONOUS, NULL);
    int id = *(int *)args;
    char * fileName = malloc(sizeof(char) * 30);

    FILE *fptr;

    sprintf(fileName, "./thread_%d.txt", id);
    fptr = fopen(fileName, "w");

    // free(fileName); // Not calling this here, bcz if called here, then cancelled, it would 
                        // result in double memory free. first here, then in cleanup.
    pthread_cleanup_push(clear_malloc, (void*)fileName);

    // file close cleanup
    pthread_cleanup_push(closeFile, (void*)fptr);

    // Failed Condition
    if (fptr == NULL)
    {
        printf("Error Occurred While creating a "
               "file !");
        pthread_exit(0);
    }

    while (1)
    {
        fputs("hello this is a thread\n", fptr);
        fflush(fptr);
        sleep(2);
    }

    pthread_cleanup_pop(0);
    pthread_cleanup_pop(0);

    return NULL;
}

struct ThreadWithId
{
    pthread_t thread;
    int id;
};

int main()
{
    struct ThreadWithId threads[Nr_threads];

    for (int i = 0; i < Nr_threads; i++)
    {
        threads[i].id = i;
        pthread_create(&threads[i].thread, NULL, write_to_file, &threads[i].id);
    }

    while(true){
        int choice;
        printf("enter thread to cancel[0,%d]: ", Nr_threads-1);
        scanf("%d", &choice);
        if (choice < 0 || choice >= Nr_threads){
            printf("Enter something sensible, you dofus\n");
            continue;
        }
        if (threads[choice].id == -1){
            printf("Already cleared\n");
            continue;
        }
        pthread_cancel(threads[choice].thread);
        threads[choice].id = -1;

        bool allDone = true;
        for(int i = 0 ; i < Nr_threads; i++){
            if (threads[i].id != -1){
                allDone=false;
                break;
            }
        }
        if (allDone){
            printf("Done with all the threads, bye\n");
            break;
        }
        sleep(1);
    }
    return 0;

}