#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *callback_fn(void *args)
{
    const char *input_string = (char *)args;

    printf("%s \n", input_string);
    printf("Bugger 1,2 down. Mission completed. Retuning\n");

    return NULL;
}

pthread_t create_thread()
{
    pthread_t thread1;

    const char *input_to_thread = "Hello, I am a thread1";
    // int local_var = 4;
    // int *input_to_thread = &local_var;

    int rc = pthread_create(&thread1, NULL, callback_fn, (void *)input_to_thread);

    if (rc != 0)
    {
        perror("thread not able to create");
        exit(EXIT_FAILURE);
    }
    return thread1;
}

int main()
{
    pthread_t thread1 = create_thread();
    printf("Main Thread waiting for child to finish\n");
    pthread_join(thread1, NULL);
    printf("Both threads have been joined. Ending execution...\n");
}