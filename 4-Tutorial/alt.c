/*
 * thread.c - simple example demonstrating the creation of threads
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdlib.h>
int Value = 0;

void *thr_func(void *ptr)
{
    Value = 1;
    printf("Value=%d\n", Value);
    return NULL;
}

int main()
{
    int s, pid;
    pid = fork();

    if (pid == -1)
    {
        printf("Error creating thread.\n");
        return -1;
    }

    if (pid == 0)
    {
        printf("Value=%d\n", Value);
    }
    else
    {

        Value = 1;
        printf("Value=%d\n", Value);
        wait(&s);
    }

    if (WIFEXITED(s))
    {
        printf("Value=%d\n", Value);
    }

    exit(EXIT_SUCCESS);
    return 0;
}
