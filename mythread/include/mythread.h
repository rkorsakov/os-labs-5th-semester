#ifndef MYTHREAD_H
#define MYTHREAD_H

#include <sys/types.h>

#define MYTHREAD_SUCCESS 0
#define MYTHREAD_ERROR -1
#define STACK_SIZE (1024 * 1024)
#define GUARD_PAGE_SIZE (4096)

typedef struct {
    pid_t tid;
    void *(*start_routine)(void *);
    void *arg;
    void *retval;
    void *stack;
    int finished;
    int joined;
} mythread;

typedef mythread *mythread_t;

int mythread_create(mythread_t thread, void *(start_routine), void *arg);
int mythread_join(mythread_t thread, void **retval);

#endif
