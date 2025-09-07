#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void cleanup_handler(void *arg)
{
	char **str_ptr = (char **)arg;
	if (*str_ptr != NULL)
	{
		free(*str_ptr);
		*str_ptr = NULL;
	}
}

void *mythread(void *arg)
{
	char *string = malloc(strlen("hello world") + 1);
	strcpy(string, "hello world");
	pthread_cleanup_push(cleanup_handler, &string);
	while (1)
	{
		pthread_cancel(pthread_self());
		printf("%s\n", string);
	}
	pthread_cleanup_pop(0);
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
	pthread_join(tid, NULL);
	return 0;
}