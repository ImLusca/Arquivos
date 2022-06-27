#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"

char* readUntil(FILE* a, char delimiter);
int readNumberUntil(FILE* a, char delimiter);
void readStaticUntil(FILE* a, int tamanho, char* s, char delimiter);
void lerRegistrocsv(FILE* arquivo, registro_t* registro, char* tipo);
void liberar(registro_t* r);
void escreverNoArquivo(FILE* arquivo, registro_t* registro, cabecalho_t* cabecalho, char* tipo);
void inicializarCabecalho(cabecalho_t* cabecalho);
void liberaMemSelect(int delCab, char* tArq,char* aEnt, FILE *fptr, cabecalho_t *cab);
void escreverCabecalho(FILE* arquivo, cabecalho_t* cabecalho, char* tipo);
void atualizarCabecalho(FILE* arquivo, cabecalho_t* cabecalho, char* tipo);
void binarioNaTela(char* nomeArquivoBinario);
void imprimirRegistro(registro_t* r, cabecalho_t* c);
void liberaRegistro(registro_t *reg);
void liberaStructBusca(buscaParams_t *busca);
void inicializaStructBusca(buscaParams_t *busca, int numFiltros, FILE *fptr);
int ehValidoFiltro(registro_t *reg, buscaParams_t *busca);
long int retornaIndiceDisponivel(cabecalho_t *cab, FILE *fptr, int tipoArq);// arquivo
void insereRegistroNoArquivo(registro_t *reg, cabecalho_t *cab, FILE *fptr, int tipoArquivo);// arquivo
registro_t *lerRegistroStdin(int tipoRegistro);
registro_t* lerRegistro(int tipo,FILE *fptr);
cabecalho_t *lerCabecalho(int tipo,FILE *fptr);

void atualizaCabecariot1(cabecalho_t *cab, FILE *fptr);
void atualizaCabecariot2(cabecalho_t *cab, FILE *fptr);
int buscaBinaria(int chave, registroIndice_t* vetor, int esq, int dir);

#endif
