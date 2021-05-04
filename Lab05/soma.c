#include <pthread.h>
#include <stdio.h>
#include <stdlib.h> 
#include <math.h>
#include <unistd.h>

/* Variaveis globais */
int bloqueadas = 0;
pthread_mutex_t x_mutex;
pthread_cond_t x_cond;

int* vet;
int numThreads;
int ArrSize;
int dim;

// funcao barreira
void barreira(int nthreads) {
    pthread_mutex_lock(&x_mutex); //inicio secao critica
    if (bloqueadas == (nthreads-1)) { 
      //ultima thread a chegar na barreira
      pthread_cond_broadcast(&x_cond);
      bloqueadas=0;
    } else {
      bloqueadas++;
      pthread_cond_wait(&x_cond, &x_mutex);
    }
    pthread_mutex_unlock(&x_mutex); //fim secao critica
}

// funcao das threads
void *tarefa (void *arg) {
    int id = *(int*)arg;
    int aux;
    int ref;

for(int i=0; i<dim; i++) {
    ref = (int) pow(2, i);
    if(id - ref >= 0) {
      aux = vet[id - ref];
    }
    barreira(numThreads);
    if(id - ref >= 0) {
      vet[id] += aux;
    }
    barreira(numThreads);
  }
pthread_exit(NULL);
}

/* Funcao principal */
int main(int argc, char *argv[]) {
    int *vetRef;

    /* Inicilaiza o mutex (lock de exclusao mutua) e a variavel de condicao */
    pthread_mutex_init(&x_mutex, NULL);
    pthread_cond_init (&x_cond, NULL);

    // leitura e avaliacao dos parametros de entrada
    if(argc<2){
        printf("Digite: %s <dimensao da matriz> \n", argv[0]);
        return 1; 
    }

    dim = atoi(argv[1]);
    ArrSize = pow(2,dim);
    numThreads = ArrSize;
   
    
    
    // alocação de memoria para as estruturas de dados 
    vet = (int*) malloc( sizeof(int) * (ArrSize) ); 
    if(vet == NULL) { printf("ERRO--malloc\n"); return 2; }    

    vetRef = (int*) malloc( sizeof(int) * (ArrSize) );
    if(vetRef == NULL) { printf("ERRO--malloc\n"); return 2; }
    
    // inicializacao dos vetores
    for(int i=0; i<ArrSize; i++) vet[i] = 1;    
    for(int i=0; i<ArrSize; i++) vetRef[i] = 1; 

    
    printf("Vetor inicial:\n\n");
    for(int i=0; i<ArrSize; i++) printf("[%d] ", vetRef[i]);

    // versao sequencial usada como controle
    printf("\n\nVetor final esperado:\n\n");
    for(int i=1; i<ArrSize; i++) vetRef[i] = vetRef[i] + vetRef[i-1];
    for(int i=0; i<ArrSize; i++) printf("[%d] ", vetRef[i]); 


    // versao concorrente

    /*declara as threads*/
    int id[numThreads];
    pthread_t threads[numThreads];
    
    /* Cria as threads */
    for(int i=0;i<numThreads;i++) {
        id[i]=i;
        pthread_create(&threads[i], NULL, tarefa, (void *) &id[i]);
    }

    /* Espera todas as threads completarem */
    for (int i = 0; i < numThreads; i++) {
    pthread_join(threads[i], NULL);
    }
    printf("\n\nVetor final obtido:\n\n");
    for(int i=0; i<ArrSize; i++){
        printf("[%d] ", vet[i]);
    }
    printf ("\n\nFIM.\n");
   
    /* Desaloca variaveis e termina */
    pthread_mutex_destroy(&x_mutex);
    pthread_cond_destroy(&x_cond);
    return 0;
}

