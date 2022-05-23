#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"

void lerArquivoTipo1(FILE* arquivo, registro_t* registro);
void lerArquivoTipo2(FILE* arquivo, registro_t* registro);
char* readUntil(FILE* a, char delimiter);
void binarioNaTela(char* nomeArquivoBinario);
registro_t* lerRegistroTipo1(FILE *fptr);
registro_t* lerRegistroTipo2(FILE *fptr);

#endif