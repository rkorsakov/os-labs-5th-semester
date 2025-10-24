#define _GNU_SOURCE
#include "mythread.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sched.h>
#include <sys/mman.h>
#include <unistd.h>
#include <errno.h>

typedef struct stack_node {
    void *stack;
    struct stack_node *next;
} stack_node;

static stack_node *stack_pool = NULL;

static void *get_stack_from_pool(void) {
    if (stack_pool == NULL) {
        return NULL;
    }
    stack_node *node = stack_pool;
    void *stack = node->stack;
    stack_pool = node->next;
    return stack;
}

static void return_stack_to_pool(void *stack) {
    stack_node *node = malloc(sizeof(stack_node));
    node->stack = stack;
    node->next = stack_pool;
    stack_pool = node;
}

static int thread_wrapper(void *arg) {
    mythread_t thread = (mythread_t)arg;
    thread->retval = thread->start_routine(thread->arg);
    thread->finished = 1;
    if (thread->detached){
        return_stack_to_pool(thread->stack);
    }
    return 0;
}

int mythread_create(mythread_t thread, void *(start_routine), void *arg) {
    void *stack;
    
    stack = get_stack_from_pool();
    if (stack == NULL) {
        printf("Allocated\n");
        stack = mmap(NULL, STACK_SIZE + GUARD_PAGE_SIZE, PROT_NONE, 
                    MAP_PRIVATE|MAP_ANONYMOUS|MAP_STACK, -1, 0);
        if (stack == MAP_FAILED) {
            fprintf(stderr, "error: mmap failed\n");
            return MYTHREAD_ERROR;
        }
        
        if (mprotect(stack + GUARD_PAGE_SIZE, STACK_SIZE, PROT_READ | PROT_WRITE) == -1) {
            munmap(stack, STACK_SIZE + GUARD_PAGE_SIZE);
            fprintf(stderr, "error: mprotect failed\n");
            return MYTHREAD_ERROR;
        }
    }else{
        printf("Took from stack pool\n");
    }
    thread->start_routine = start_routine;
    thread->arg = arg;
    thread->retval = NULL;
    thread->stack = stack;
    thread->joined = 0;
    thread->finished = 0;
    thread->detached = 0;
    
    void *stack_top = stack + STACK_SIZE + GUARD_PAGE_SIZE;
    int flags = CLONE_VM|CLONE_FS|CLONE_FILES|CLONE_SIGHAND|CLONE_THREAD|CLONE_SYSVSEM;
    
    pid_t tid = clone(thread_wrapper, stack_top, flags, thread);
    if (tid == -1){
        fprintf(stderr, "error: clone failed - %s\n", strerror(errno));
        munmap(stack, STACK_SIZE + GUARD_PAGE_SIZE);
        return MYTHREAD_ERROR;
    }
    
    thread->tid = tid;
    return MYTHREAD_SUCCESS;
}

int mythread_join(mythread_t thread, void **retval) {
    if (!thread) {
        return MYTHREAD_ERROR;
    }
    
    if (thread->detached) {
        fprintf(stderr, "error: cannot join detached thread\n");
        return MYTHREAD_ERROR;
    }
    
    while (!thread->finished) {
        usleep(1000);
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

int mythread_detach(mythread_t thread) {
    if (!thread) {
        return MYTHREAD_ERROR;
    }
    
    if (thread->detached) {
        return MYTHREAD_ERROR;
    }

    thread->detached = 1;

    return MYTHREAD_SUCCESS;
}
