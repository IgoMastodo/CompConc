#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"
#include<math.h>

long int it; // numero de iteracoes
long double pi=0;
int nthreads ; // numero de threads

void* tarefa(void* arg){
    long int id = (long int) arg;
    long double* somaLocal;
    long int tamBloco = it/nthreads;
    long int ini = id * tamBloco;
    long int fim;   
    somaLocal = (long double*) malloc(sizeof(long double));
    *somaLocal=0;
    
    if(id==nthreads-1) fim = it;
    else fim = ini + tamBloco;  
    
    //printf("Thread %ld\n",id);

    for(long int i=ini; i<fim; i++)  *somaLocal +=  4 * (( pow(-1,((long double) i)) ) * 1/( 2*i + 1));

    //printf("%.Lf\n",*somaLocal);

    pthread_exit((void*) somaLocal); // temos que dar cast para void* aqui, pegamos esse valor com o join

}

int main(int argc, char* argv[]){

    long double somaConc=0;     // identificadores das threads no sistema
    double ini, fim, delta;
    pthread_t* tid;
    long double* retorno, dif;
    // leitura e avaliacao dos parametros de entrada
    if(argc<3){
        fprintf(stderr,"Digite: %s <numero de iteracoes> <numero de threads>\n", argv[0]);
        return 1; 
    }
    it = atoi(argv[1]);
    nthreads = atoi(argv[2]);
    
    // chamada das threads
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
    pi=somaConc;
    delta=fim-ini;
    dif = M_PI- pi;
    printf("Valor de pi (concorrente): %.15Lf\n",pi);
    printf("Diferenca entre a constante M_PI  e o calculado: %.15Lf\n",dif);
    printf("Tempo usado na soma concorrente: %lf\n",delta);
    puts("");
    pi=0;
    // versao sequencial
    GET_TIME(ini); 
    for(int i=0; i<it; i++){
        pi += 4 * (( pow(-1,((double) i)) ) * 1/( 2*i + 1));
    }
    GET_TIME(fim);
    delta=fim-ini;
    dif = M_PI - pi;
    printf("Valor de pi (sequencial): %.15Lf\n",pi);
    printf("Diferenca entre a constante M_PI e o calculado: %.15Lf\n",dif);
    printf("Tempo usado na soma sequencial: %lf\n",delta);
    

    free(tid);
    
    return 0;
}