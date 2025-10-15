#define _GNU_SOURCE
#include "mythread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <signal.h>
#include <linux/sched.h> 
#include <linux/futex.h>
#include <sys/mman.h>
#include <sys/wait.h>
#include <sys/syscall.h>
#include <stdint.h>
#include <errno.h>

static int thread_function(void *arg) {
    mythread_t thread = (mythread_t)arg;
    thread->retval = thread->start_routine(thread->arg);
    thread->finished = 1;
    return 0;
}

int mythread_create(mythread_t thread, void *(start_routine), void *arg) {
    int total_size = STACK_SIZE + GUARD_PAGE_SIZE;
    void *stack = mmap(NULL, total_size, PROT_NONE, MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
    if (stack == MAP_FAILED) {
        fprintf(stderr, "error: mmap failed - %s\n", strerror(errno));
        return MYTHREAD_ERROR;
    }
    thread->start_routine = start_routine;
    thread->arg = arg;
    thread->retval = NULL;
    thread->stack = stack;
    thread->joined = 0;
    thread->finished = 0;
    int res = mprotect(stack + GUARD_PAGE_SIZE, STACK_SIZE, PROT_READ | PROT_WRITE);
    if (res == -1){
        fprintf(stderr, "error: mprotect failed - %s\n", strerror(errno));
        munmap(stack, total_size);
        return MYTHREAD_ERROR;
    }
    void *stack_top = stack + total_size;
    int flags = CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM|CLONE_PARENT_SETTID|CLONE_CHILD_CLEARTID;
    pid_t pid = clone(thread_function, stack_top, flags, thread);
    if (pid == -1){
        fprintf(stderr, "error: clone failed - %s\n", strerror(errno));
        munmap(stack, total_size);
        return MYTHREAD_ERROR;
    }
    thread->tid = pid;
    return MYTHREAD_SUCCESS;
}

int mythread_join(mythread_t thread, void **retval) {
    if (!thread) {
        fprintf(stderr, "error: thread is NULL\n");
        return MYTHREAD_ERROR;
    }
    
    if (thread->joined) {
        fprintf(stderr, "error: thread already joined\n");
        return MYTHREAD_ERROR;
    }
    
    while (!thread->finished) {
        usleep(100);
    }
    
    if (retval != NULL) {
        *retval = thread->retval;
    }
    
    if (thread->stack) {
        munmap(thread->stack, STACK_SIZE + GUARD_PAGE_SIZE);
        thread->stack = NULL;
    }
    
    thread->joined = 1;
    
    return MYTHREAD_SUCCESS;
}