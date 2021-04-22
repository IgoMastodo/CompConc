#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>

#define NTHREADS  4

/* Variaveis globais */
int x = 0, aux=0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond, y_cond;

/* Thread A */
void *A (void *t) {

  printf("A: Comecei\n");
  

  printf("Seja bem vindo\n");

  pthread_mutex_lock(&x_mutex);
  x++;
  if (x==1) {
      printf("A:  x = %d, vai sinalizar a condicao \n", x);
      pthread_cond_signal(&x_cond);
  }
  pthread_mutex_unlock(&x_mutex);

  pthread_exit(NULL);
}

/* Thread B */
void *B (void *t) {
  printf("B: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  aux++;
  if (x < 1) { 
     printf("B: x= %d, vai se bloquear...\n", x);
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("B: sinal recebido e mutex realocado, x = %d\n", x);
  }
  printf("Sente-se por favor.\n");
  if(aux==2){ pthread_cond_signal(&y_cond); }
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Thread C */
void *C (void *t) {
  printf("C: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  aux++;
  if (x < 1) { 
     printf("C: x= %d, vai se bloquear...\n", x);    
     pthread_cond_wait(&x_cond, &x_mutex);
     printf("C: sinal recebido e mutex realocado, x = %d\n", x);
  }
  printf("Fique a vontade\n");
  if(aux==2){ pthread_cond_signal(&y_cond); }
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

// Thread D
void *D (void *t) {
  printf("D: Comecei\n");

  pthread_mutex_lock(&x_mutex);
  if (x < 3) { 
     printf("D: x= %d, vai se bloquear...\n", x);
     pthread_cond_wait(&y_cond, &x_mutex);
     printf("D: sinal recebido e mutex realocado, x = %d\n", x);
  }
  printf("Volte sempre!\n");
  pthread_mutex_unlock(&x_mutex); 
  pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
  int i; 
  pthread_t threads[NTHREADS];

  /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
  pthread_mutex_init(&x_mutex, NULL);
  pthread_cond_init (&x_cond, NULL);
  pthread_cond_init (&y_cond, NULL);

  /* Cria as threads */
  pthread_create(&threads[3], NULL, A, NULL);
  pthread_create(&threads[2], NULL, B, NULL);
  pthread_create(&threads[1], NULL, C, NULL);
  pthread_create(&threads[0], NULL, D, NULL);

  /* Espera todas as threads completarem */
  for (i = 0; i < NTHREADS; i++) {
    pthread_join(threads[i], NULL);
  }
  printf ("\nFIM\n");

  /* Desaloca variaveis e termina */
  pthread_mutex_destroy(&x_mutex);
  pthread_cond_destroy(&x_cond);
   pthread_cond_destroy(&y_cond);
}
