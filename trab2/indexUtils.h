#ifndef INDEXUTILS_H
#define INDEXUTILS_H

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "funcionalidades.h"
#include "utils.h"

void escreverCabecalhoIndex(FILE* arquivo, cabecalhoIndex_t* cabecalho);
indice_t* inicializarIndice();
void escreverIndice(FILE* arquivo, indice_t* indice, char* tipoArquivo);
void liberarIndice(indice_t* indice);
cabecalhoIndex_t* lerCabecalhoIndex(FILE* index);
indice_t* indiceParaRAM(FILE* index, int tipo);
registroIndice_t* buscaIndex(FILE* index, indice_t* indice, int chave);
indice_t* removerRegistroIndex(FILE* index, int* id, int tipo, int qttId);

#endif