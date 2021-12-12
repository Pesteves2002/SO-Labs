/*
 * thread.c - simple example demonstrating the creation of threads
 */

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

int Value = 0;

void *thr_func(void *ptr)
{
   Value = 1;
   return NULL;
}

int main()
{
   pthread_t tid;
   if (pthread_create(&tid, NULL, thr_func, NULL) != 0)
   {
      printf("Error creating thread.\n");
      return -1;
   }
   sleep(1);

   Value = 2;

   if (pthread_join(tid, NULL) != 0)
   {
      printf("Error joining thread.\n");
      return -1;
   }

   printf("Value=%d\n", Value);
   return 0;
}
