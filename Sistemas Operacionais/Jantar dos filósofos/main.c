#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#define N 5
#define LEFT (i + N - 1) % N
#define RIGHT (i + 1) % N
#define THINKING 0
#define HUNGRY 1
#define EATING 2

pthread_mutex_t ListaFilosofos[N];
pthread_mutex_t mutex;
int estado[N];
int nfilosofo[N] = {0, 1, 2, 3, 4};

//Funções
void Pensar(int i);
void Comer(int i);
void PegarGarfo(int i);
void LargarGarfo(int i);
void *filosofo(void *num);
void Verificar(int i);

void main(void) {
  int i;
  pthread_t thread_id[N];

  for (i = 0; i < N; i++) {
    pthread_create(&thread_id[i], NULL, filosofo, &nfilosofo[i]);

    printf("Filosofo %d criado\n", i);
  }

  for (i = 0; i < N; i++)
    pthread_join(thread_id[i], NULL);
  return;
}

void Pensar(int i) {
  printf("O filosofo %d está pensando\n", i);
  sleep(rand() % 4); 
  return;
}

void Comer(int i) {
  printf("O filosofo %d está comendo\n", i);
  sleep(rand() % 8); 
  return;
}

void Verificar(int i) {
  if (estado[i] == HUNGRY && estado[LEFT] != EATING &&
      estado[RIGHT] != EATING) {
    estado[i] = EATING;
    pthread_mutex_unlock(&ListaFilosofos[i]); 
  }
}

void PegarGarfo(int i) {
  pthread_mutex_lock(&mutex); 
  estado[i] = HUNGRY;
  printf("O filosofo %d está com fome\n", i);
  Verificar(i);
  pthread_mutex_unlock(&mutex); 
  pthread_mutex_lock(&ListaFilosofos[i]); 
}

void LargarGarfo(int i) {
  pthread_mutex_lock(&mutex);
  estado[i] = THINKING;
  Verificar(LEFT);
  Verificar(RIGHT);
  pthread_mutex_unlock(&mutex); 
}

void *filosofo(void *num) {
  int *i = num;

  while (1) {
    Pensar(*i);
    PegarGarfo(*i);
    Comer(*i);
    LargarGarfo(*i);
  }
}
