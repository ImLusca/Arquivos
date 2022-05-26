#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"

char* readUntil(FILE* a, char delimiter);
int readNumberUntil(FILE* a, char delimiter);
void readStaticUntil(FILE* a, int tamanho, char* s, char delimiter);
void lerRegistrocsv(FILE* arquivo, registro_t* registro, char* tipo);
void escreverNoArquivo(FILE* arquivo, registro_t* registro, cabecalho_t* cabecalho, char* tipo);
void inicializarCabecalho(cabecalho_t* cabecalho);
void escreverCabecalho(FILE* arquivo, cabecalho_t* cabecalho, char* tipo);
void atualizarCabecalho(FILE* arquivo, cabecalho_t* cabecalho, char* tipo);
void binarioNaTela(char* nomeArquivoBinario);
void imprimirRegistro(registro_t* r, cabecalho_t* c);
void liberaRegistro(registro_t *reg);
void liberaStructBusca(buscaParams_t *busca);
void inicializaStructBusca(buscaParams_t *busca);
int ehValidoFiltro(registro_t *reg, buscaParams_t *busca);
registro_t* lerRegistro(int tipo,FILE *fptr);
cabecalho_t *lerCabecalho(int tipo,FILE *fptr);

#endif
