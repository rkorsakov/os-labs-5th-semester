#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void *mythread(void *arg)
{
	int *counter = (int *)arg;
	while (1)
	{	
		(*counter)++;
		pthread_testcancel();
	}
	return NULL;
}

int main()
{
	pthread_t tid;
	int err;
	int counter = 0;
	err = pthread_create(&tid, NULL, mythread, &counter);
	if (err)
	{
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}
	err = pthread_cancel(tid);
	sleep(3);
	if (err)
    {
        printf("main: pthread_cancel() failed: %s\n", strerror(err));
    }
    
	err = pthread_join(tid, NULL);
    if (err)
    {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
	return 0;
}