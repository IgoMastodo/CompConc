#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>
#include <semaphore.h>

#define NTHREADS 4

// Variaveis globais
int x = 0;                //variavel compartilhada entre as threads usada como controle para a execucao da ultima thread
sem_t condt2t3, condt1;     //semaforos para sincronizar a ordem de execucao das threads

//funcao executada pela thread 1
void *t1 (void *threadid) {
  int *tid = (int*) threadid;
  sem_wait(&condt1); //espera T2 e T3 executar
  printf("Volte sempre!\n");
  pthread_exit(NULL);
}

//funcao executada pela thread 2
void *t2 (void *threadid) {
  int *tid = (int*) threadid;
  sem_wait(&condt2t3); //espera T1 executar
  printf("Sente-se por favor.\n");
  x++;
  sem_post(&condt2t3);	//libera T3 para executar, caso seja a primeira a rodar
  if(x==2) sem_post(&condt1);  //saida da secao critica, libera a ultima thread caso a thread 3 ja tenha rodado antes
  pthread_exit(NULL);
}

//funcao executada pela thread 3
void *t3 (void *threadid) {
  int *tid = (int*) threadid;
  sem_wait(&condt2t3); //espera T1 executar
  printf("Fique a vontade.\n");
  x++;
  sem_post(&condt2t3);//libera T2 para executar, caso seja a primeira a rodar
  if(x==2) sem_post(&condt1); //saida da secao critica, libera a ultima thread caso a thread 2 ja tenha rodado antes
  pthread_exit(NULL);
}

//funcao executada pela thread 4
void *t4 (void *threadid) {
  int *tid = (int*) threadid;
  printf("Seja bem-vindo!\n");
  sem_post(&condt2t3);//libera T2 ou T3 para rodar, dependendo da ordem da fila
  pthread_exit(NULL);
}

//funcao principal
int main(int argc, char *argv[]) {
  pthread_t tid[NTHREADS];
  int *id[3], t;

  for (t=0; t<NTHREADS; t++) {
    if ((id[t] = malloc(sizeof(int))) == NULL) {
       pthread_exit(NULL); return 1;
    }
    *id[t] = t+1;
  }

  //inicia os semaforos
  sem_init(&condt2t3, 0, 0);
  sem_init(&condt1, 0, 0);

  //cria as tres threads
  if (pthread_create(&tid[1], NULL, t2, (void *)id[1])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[2], NULL, t3, (void *)id[2])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[3], NULL, t4, (void *)id[3])) { printf("--ERRO: pthread_create()\n"); exit(-1); }
  if (pthread_create(&tid[0], NULL, t1, (void *)id[0])) { printf("--ERRO: pthread_create()\n"); exit(-1); }


  //--espera todas as threads terminarem
  for (t=0; t<NTHREADS; t++) {
    if (pthread_join(tid[t], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
    } 
    free(id[t]);
  } 
  pthread_exit(NULL);
}

