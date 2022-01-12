#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

int value = 0;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

void *thr_func(void *ptr)
{
  // Primeiro passo: espera ate' valor ser positivo
  if (pthread_mutex_lock(&mutex) != 0)
    exit(EXIT_FAILURE);

  while (value == 0)
    pthread_cond_wait(&cond, &mutex);

  // Segundo passo: modifica o valor
  printf("[Thread nova] Leu value=%d, vai incrementa'-lo\n", value);
  value++;

  if (pthread_mutex_unlock(&mutex) != 0)
    exit(EXIT_FAILURE);

  // Finalmente, termina a thread
  return NULL;
}

int main()
{
  pthread_t tid;

  // Cria nova thread
  if (pthread_create(&tid, NULL, thr_func, NULL) != 0)
  {
    printf("Error creating thread.\n");
    return -1;
  }

  // Atraso artificial
  sleep(5);

  // Incrementa o valor (permitindo 'a outra tarefa sair da sua espera)
  if (pthread_mutex_lock(&mutex) != 0)
    exit(EXIT_FAILURE);

  value++;

  pthread_cond_signal(&cond);

  if (pthread_mutex_unlock(&mutex) != 0)
    exit(EXIT_FAILURE);

  // Aguarda pela terminacao da outra tarefa
  if (pthread_join(tid, NULL) != 0)
    exit(EXIT_FAILURE);

  if (pthread_mutex_lock(&mutex) != 0)
    exit(EXIT_FAILURE);
  printf("[Thread inicial] Value=%d\n", value);
  if (pthread_mutex_unlock(&mutex) != 0)
    exit(EXIT_FAILURE);

  return EXIT_SUCCESS;
}
