#include <stdio.h>
#include <stdlib.h> 
#include <pthread.h>

#define VECTORSIZE 25

//cria a estrutura de dados para armazenar os argumentos da thread
typedef struct {
   int idThread; // identificador da thread
   int* vetor; // ponteiro para o tipo inteiro que vai ser usado para o vetor
} t_Args;

//funcao executada pelas threads
void* tarefa (void* arg) {
  
  t_Args* args = (t_Args *) arg;
  
  int id = args->idThread;
  
  if(id == 0){
    for (int i=0; i<VECTORSIZE/2; i++) {
        args->vetor[i]++;
        printf("Posição %d do vetor icrementada para %d\n", i, args->vetor[i]);
    }  
  }
  else{
    for (int i=VECTORSIZE/2; i<VECTORSIZE; i++) {   
        args->vetor[i]++;
        printf("Posição %d do vetor icrementada para %d\n", i, args->vetor[i]);
    }  
  }
  free(arg); //aqui pode liberar a alocacao feita na main

  pthread_exit(NULL);
}

int main(void){

    // declaração de uma variavel do tipo ponteiro para int com alocação para N posições na memória com tamanho de int    
    int* vector = (int*) malloc(VECTORSIZE * sizeof(int));
    
    // declaração de variável do tipo ponteiro para t_Args, nossa struct
    t_Args* arg;
    
    // declaração da id das threads pro sistema
    pthread_t tid[2];

    // preenchimento dos espaços da memoria, que poderia ter sido feito direto usando calloc, visto que inicializamos com 0 em todas as posições;   
    for (int i=0; i<VECTORSIZE; i++) vector[i]=0;
    
    // mostrar o estado inicial do vetor
    printf("Valores iniciais do vetor com %d espaços: \n", VECTORSIZE);
    for(int i=0; i<VECTORSIZE; i++) printf(" %d ", vector[i]);    
   
    printf("\n");   

    //agora vamos declarar os argumentos para as threads
    for(int thread=0; thread<2; thread++) {
        printf("--Aloca e preenche argumentos para thread %d\n", thread);
        arg = malloc(sizeof(t_Args));
        if (arg == NULL) {
            printf("--ERRO: malloc()\n"); exit(-1);
        }
        arg->idThread = thread;  
        arg->vetor = vector;
        //arg->vetor
        printf("--Cria a thread %d\n", thread);
        if (pthread_create(&tid[thread], NULL, tarefa, (void*) arg)) {
        printf("--ERRO: pthread_create()\n"); exit(-1);
        }
    }    
    
    //--espera todas as threads terminarem
    for (int thread=0; thread<2; thread++) {
         if (pthread_join(tid[thread], NULL)) {
         printf("--ERRO: pthread_join() \n"); exit(-1); 
         } 
    }
   
    // verificação dos valores do vetor
    for (int i=0; i<VECTORSIZE; i++) {
        if(args->vetor[i]!=1) { printf("--ERRO: SOMA)\n"); exit(-1); }
    }
    printf("\nValores finais do vetor com %d espaços: \n", VECTORSIZE);
    for(int i=0; i<VECTORSIZE; i++) printf(" %d ", vector[i]);
    
    printf("\n Fim da execução\n"); 
}
