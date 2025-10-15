#include "../include/mythread.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

void *thread1(void* arg) {
    printf("working...\n");    
    usleep(3000); 
    return NULL; 
}

void *thread2(void* arg){
    char *a = malloc(strlen("hello world") + 1);
	strcpy(a, "hello world");
	return a;
}

int main() {
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
    
    printf("2. Hello world on heap test:\n" );
    
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
    return 0;
}