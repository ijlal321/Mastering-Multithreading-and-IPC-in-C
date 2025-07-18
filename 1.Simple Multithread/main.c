#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <unistd.h>

void* callback_fn (void* args){
    const char* input_string = (char*)args;

    printf("%s", input_string);
    return NULL;
}

void create_thread(){
    pthread_t thread1;

    static const char* input_to_thread = "Hello, I am a thread1";

    int rc = pthread_create(&thread1, NULL, callback_fn, (void*)input_to_thread);

    if (rc != 0){
        perror("thread not able to create");
        exit(EXIT_FAILURE);
    }
    pthread_join(thread1, NULL);
}


int main (){

    create_thread();
    
}