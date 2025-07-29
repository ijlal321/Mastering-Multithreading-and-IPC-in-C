#include <stdio.h>
#include <pthread.h>
#include <unistd.h>  // for sleep
#include "rec_mutex.h"

rec_mutex_t mutex;

void* thread_func_simple(void* arg) {
    printf("Thread %ld: Trying to lock mutex...\n", (long)pthread_self());
    rec_mutex_lock(&mutex);
    printf("Thread %ld: Acquired lock\n", (long)pthread_self());

    sleep(1);  // simulate work

    printf("Thread %ld: Unlocking...\n", (long)pthread_self());
    rec_mutex_unlock(&mutex);
    printf("Thread %ld: Unlocked\n", (long)pthread_self());
    return NULL;
}

void* thread_func_recursive(void* arg) {
    printf("Thread %ld: Locking recursively...\n", (long)pthread_self());
    rec_mutex_lock(&mutex);
    printf("Thread %ld: First lock acquired\n", (long)pthread_self());

    rec_mutex_lock(&mutex);
    printf("Thread %ld: Second lock acquired\n", (long)pthread_self());

    sleep(1);

    printf("Thread %ld: Unlocking once...\n", (long)pthread_self());
    rec_mutex_unlock(&mutex);

    printf("Thread %ld: Unlocking again...\n", (long)pthread_self());
    rec_mutex_unlock(&mutex);

    return NULL;
}

void* thread_func_illegal_unlock(void* arg) {
    sleep(0.5);  // Ensure another thread has the lock
    printf("Thread %ld: Trying illegal unlock...\n", (long)pthread_self());
    rec_mutex_unlock(&mutex);  // This should trigger an assert
    return NULL;
}

int main() {
    printf("Initializing recursive mutex...\n");
    rec_mutex_init(&mutex);

    printf("\n=== Test 1: Simple lock/unlock ===\n");
    pthread_t t1;
    pthread_create(&t1, NULL, thread_func_simple, NULL);
    pthread_join(t1, NULL);

    printf("\n=== Test 2: Recursive locking ===\n");
    pthread_t t2;
    pthread_create(&t2, NULL, thread_func_recursive, NULL);
    pthread_join(t2, NULL);

    printf("\n=== Test 3: Illegal unlock by wrong thread ===\n");
    pthread_t t3a, t3b;
    rec_mutex_lock(&mutex);  // Lock in main thread
    pthread_create(&t3b, NULL, thread_func_illegal_unlock, NULL);
    pthread_join(t3b, NULL);  // This should assert (intentionally)
    rec_mutex_unlock(&mutex);  // Should not be reached if assert works

    printf("\nDestroying recursive mutex...\n");
    rec_mutex_destroy(&mutex);

    return 0;
}
