#include <pthread.h>

// paradigma leitura e escrita, com prioridade para escrita

typedef struct {
    pthread_mutex_t totem;
    pthread_cond_t cond_leit;
    pthread_cond_t cond_escr;
    int qtd_leitoras;
    int qtd_escritoras;
    // se igual a 1 entraLeitura bloqueia novas threads de entrarem
    // se igual a 0 entraLeitura permite entrarem para leitura normalmente
    int querem_escrever;
} Controle;

void inicializaCtrlLeituraEscrita(Controle** ctrl);
void terminaCtrlLeituraEscrita(Controle** ctrl);
void entraEscrita(Controle* ctrl);
void saiEscrita(Controle* ctrl);
void entraLeitura(Controle* ctrl);
void saiLeitura(Controle* ctrl);
