/*
 * thread.c - simple example demonstrating the creation of threads
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
int Value = 0;

void *thr_func(void *ptr)
{
    Value = 1;
    return NULL;
}

int main()
{
    pthread_t tid;
    if (fork() != 0)
    {
        printf("Error creating thread.\n");
        return -1;
    }

    Value = 2;

    if (wait(&tid) != 0)
    {
        printf("Error joining thread.\n");
        return -1;
    }

    sleep(1);

    printf("Value=%d\n", Value);
    return 0;
}
