#include <stdio.h>
#include <stdlib.h>
#include "escritaprior.h"

void* sensores(void* args) {
  pthread_exit(NULL);
}
void* atuadores(void* args) {
  pthread_exit(NULL);
}

typedef struct {
  Controle *ctrl;
} Argumento;

int main(int argc, char** argv) {
    Controle *ctrl;
    Argumento *arg;
    inicializaCtrlLeituraEscrita(&ctrl);
    /*
    coloca pra rodar e tudo mais
    */
    terminaCtrlLeituraEscrita(&ctrl);
    return 0;
}