#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdlib.h>

typedef struct exampleStruct
{
    char *str;
    int num;
} exampleStruct;

void *mythread(void *arg)
{
    exampleStruct *s = (exampleStruct *)arg;
    printf("exampleStruct fields:\n%s\n%d\n", s->str, s->num);
    return NULL;
}

int main()
{
    pthread_t tid;
    int err;
    
    exampleStruct s;
    s.str = "hello";
    s.num = 67;
    
    err = pthread_create(&tid, NULL, mythread, &s);
    if (err)
    {
        printf("main: pthread_create() failed: %s\n", strerror(err));
        return -1;
    }
    
    err = pthread_join(tid, NULL);
    if (err)
    {
        printf("main: pthread_join() failed: %s\n", strerror(err));
        return -1;
    }
    
    return 0;
}