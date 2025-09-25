#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <sys/types.h>
#include <unistd.h>

int global_var = 5;

void *mythread(void *arg) {
    static int static_var = 5;
    int local_var = 5;
    const int const_var = 5;
    local_var++;
    global_var++;
    printf("mythread [%d %d %d %ld]: Hello from mythread!\n", getpid(), getppid(), gettid(), pthread_self());
    printf("local: %p, static: %p, const: %p, global: %p\n", 
           (void*)&local_var, (void*)&static_var, (void*)&const_var, (void*)&global_var);
    printf("local value: %d, global value: %d\n", local_var, global_var);
	sleep(1000);
    return NULL;
}

int main() {
    pthread_t tidArray[5];
    int err;
    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    
    for (int i = 0; i < 5; i++) {
        err = pthread_create(&tidArray[i], NULL, mythread, NULL);
        if (err) {
            printf("main: pthread_create() failed: %s\n", strerror(err));
            return -1;
        }
    }
    
    for (int i = 0; i < 5; i++) {
        err = pthread_join(tidArray[i], NULL);
        if (err) {
            printf("main: pthread_join() failed for thread %d: %s\n", i, strerror(err));
            return -1;
        }
    }
    
    return 0;
}