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
  /* outras variáveis,ex. nome do titular, etc. */
  pthread_rwlock_t trinco;
} conta_t;

// Variaveis globais
conta_t c;

int depositar_dinheiro(conta_t *conta, int valor)
{
  if (valor < 0)
    return -1;

  pthread_rwlock_wrlock(&conta->trinco);
  conta->saldo += valor;
  conta->numMovimentos++;
  pthread_rwlock_unlock(&conta->trinco);
  return valor;
}

int levantar_dinheiro(conta_t *conta, int valor)
{
  if (valor < 0)
    return -1;

  pthread_rwlock_wrlock(&conta->trinco);
  if (conta->saldo >= valor)
  {
    conta->saldo -= valor;
    conta->numMovimentos++;
  }
  else
    valor = -1;
  pthread_rwlock_unlock(&conta->trinco);

  return valor;
}

void consultar_conta(conta_t *conta)
{
  int s, n;

  pthread_rwlock_rdlock(&conta->trinco);
  s = conta->saldo;
  n = conta->numMovimentos;
  pthread_rwlock_unlock(&conta->trinco);

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
  return NULL;
}

void *fnConsulta(void *arg)
{
  int m = *((int *)arg);
  for (int i = 0; i < m; i++)
  {
    consultar_conta(&c);
  }
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
  pthread_rwlock_init(&c.trinco, NULL);

  if (pthread_create(&tid[0], NULL, fnAlice, (void *)&m) != 0)
    exit(EXIT_FAILURE);
  if (pthread_create(&tid[1], NULL, fnBob, (void *)&m) != 0)
    exit(EXIT_FAILURE);
  for (int i = 2; i < 6; ++i)
    if (pthread_create(&tid[i], NULL, fnConsulta, (void *)&m) != 0)
      exit(EXIT_FAILURE);

  for (int i = 0; i < 6; ++i)
    pthread_join(tid[i], NULL);

  printf("História chegou ao fim\n");
  consultar_conta(&c);

  exit(EXIT_SUCCESS);
}