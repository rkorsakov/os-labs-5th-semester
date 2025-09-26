#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

typedef struct exampleStruct
{
	char *str;
	int num;
} exampleStruct;

void *mythread(void *arg)
{
	exampleStruct *s = (exampleStruct *)arg;
	while (1){
	printf("exampleStruct fields:\n%s\n%d\n", s->str, s->num);
}
	free(s);
	return NULL;
}

int main()
{
	pthread_t tid;
	int err;
	exampleStruct *s = malloc(sizeof(exampleStruct));
	s->str = "hello";
	s->num = 67;
	pthread_attr_t attr;
	pthread_attr_init(&attr);
	pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_DETACHED);
	err = pthread_create(&tid, &attr, mythread, s);
	pthread_attr_destroy(&attr);
	if (err)
	{
		free(s);
		printf("main: pthread_create() failed: %s\n", strerror(err));
		return -1;
	}

	return 0;
}
