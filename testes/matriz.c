#include<stdio.h>
#include<stdlib.h>
#include<pthread.h>
#include"timer.h"

float* mat;   //matriz de entrada 
float* met;   //vetor de entrada
float* saida; //vetor de saida 
int nthreads; //numero de threads

typedef struct {
    int id; // identificador do elemento que a thread ira processar
    int dim; // dimensao das estruturas de entrada
} tArgs;

//funcao que a thread executa
void* tarefa(void* arg){
    tArgs* args = (tArgs*) arg;
    //printf("Thread %d\n", args->id);
    for(int i=args->id; i<args->dim; i+=nthreads){
        for(int j=0; j<(args->dim); j++){ 
            for(int k=0; k<(args->dim); k++){
                saida[i*(args->dim) + j] += mat[i*(args->dim) + k] * met[k*(args->dim) + j]; 
        }   }
    }    
    pthread_exit(NULL);

}


int main(int argc, char* argv[]){
    
    int dim; // dimensão da matriz de entrada
    pthread_t* tid; // identificadores das threads no sistema
    tArgs* args;   // identificadores locais das threads e dimensao
    double inicio, fim, delta, x, y, z, total;
    
    GET_TIME(inicio);
    // leitura e avaliacao dos parametros de entrada
    if(argc<3){
        printf("Digite: %s <dimensao da matriz> <numero de threads>\n", argv[0]);
        return 1; 
    }
    dim = atoi(argv[1]); // transforma string em int e armazena em dim
    nthreads = atoi(argv[2]);
    if(nthreads>dim) nthreads = dim;    
    // alocação de memoria para as estruturas de dados 
    mat = (float*) malloc(sizeof(float) * dim * dim);       // dim * dim pois é uma matriz quadrada, tratada como vetor unidimensional 
    if(mat == NULL){ printf("ERRO--malloc\n"); return 2; }
    met = (float*) malloc(sizeof(float) * dim * dim);  
    if(met == NULL){ printf("ERRO--malloc\n"); return 2; }
    saida = (float*) malloc(sizeof(float) * dim * dim); 
    if(saida == NULL){ printf("ERRO--malloc\n"); return 2; }
    
    // inicializacao das estruturas de dados 
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++){
            mat[i*dim + j] = 3; // dim == colunas nesse caso , equivalente a preencher mat[i][j]
            met[i*dim + j] = 2;
            saida[i*dim + j] = 0;
        }
    }
    GET_TIME(fim);
    delta = fim - inicio;
    x = delta;
    printf("Tempo de inicializacao: %lf\n",delta);   
    
  

    // multiplicacao da matriz pelo vetor versão concorrente
    GET_TIME(inicio);    
    // alocação das estruturas
    tid = (pthread_t*) malloc(sizeof(pthread_t)*nthreads);
    if(tid==NULL){puts("ERRO--malloc"); return 2;}
    args = (tArgs*) malloc(sizeof(tArgs)*nthreads);
    if(args==NULL){puts("ERRO--malloc"); return 2;}
    
    // criação das threads
    for(int i=0; i<nthreads; i++){
        (args+i)->id = i;
        (args+i)->dim = dim;
        if( pthread_create(tid+i, NULL, tarefa, (void*) (args+i)) ) {
             puts("ERRO--pthread_create\n"); return 3;
        }
    }

    // espera pelo termino das threads 
    for(int i=0; i<nthreads; i++){
        pthread_join( *(tid+i), NULL); // aqui passamos o valor, nao o endereco, por isso usamos *
    }
    GET_TIME(fim);
    delta = fim - inicio;
    y = delta;
    printf("Tempo das threads: %lf\n",delta);   

    
    
    // exibicao dos resultados
     /*   
    puts("Primeira matriz de entrada:");
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++) { printf("%.1f ", mat[i*dim+j]); }
        puts("");
    }    
    puts("Segunda matriz de entrada:");
    for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++) { printf("%.1f ", met[i*dim+j]); }
        puts("");
    }    
        puts("Matriz de saída:");
     for(int i=0; i<dim; i++){
        for(int j=0; j<dim; j++) { printf("%.1f ", saida[i*dim+j]); }
        puts("");
    }    
    */
    
    GET_TIME(inicio);
    // liberacao da memoria
    free(met);
    free(mat);
    free(saida);
    free(args);
    free(tid);

    GET_TIME(fim);
    delta = fim - inicio;
    z = delta;
    printf("Tempo de finalizacao: %lf\n",delta);
    total = x+y+z;
    printf("Tempo total: %lf\n",total);
    return 0;
}
