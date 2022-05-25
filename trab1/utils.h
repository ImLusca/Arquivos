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
registro_t* lerRegistroTipo1(FILE* fptr);
registro_t* lerRegistroTipo2(FILE* fptr);
cabecalho_t* lerCabecalhoTipo1(FILE* fptr);
cabecalho_t* lerCabecalhoTipo2(FILE* fptr);
void liberar(registro_t* r);
void imprimirRegistro(registro_t* r, cabecalho_t* c);

#endif