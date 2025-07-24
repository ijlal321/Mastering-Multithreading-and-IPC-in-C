#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

int arr[] = {1, 2, 3, 4, 5};

pthread_mutex_t arr_mutex;

void *printSum(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&arr_mutex);
        int sum = 0;
        for (int i = 0; i < 5; i++)
        {
            sum += arr[i];
        }
        printf("the Sum is: %d\n", sum);
        pthread_mutex_unlock(&arr_mutex);
    }
}

void *replaceArr(void *args)
{
    while (1)
    {
        pthread_mutex_lock(&arr_mutex);
        int temp = arr[0];
        arr[0] = arr[4];
        arr[4] = temp;
        pthread_mutex_unlock(&arr_mutex);
    }
}

int main()
{
    pthread_mutex_init(&arr_mutex, NULL);
    pthread_t t1, t2;

    int rc = pthread_create(&t1, NULL, printSum, NULL);
    if (rc != 0)
    {
        printf("Thread CRreating Failed");
        exit(0);
    }
    rc = pthread_create(&t2, NULL, replaceArr, NULL);
    if (rc != 0)
    {
        printf("Thread CRreating Failed");
        exit(0);
    }

    pthread_join(t1, NULL);
    pthread_join(t2, NULL);

    pthread_mutex_destroy(&arr_mutex);
    return 0;
}