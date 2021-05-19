#include <pthread.h>
#include "escritaprior.h"


void inicializaControle(Controle** ctrl){
 
}
void finalizaControle(Controle** ctrl){
   
}
void entraEscrita() {

}
void saiEscrita() {

}
void entraLeitura() {

}
void saiLeitura() {

}
// funcao das threads
void *leitor (void *arg) {
    while(1) {
        EntraLeitura();
        //le algo...
        SaiLeitura();
        //faz outra coisa...
        }
    }
void *escritor (void *arg) {
    while(1) {
    EntraEscrita();
    //escreve algo...
    SaiEscrita();
    //faz outra coisa...
    }
}
int leit=0, escr=0; //globais

void EntraLeitura(Controle* ctrl) {
    pthread_mutex_lock(&mutex);
    while(escr > 0 && ctrl->querem_escrever > 0) {
        pthread_cond_wait(&cond_leit, &mutex);
    }
    leit++;
    pthread_mutex_unlock(&mutex);
}
void SaiLeitura(Controle* ctrl) {
    pthread_mutex_lock(&mutex);
    leit--;
    if(leit==0) pthread_cond_signal(&cond_escr);
    pthread_mutex_unlock(&mutex);
}
//Funcoes para escrita
int leit=0, escr=0; //globais
void EntraEscrita (Controle* ctrl) {
    pthread_mutex_lock(&mutex);
    while((leit>0) || (escr>0)) {
        pthread_cond_wait(&cond_escr, &mutex);
    }
    escr++;
    pthread_mutex_unlock(&mutex);
}
void SaiEscrita (Controle* ctrl) {
    pthread_mutex_lock(&mutex);
    escr--;
    pthread_cond_signal(&cond_escr);
    pthread_cond_broadcast(&cond_leit);
    pthread_mutex_unlock(&mutex);
}

