#ifndef FUNCIONALIDADES_H
#define FUNCIONALIDADES_H
#define TIPOS_IGUAIS 0
#define TAM_CABECALHO1 182
#define ESTATICOS1 34
#define ESTATICOS2 37
#define TAM_CABECALHO2 190
#define TAM_REG 97
#define CABECALHO_CSV 61
#define TAM_CABECALHO_IND 1
#define ARQUIVO_INCONSISTENTE '0'
#define ARQUIVO_CONSISTENTE '1'
#define REGISTRO_REMOVIDO '1'
#define NUM_PARAMETROS 7

#include <stdlib.h>
#include <stdio.h>


typedef struct cabecalho
{
	char status;
	int topoA;
	long int topoB;
	char descricao[41];
	char desC1[23];
	char desC2[20];
	char desC3[25];
	char desC4[9];
	char codC5;
	char desC5[17];
	char codC6;
	char desC6[19];
	char codC7;
	char desC7[20];
	int proxRRN;
	long int proxByteOffset;
	int nroRegRem;
}cabecalho_t;

typedef struct registro
{
	char removido;
	int tamRegistro;
	int proxA;
	long int proxB;
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

typedef struct buscaParams{
	int ehBuscado[7];
	char **filtros;
} buscaParams_t;

typedef struct cabecalhoIndice
{
	char status;
} cabecalhoIndex_t;

typedef struct registroIndice
{
	int id;
	int RRN;
	long long int byteOffset;
} registroIndice_t;

typedef struct indice
{
	int tamanho;
	registroIndice_t* registros;
} indice_t;

typedef struct idsRemovidos
{
	int tamanho;
	int* ids;
}idsRemovidos_t;

void createTable();
void selectSemWhere();
void selectCWhere();
void recuperarRegistro();
void createIndex();
void delete();
void insertInto();

#endif
