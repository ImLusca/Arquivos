#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funcionalidades.h"
#include "utils.h"
#include "indexUtils.h"

/**
 * @brief Escreve o cabe�alho em um arquivo bin�rio de �ndices
 *
 * @param index : ponteiro para o arquivo
 * @param cabecalho : struct contendo os dados do cabe�alho
 */
void escreverCabecalhoIndex(FILE* index, cabecalhoIndex_t* cabecalho)
{
	fseek(index, 0, 0);
	fwrite(&cabecalho->status, 1, 1, index);
}

cabecalhoIndex_t* lerCabecalhoIndex(FILE* index)
{
	cabecalhoIndex_t* cab = malloc(sizeof(cabecalhoIndex_t));
	fread(&cab->status, 1, 1, index);

	return cab;
}

indice_t* inicializarIndice()
{
	indice_t* i = malloc(sizeof(indice_t));
	i->tamanho = 0;
	i->registros = NULL;

	return i;
}

static int compara(const void* reg1, const void* reg2)
{
	int id1 = ((registroIndice_t*)reg1)->id;
	int id2 = ((registroIndice_t*)reg2)->id;
	return id1 - id2;
}

void escreverIndice(FILE* index_fptr, indice_t* indice, char* tipoArquivo)
{
	qsort(indice->registros, indice->tamanho, sizeof(registroIndice_t), compara);

	for (int i = 0; i < indice->tamanho; i++)
	{
		fwrite(&indice->registros[i].id, sizeof(int), 1, index_fptr);

		if (strcmp(tipoArquivo, "tipo1") == TIPOS_IGUAIS)
			fwrite(&indice->registros[i].RRN, sizeof(int), 1, index_fptr);

		else if (strcmp(tipoArquivo, "tipo2") == TIPOS_IGUAIS)
			fwrite(&indice->registros[i].byteOffset, sizeof(long long int), 1, index_fptr);
	}
}

void liberarIndice(indice_t* indice)
{
	free(indice->registros);
	free(indice);
}

registroIndice_t* lerRegistroIndex(FILE* index, int tipo)
{
	registroIndice_t* r = malloc(sizeof(registroIndice_t));
	fread(&r->id, sizeof(int), 1, index);
	if (tipo == 1)
		fread(&r->RRN, sizeof(int), 1, index);
	else if (tipo == 2)
		fread(&r->byteOffset, sizeof(long long int), 1, index);

	return r;
}

indice_t* indiceParaRAM(FILE* index, int tipo)
{
	indice_t* i = malloc(sizeof(indice_t));

	char buffer;
    int count = 0;
	fseek(index, TAM_CABECALHO_IND, SEEK_SET);

    while (fread(&buffer, 1, 1, index) != 0)
    {
        ungetc(buffer, index);

		registroIndice_t* registroIndice = lerRegistroIndex(index, tipo);

        i->registros = realloc(i->registros, (count + 1) * sizeof(registroIndice_t));
        i->registros[count] = (*registroIndice);
        
        count++;
        free(registroIndice);
    }
    i->tamanho = count;

	return i;
}

registroIndice_t* buscaIndex(FILE* index, indice_t* indice, int chave)
{
	registroIndice_t* registro = NULL;
	int posicao = buscaBinaria(chave, indice->registros, 0, indice->tamanho);

	if (posicao == -1) // registro n�o achado pela busca bin�ria
		printf("Registro inexistente.\n");

	else
		registro = &(indice->registros[posicao]);

	return registro;
}

indice_t* removerRegistroIndex(FILE* index, int* id, int tipo, int qttId)
{
	indice_t* indice = indiceParaRAM(index, tipo);

	for (int i = 0; i < qttId; i++)
	{
		int posicao = buscaBinaria(id[i], indice->registros, 0, indice->tamanho);

		if (posicao == -1)
		{ // registro n�o achado pela busca bin�ria
			printf("Registro inexistente.\n");
			return indice;
		}

		for (int i = posicao; i < indice->tamanho; i++)
			indice->registros[i] = indice->registros[i + 1];

		indice->tamanho--;
		indice->registros = realloc(indice->registros, indice->tamanho * sizeof(registroIndice_t));
	}
	return indice;
}