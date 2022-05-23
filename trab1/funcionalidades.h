#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H
#include <stdlib.h>
#include <stdio.h>

typedef struct cabecalho1
{
	char status;
	int topo;
	char descricao[40];
	char desC1[22];
	char desC2[19];
	char desC3[24];
	char desC4[8];
	char corC5;
	char desC5[16];
	char corC6;
	char desC6[18];
	char corC7;
	char desC7[19];
	int proxRRN;
	int nroRegRem;

}cabecalho1_t;

typedef struct registro
{
	char removido;
	int tamRegistro;
	int prox;
	int id;
	int ano;
	int qtt;
	char sigla[2];
	int tamCidade;
	char* cidade;
	char codC5;
	int tamMarca;
	char* marca;
	char codC6;
	int tamModelo;
	char* modelo;
	char codC7;
} registro_t;


FILE* funcionalidade1();
void funcionalidade2();
FILE* funcionalidade3();
FILE* funcionalidade4();



#endif