#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

int sigint_received = 0;

void sigint_handler(int sig)
{
    printf("Received SIGINT signal\n");
    sigint_received = 1;
}

void *thread1(void *arg)
{
    sigset_t set;
    sigfillset(&set);
    pthread_sigmask(SIG_BLOCK, &set, NULL);
    while (1)
    {
        sleep(1);
    }
    return NULL;
}

void *thread2(void *arg)
{
    signal(SIGINT, sigint_handler);
    while (1)
    {
        if (sigint_received) {
            sigint_received = 0;
        }
    }
    return NULL;
}

void *thread3(void *arg)
{
    sigset_t wait_set;
    sigemptyset(&wait_set);
    sigaddset(&wait_set, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &wait_set, NULL);
    int sig;
    while (1)
    {
        int result = sigwait(&wait_set, &sig);
        if (result == 0 && sig == SIGQUIT)
        {
            printf("Received SIGQUIT signal\n");
        }
    }
    return NULL;
}

int main()
{
    pthread_t tid1, tid2, tid3;
    int err;

    printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());

    err = pthread_create(&tid1, NULL, thread1, NULL);
    if (err)
    {
        printf("main: pthread_create(thread1) failed: %s\n", strerror(err));
        return -1;
    }

    err = pthread_create(&tid2, NULL, thread2, NULL);
    if (err)
    {
        printf("main: pthread_create(thread2) failed: %s\n", strerror(err));
        return -1;
    }

    err = pthread_create(&tid3, NULL, thread3, NULL);
    if (err)
    {
        printf("main: pthread_create(thread3) failed: %s\n", strerror(err));
        return -1;
    }

    while (1) {
        printf("Sending SIGINT to thread2...\n");
        pthread_kill(tid2, SIGINT);
        
        sleep(2);

        printf("Sending SIGQUIT to thread3...\n");
        pthread_kill(tid3, SIGQUIT);
        
        sleep(2);
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    return 0;
}