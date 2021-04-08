#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

long int dim; // dimensao do vetor de entrada
double * vetor;   // matriz de entrada  
int nthreads; // numero de threads


//funcao que a thread executa
void* tarefa(void* arg){
    long int id = (long int) arg;
    double* somaLocal;
    long int tamBloco = dim/nthreads;
    long int ini = id * tamBloco;
    long int fim;   
    somaLocal = (double*) malloc(sizeof(double));
    if(somaLocal==NULL){
        fprintf(stderr,"ERRO--malloc\n"); exit(1);
    }
    *somaLocal=0;
    
    if(id==nthreads-1) fim = dim;
    else fim = ini + tamBloco;  
    
    for(long int i=ini; i<fim; i++)  *somaLocal += vetor[i];
 
    pthread_exit((void*) somaLocal); // temos que dar cast para void* aqui, pegamos esse valor com o join

}


int main(int argc, char* argv[]){
    
    double somaSeq=0;
    double somaConc=0;     // identificadores das threads no sistema
    double ini, fim, delta;
    pthread_t* tid;
    double* retorno;
    
    // leitura e avaliacao dos parametros de entrada
    if(argc<3){
        fprintf(stderr,"Digite: %s <dimensao do vetor> <numero de threads>\n", argv[0]);
        return 1; 
    }
    dim = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    
    // aloca o vetor de entrada
    vetor = (double*) malloc(sizeof(double) * dim);
     if(vetor==NULL){
       fprintf(stderr,"ERRO--malloc\n");
       return 2; 
    }
    // preenche o vetor de entrada
    for(long int i=0; i<dim; i++){
        vetor[i] = 1000.1/(i+1);    
    }   
    
    // soma sequencial dos elementos
    GET_TIME(ini);
    for(long int i=0; i<dim; i++){
        somaSeq += vetor[i];    
    }
    GET_TIME(fim);
    delta = fim-ini;
    printf("Tempo sequencial: %lf\n",delta);
    
    // soma concorrente dos elementos
    GET_TIME(ini);
    tid = (pthread_t*) malloc(sizeof(pthread_t) * nthreads);
    if(tid==NULL){
        fprintf(stderr,"ERRO--malloc");
        return 2;    
    }
    for(long int i=0; i<nthreads; i++){
        if(pthread_create(tid+i,NULL,tarefa,(void*) i)){
            fprintf(stderr,"ERRO--pthread_create");
            return 3;
        }    
    }
    for(long int i=0; i<nthreads; i++){
        if( pthread_join(*(tid+i), (void**) &retorno) ){
            fprintf(stderr,"ERRO--pthread_create");
            return 3;
        }  
        somaConc += *retorno;  
        free(retorno);
    }
    GET_TIME(fim);
    delta = fim - ini;
    printf("Tempo concorrente: %lf\n", delta);
    
    // exibir os resultados
    printf("Soma sequencial: %lf\n", somaSeq);
    printf("Soma concorrente: %lf\n", somaConc);

    // liberacao de memoria
    free(vetor);
    free(tid);

    return 0;    
}
