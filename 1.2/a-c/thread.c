#define _GNU_SOURCE
#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>

void *mythread(void *arg) {
	char *a = malloc(strlen("hello world") + 1);
	strcpy(a, "hello world");
	return a;
}

int main() {
	pthread_t tid;
	int err;
	printf("main [%d %d %d]: Hello from main!\n", getpid(), getppid(), gettid());
	err = pthread_create(&tid, NULL, mythread, NULL);
		if (err) {
			printf("main: pthread_create() failed: %s\n", strerror(err));
			return -1;
		}
	void *returnValue;
	pthread_join(tid, &returnValue);
	char *result = (char*)returnValue;
	printf("%s\n", result);
	free(returnValue);
	return 0;
}

