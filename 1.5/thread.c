#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void sigint_handler(int sig)
{
    printf("Received SIGINT signal\n");
}

void *block_signals_thread(void *arg)
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

void *handle_sigint_thread(void *arg)
{
    if (signal(SIGINT, sigint_handler) == SIG_ERR)
    {
        perror("signal");
        return NULL;
    }

    while (1)
    {
        sleep(1);
    }
    return NULL;
}

void *handle_sigquit_thread(void *arg)
{
    sigset_t wait_set;
    sigemptyset(&wait_set);
    sigaddset(&wait_set, SIGQUIT);
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
    sigset_t block_set;
    sigaddset(&block_set, SIGQUIT);
    pthread_sigmask(SIG_BLOCK, &block_set, NULL);
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
    err = pthread_create(&tid1, NULL, block_signals_thread, NULL);
    if (err)
    {
        printf("main: pthread_create(block_signals_thread) failed: %s\n", strerror(err));
        return -1;
    }

    err = pthread_create(&tid2, NULL, handle_sigint_thread, NULL);
    if (err)
    {
        printf("main: pthread_create(handle_sigint_thread) failed: %s\n", strerror(err));
        return -1;
    }

    err = pthread_create(&tid3, NULL, handle_sigquit_thread, NULL);
    if (err)
    {
        printf("main: pthread_create(handle_sigquit_thread) failed: %s\n", strerror(err));
        return -1;
    }

    pthread_join(tid1, NULL);
    pthread_join(tid2, NULL);
    pthread_join(tid3, NULL);

    return 0;
}