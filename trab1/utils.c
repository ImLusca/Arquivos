#include <stdio.h>
#include <stdlib.h>
#include "funcionalidades.h"

char* readUntil(FILE* a, char delimiter)
{
	char* word = NULL;

	int i = 0;
	do
	{
		word = realloc(word, (i + 1) * sizeof(char));
		word[i] = fgetc(a);

		if (word[i] == '\r') word[i] = fgetc(a);

		if (word[i] == '\n' || word[i] == delimiter || word[i] == EOF)
		{
			word[i] = '\0';
		}

		i++;
	} while (word[i - 1] != '\0');

	return word;
}

void lerArquivoTipo1(FILE* arquivo, registro_t* registro)
{
	fscanf(arquivo, "%d,%d,%d,%c%c", &registro->id, &registro->ano, &registro->qtt, 
		&registro->sigla[0], &registro->sigla[1]);
	registro->cidade = readUntil(arquivo, ',');
	registro->marca = readUntil(arquivo, ',');
	registro->modelo = readUntil(arquivo, ',');
	fscanf(arquivo, "%c,%d,%d,%d,%d", &registro->removido, &registro->prox,
		&registro->tamCidade, &registro->tamMarca, &registro->tamModelo);
}

void lerArquivoTipo2(FILE* arquivo, registro_t* registro)
{
	fscanf(arquivo, "%d,%d,%d,%c%c", &registro->id, &registro->ano, &registro->qtt,
		&registro->sigla[0], &registro->sigla[1]);
	registro->cidade = readUntil(arquivo, ',');
	registro->marca = readUntil(arquivo, ',');
	registro->modelo = readUntil(arquivo, ',');
	fscanf(arquivo, "%c,%d,%d,%d,%d,%d", &registro->removido, &registro->tamRegistro, 
		&registro->prox, &registro->tamCidade, &registro->tamMarca, &registro->tamModelo);
}


registro_t *lerRegistroTipo1(FILE *fptr){
	
	registro_t *reg = malloc(sizeof(registro_t));

	fscanf(fptr, "%c%i%i%i%i", &reg->removido, &reg->prox,&reg->id,&reg->ano, &reg->qtt);
	fscanf(fptr, "%c%c%i%c", &reg->sigla[0],&reg->sigla[1],&reg->tamCidade, &reg->codC5);
	
	reg->cidade = malloc(sizeof(char) * reg->tamCidade);
	fread(reg->cidade,reg->tamCidade, 1, fptr);

	fscanf(fptr,"%i%c", &reg->tamMarca,&reg->codC6);

	reg->marca = malloc(sizeof(char) * reg->tamMarca);
	fread(reg->marca,reg->tamMarca, 1, fptr);
	
	fscanf(fptr,"%i%c", &reg->tamModelo,&reg->codC7);

	reg->modelo = malloc(sizeof(char) * reg->tamModelo);
	fread(reg->modelo,reg->tamModelo, 1, fptr);

	return reg;
}

void binarioNaTela(char* nomeArquivoBinario) { /* Você não precisa entender o código dessa função. */

	/* Use essa função para comparação no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (você não vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char* mb;
	size_t fl;
	FILE* fs;
	if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (função binarioNaTela): não foi possível abrir o arquivo que me passou para leitura. Ele existe e você tá passando o nome certo? Você lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char*)malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for (i = 0; i < fl; i++) {
		cs += (unsigned long)mb[i];
	}
	printf("%lf\n", (cs / (double)100));
	free(mb);
	fclose(fs);
}