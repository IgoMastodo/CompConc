#include<stdio.h>
#include<pthread.h>
#include<math.h>
#include"timer.h"


typedef struct {
    int id;
    long int NTHREADS;
    long int NTERMOS;
    long double limInf , limSup;
} Argumento;

void* tarefaSimpson(void* container){
    Argumento* args = (Argumento*) container;
    long int id=(long int) args->id;    // numero da thread
    long double* somaLocal;             // soma local feita por cada thread e eh o retorno
    long int tamBloco = args->NTERMOS/args->NTHREADS; // separacao dos termos para blocos das threads
    long int ini = id * tamBloco;       // definicao do inicio para cada thread
    long int fim;                       // definicao do fim para cada thread
    long double altura = (args->limSup - args->limInf)/((long double) args->NTERMOS); // h=(a-b)/n 

    somaLocal = (long double*) malloc(sizeof(long double)); //
    *somaLocal = 0;

    if(id == args->NTHREADS-1) fim = args->NTERMOS;
    else fim = ini + tamBloco;
        
    for(long int i=ini; i<fim; i++) *somaLocal += 0;
        
    pthread_exit((void*) somaLocal); // melhor ser NULL 
    // temos que dar cast para void* aqui, pegamos esse valor com o join

}

void* tarefaTrapezio(void* container){
    Argumento* args = (Argumento*) container;
    long int id=(long int) args->id;    // numero da thread
    long double* somaLocal;             // soma local feita por cada thread e eh o retorno
    long int tamBloco = args->NTERMOS/args->NTHREADS; // separacao dos termos para blocos das threads
    long int ini = id * tamBloco;       // definicao do inicio para cada thread
    long int fim;                       // definicao do fim para cada thread
    somaLocal = (long double*) malloc(sizeof(long double)); //
    *somaLocal = 0;

    if(id == args->NTHREADS-1) fim = args->NTERMOS;
    else fim = ini + tamBloco;

    for(long int i=ini; i<fim; i++) *somaLocal +=  0;

    pthread_exit((void*) somaLocal); // melhor ser NULL 
    // temos que dar cast para void* aqui, pegamos esse valor com o join

}

double funcPoli(int grau, double fator[n], double x){
    double func;
    for(int i=0;i<grau;i++){
        func += fator[i]* pow(x,i); 
    }
    return func;
}
double funcExp(double a, double b,double c, double x){
    return a*exp(b*x) + c;
}
double funcLog(double a, double b,double c, double x){
    return a*log(b*x) + c;
}
double funcSen(double a, double b,double c, double x){
    // a*sin(b*x);
    return a*sin(b*x) + c;
}
double funcCos(double a, double b, double c, double x){
    return a*cos(b*x) + c;
}

void como_usar() {
    printf("\n ### Programa de análise de desempenho comparando os Métodos de Integração, Trapézios e 1/3 de Simpson ###");
    printf("\n\t[ 1 ] -> Polinômio");   // [a0]*x^0 + [a1]*x^1 + ... + [an]*x^n , [i, f]
    printf("\n\t[ 2 ] -> Exponencial"); // a*e^(b * x) + c, [i, f]
    printf("\n\t[ 3 ] -> Logaritimica");// a * ln(b * x) + c, x > 2  b > 1, [i > 2 , f >2]
    printf("\n\t[ 4 ] -> Seno");        // a * sin(b * x) + c , [i, f]
    printf("\n\t[ 5 ] -> Cosseno");     // a * cos(b * x) + c , [i, f]
    printf("\n Digite uma das opções acima : ");
}

int* pega_valores_polinomio() {
    int *ret;
    char *listener;
    int i;
    ret = (int *) malloc(sizeof(int)*1000);
    for( i = 0 i < 1000 ; i++) {
        ret[i] = 0;
    }
    i = 0;
    printf("\n\t Polinômio ( [a0]*x^0 + [a1]*x^1 + ... + [an]*x^n ) ");
    printf("\n\t Exemplo:");
    printf("\n\t\t 1 2 3 10");
    printf("\n\t\t 1 + x + 2*x^2 + 3*x^3 + 10*x^4");
    printf("\n\t Digite os valores das constantes a{n}:");
    scanf("%s", listener);
    listener = strtok(str,",");
    ret[i] = atoi(listener);
    i++;
    while (listener != NULL)
    {
        listener = strtok (NULL, ",");
        if(listener != NULL)
        ret[i] = atoi(listener);
        i++;
    }
    return ret;
}

int* pega_valores_exponencial() {
    int* ret;
    return ret;
}

int* pega_valores_logaritmo() {
    int* ret;
    return ret;
}

int* pega_valores_seno() {
    int* ret;
    return ret;
}

int* pega_valores_cosseno() {
    int* ret;
    return ret;
}

int main(void){
    int resposta, qtd_constantes;
    int *constantes;
    Argumento* args;
    long double somaConc=0; 
    double ini,fim,delta;          // variaveis de tempo
    pthread_t* tid;                // identificacao das threads
    long double* integralControle; // valor esperado da integral
    
    
    while(resposta != 0) {
        como_usar();
        scanf("%d", &resposta);
        qtd_constantes = 0;
        switch(resposta) {
            case 1:
                constantes = pega_valores_polinomio();
                break;
            case 2:
                constantes = pega_valores_exponencial();
                break;
            case 3:
                constantes = pega_valores_logaritmo();
                break;
            case 4:
                constantes = pega_valores_seno();
                break;
            case 5:
                constantes = pega_valores_cosseno();
                break;
        }
        while(constantes[qtd_constantes] != 0) {
            qtd_constantes++;
        }
    }    

    /*Entrada de dados */
    printf("\nDiga o limite inicial: ");
    scanf("%lf",&args->limInf);
    printf("\nDiga o limite final: ");  
    scanf("%lf",&args->limSup);
    printf("\nEntre o numero de sub-intervalos/ precisao ");
    scanf("%lf",&args->NTERMOS);
    

    args = (Argumento *) malloc(sizeof(Argumento)* /* coloca aqui a variavel de quantidade de termos */);
    if(args == NULL || tid == NULL) {
        printf("\nERRO : malloc");
        return 2;
    }

    // parte concorrente
    // criação das threads
    for(int i=0; i<args->NTHREADS; i++){
        (args+i)->id = i;
        if( pthread_create(tid+i, NULL, tarefa, (void*) (args+i)) ) {
             puts("ERRO--pthread_create\n"); return 3;
        }
    }

    // espera pelo termino das threads 
    for(int i=0; i<args->NTHREADS; i++){
        pthread_join( *(tid+i), NULL); 
    }
    GET_TIME(fim);
    delta = fim - ini;
    printf("Tempo das threads: %lf\n",delta);   


    
    
    return 0;
}