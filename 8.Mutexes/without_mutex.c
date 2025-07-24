#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>



// here we would get printed 11, 15, 19.
// bcz sometimes array is swapped when we compute.

int arr[] = {1, 2, 3, 4, 5};

void *printSum(void *args)
{
    while (1)
    {
        int sum = 0;
        for (int i = 0; i < 5; i++)
        {
            sum += arr[i];
        }
        printf("the Sum is: %d\n", sum);
    }
}

void *replaceArr(void *args)
{
    while (1)
    {
        int temp = arr[0];
        arr[0] = arr[4];
        arr[4] = temp;
    }
}

int main()
{
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
}