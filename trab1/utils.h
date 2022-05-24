#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"

char* readUntil(FILE* a, char delimiter);
int readNumberUntil(FILE* a, char delimiter);
void readStaticUntil(FILE* a, int tamanho, char* s, char delimiter);
void lerArquivo(FILE* arquivo, registro_t* registro);
void escreverArquivo(FILE* arquivo, registro_t* registro, char* tipo);
void binarioNaTela(char* nomeArquivoBinario);
registro_t* lerRegistroTipo1(FILE *fptr);
registro_t* lerRegistroTipo2(FILE *fptr);

#endif