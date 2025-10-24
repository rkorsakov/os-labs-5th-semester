#include "../include/mythread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void *thread1(void* arg)
{
    printf("working...\n");
    sleep(1);
    return NULL;
}

void *thread2(void* arg)
{
    char *a = malloc(strlen("hello world") + 1);
    strcpy(a, "hello world");
    return a;
}

void *thread3(void* arg)
{
    int *a = (int*)arg;
    printf("%d\n", *a);
    return NULL;
}

void *thread4(void* arg)
{
    int *id = (int*)arg;
    printf("Thread %d working\n", *id);
    return NULL;
}

void *thread5(void* arg)
{
    printf("hi\n");
    return NULL;
}

int main()
{
    printf("=== MyThread Test ===\n\n");
    
    printf("1. Default test:\n");
    mythread t1;
    int result = mythread_create(&t1, thread1, NULL);
    
    if (result == MYTHREAD_ERROR) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    
    if (mythread_join(&t1, NULL) == MYTHREAD_ERROR) {
        fprintf(stderr, "Error joining thread\n");
        return 1;
    }
    
    printf("2. Hello world test:\n");
    
    mythread t2;
    result = mythread_create(&t2, thread2, NULL);
    
    if (result == MYTHREAD_ERROR) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    
    char *res;
    if (mythread_join(&t2, (void**)&res) == MYTHREAD_ERROR) {
        fprintf(stderr, "Error joining thread\n");
        return 1;
    }
    
    printf("%s\n", res);
    free(res);

    printf("3. Passing value test:\n");

    mythread t3;
    int a = 60;
    result = mythread_create(&t3, thread3, &a);
    
    if (result == MYTHREAD_ERROR) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    
    if (mythread_join(&t3, NULL) == MYTHREAD_ERROR) {
        fprintf(stderr, "Error joining thread\n");
        return 1;
    }
    
    printf("4. 100 threads test:\n");
    mythread threads[100];
    int thread_args[100];
    for (int i = 0; i < 100; i++) {
        thread_args[i] = i;
        result = mythread_create(&threads[i], thread4, &thread_args[i]);
        if (result == MYTHREAD_ERROR) {
            fprintf(stderr, "Error creating thread %d\n", i);
            return 1;
        }
    }
    for (int i = 0; i < 100; i++) {
        if (mythread_join(&threads[i], NULL) == MYTHREAD_ERROR) {
            fprintf(stderr, "Error joining thread %d\n", i);
            return 1;
        }
    }
    printf("5. Detach test:\n");
    mythread t4, t5;
	result = mythread_create(&t4, thread5, NULL);
	if (result == MYTHREAD_ERROR) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    mythread_detach(&t4);
    sleep(3);
    result = mythread_create(&t5, thread5, NULL);
    if (result == MYTHREAD_ERROR) {
        fprintf(stderr, "Error creating thread\n");
        return 1;
    }
    mythread_detach(&t5);
    sleep(3);
    return 0;
}