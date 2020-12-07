#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define NTHREADS  2     //total de threads a serem criadas
#define VECTORSIZE 93   //tamanho do vetor

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread, nThreads;
   int* array;          // ponteiro que vai ser usado pro vetor
} t_Args;

//funcao executada pelas threads
void *incrementa (void *arg) {
  t_Args *args = (t_Args *) arg;
  int* arr = args->array;

// VECTORSIZE / NTHREADS é a divisão do vetor de forma genérica,
// de acordo com a quantidade de threads 
// (tamanho do vetor dividido pelo numero de threads) 
  for ( int i=0 ; i < VECTORSIZE / NTHREADS ; i++ ) {
    int k = args->idThread * VECTORSIZE / NTHREADS;   // k = numero da thread * tamanho do particionamento do vetor
    int aux = arr[i + k];                             // variavel auxiliar para mostrar o valor original do conteudo do vetor
    arr[i + k] += 1;
    printf("Thread %d incrementa : %d->%d\n", args->idThread+1, aux, arr[i+k]); // mostra a operação que cada thread está realizando 
  }

  // tratamento para o caso do vetor ter um particionamento ímpar
  if (VECTORSIZE % 2 == 1 && args->idThread % 2 == 1) {
    arr[VECTORSIZE-1] += 1;
  }

  printf("Thread %d de %d criada\n", args->idThread+1, args->nThreads);
  free(arg);      //aqui pode liberar a alocação feita na main

  pthread_exit(NULL);
}

//função principal do programa
int main() {
  pthread_t tid_sistema[NTHREADS]; //identificadores das threads no sistema
  int thread;                      //variavel auxiliar
  t_Args *arg;                     //receberá os argumentos para a thread


  // Declara e preenche o vetor a ser computado com tamanho VECTORSIZE
  int vector[VECTORSIZE];
  printf("\nValores iniciais do vetor: \n\n");
  for ( int i=0 ; i<VECTORSIZE ; i++ ) { vector[i] = i+1; printf("%d | ", vector[i]); }
  printf("\n");
  printf("\n");


  for(thread=0; thread<NTHREADS; thread++) {
    arg = malloc(sizeof(t_Args));
    
    if (arg == NULL) {
      printf("--ERRO: malloc()\n"); exit(-1);
    }

    arg->idThread = thread; 
    arg->nThreads = NTHREADS;
    arg->array = vector;

    if (pthread_create(&tid_sistema[thread], NULL, incrementa, (void*) arg)) {
      printf("--ERRO: pthread_create()\n"); exit(-1);
    }
  
  }

  // Espera todas as threads terminarem
  for (thread=0; thread<NTHREADS; thread++) {
    if (pthread_join(tid_sistema[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
  }
  printf("\n");
  // Printa o resultado do vetor
  printf("Valores finais do vetor: \n\n");
  for ( int i=0 ; i<VECTORSIZE ; i++ ) { printf("%d | ", vector[i]); }
  printf("\n");

  printf("\n--Thread principal terminou\n");
  pthread_exit(NULL);
}
