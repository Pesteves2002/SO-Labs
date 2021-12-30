#include <unistd.h>
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <assert.h>

#define N 4
typedef struct
{
  int saldo;
  int numMovimentos;
  pthread_rwlock_t rwl;
  /* outras variáveis,ex. nome do titular, etc. */
} conta_t;

// Variaveis globais
conta_t c;

int depositar_dinheiro(conta_t *conta, int valor)
{

  if (valor < 0)
    return -1;
  pthread_rwlock_wrlock(&conta->rwl);

  conta->saldo += valor;
  conta->numMovimentos++;

  pthread_rwlock_wrlock(&conta->rwl);
  return valor;
}

int levantar_dinheiro(conta_t *conta, int valor)
{

  if (valor < 0)
    return -1;

  pthread_rwlock_wrlock(&conta->rwl);

  if (conta->saldo >= valor)
  {
    conta->saldo -= valor;
    conta->numMovimentos++;
  }
  else
    valor = -1;
  pthread_rwlock_unlock(&conta->rwl);

  return valor;
}

void consultar_conta(conta_t *conta)
{
  int s, n;
  pthread_rwlock_rdlock(&conta->rwl);
  s = conta->saldo;
  n = conta->numMovimentos;
  pthread_rwlock_unlock(&conta->rwl);
  printf("Consulta: saldo=%d, #movimentos=%d\n", s, n);
}

void *fnAlice(void *arg)
{
  int m = *((int *)arg);
  int total = 0;
  int r;

  for (int i = 0; i < m; i++)
  {
    r = depositar_dinheiro(&c, 1);
    if (r != -1)
      total += r;
  }

  printf("Alice depositou no total: %d\n", total);
  consultar_conta(&c);
  return NULL;
}

void *fnBob(void *arg)
{
  int m = *((int *)arg);
  int total = 0;
  int r;

  for (int i = 0; i < m; i++)
  {
    r = levantar_dinheiro(&c, 1);
    if (r != -1)
      total += r;
  }

  printf("Bob gastou no total: %d\n", total);
  consultar_conta(&c);
  return NULL;
}

int main(int argc, char **argv)
{

  pthread_t tid[6];

  int m;

  if (argc > 1)
    m = atoi(argv[1]);
  else
    m = 0;

  c.saldo = 0;
  c.numMovimentos = 0;
  pthread_rwlock_init(&c.rwl, NULL);

  if (pthread_create(&tid[0], NULL, fnAlice, (void *)&m) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create(&tid[1], NULL, fnBob, (void *)&m) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create(&tid[2], NULL, (void *)consultar_conta, (conta_t *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create(&tid[3], NULL, (void *)consultar_conta, (conta_t *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create(&tid[4], NULL, (void *)consultar_conta, (conta_t *)&c) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create(&tid[5], NULL, (void *)consultar_conta, (conta_t *)&c) != 0)
    exit(EXIT_FAILURE);

  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);
  pthread_join(tid[3], NULL);
  pthread_join(tid[4], NULL);
  pthread_join(tid[5], NULL);

  printf("História chegou ao fim\n");
  consultar_conta(&c);
  pthread_rwlock_destroy(&c.rwl);

  exit(EXIT_SUCCESS);
}
