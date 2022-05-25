#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"

static int campoEstaPreenchido(char codigo, registro_t *reg);

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

	if (word[0] == '\0')
	{
		free(word);
		return NULL;
	}

	return word;
}

int readNumberUntil(FILE* a, char delimiter)
{
	char* num = readUntil(a, delimiter);
	if (num == NULL || num[0] == '\0')
	{
		if (num != NULL) free(num);
		return -1;
	}

	int number = atoi(num);
	free(num);

	return number;
}

void readStaticUntil(FILE* a, int tamanho, char* s, char delimiter)
{
	int d = 0;
	for (int i = 0; i < tamanho; i++)
	{
		s[i] = fgetc(a);
		if (s[i] == ',')
		{
			d++;
			s[i] = '\0';
			if (i == 0)
			{
				for (int i = 0; i < tamanho; i++)
				{
					s[i] = '$';
				}
			}
			break;
		}
	}
	char delim;
	if (d == 0) delim = fgetc(a);
}

void inicializarCabecalho(cabecalho_t* cabecalho)
{
	cabecalho->topoA = -1;
	cabecalho->topoB = -1;
	strcpy(cabecalho->descricao, "LISTAGEM DA FROTA DOS VEICULOS NO BRASIL");
	strcpy(cabecalho->desC1, "CODIGO IDENTIFICADOR: ");
	strcpy(cabecalho->desC2, "ANO DE FABRICACAO: ");
	strcpy(cabecalho->desC3, "QUANTIDADE DE VEICULOS: ");
	strcpy(cabecalho->desC4, "ESTADO: ");
	cabecalho->codC5 = '0';
	strcpy(cabecalho->desC5, "NOME DA CIDADE: ");
	cabecalho->codC6 = '1';
	strcpy(cabecalho->desC6, "MARCA DO VEICULO: ");
	cabecalho->codC7 = '2';
	strcpy(cabecalho->desC7, "MODELO DO VEICULO: ");
	cabecalho->proxRRN = 0;
	cabecalho->proxByteOffset = 0;
	cabecalho->nroRegRem = 0;
}

void escreverCabecalho(FILE* arquivo, cabecalho_t* cabecalho, char* tipo)
{
	inicializarCabecalho(cabecalho);

	fseek(arquivo, 0, 0);
	fwrite(&cabecalho->status, 1, 1, arquivo);

	if (strcmp(tipo, "tipo1") == 0)
		fwrite(&cabecalho->topoA, sizeof(int), 1, arquivo);
	else if (strcmp(tipo, "tipo2") == 0)
		fwrite(&cabecalho->topoB, sizeof(long int), 1, arquivo);

	fwrite(cabecalho->descricao, 1, 40, arquivo);
	fwrite(cabecalho->desC1, 1, 22, arquivo);
	fwrite(cabecalho->desC2, 1, 19, arquivo);
	fwrite(cabecalho->desC3, 1, 24, arquivo);
	fwrite(cabecalho->desC4, 1, 8, arquivo);
	fwrite(&cabecalho->codC5, 1, 1, arquivo);
	fwrite(cabecalho->desC5, 1, 16, arquivo);
	fwrite(&cabecalho->codC6, 1, 1, arquivo);
	fwrite(cabecalho->desC6, 1, 18, arquivo);
	fwrite(&cabecalho->codC7, 1, 1, arquivo);
	fwrite(cabecalho->desC7, 1, 19, arquivo);

	if (strcmp(tipo, "tipo1") == 0)
		fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
	else if (strcmp(tipo, "tipo2") == 0)
		fwrite(&cabecalho->proxByteOffset, sizeof(long int), 1, arquivo);

	fwrite(&cabecalho->nroRegRem, sizeof(int), 1, arquivo);
}

void atualizarCabecalho(FILE* arquivo, cabecalho_t* cabecalho, char* tipo)
{
	fseek(arquivo, 0, 0);
	fwrite(&cabecalho->status, 1, 1, arquivo);

	if (strcmp(tipo, "tipo1") == 0)
	{
		fseek(arquivo, 174, 0);
		fwrite(&cabecalho->proxRRN, sizeof(int), 1, arquivo);
	}
	else if (strcmp(tipo, "tipo2") == 0)
	{
		fseek(arquivo, 178, 0);
		fwrite(&cabecalho->proxByteOffset, sizeof(long int), 1, arquivo);
	}
	fwrite(&cabecalho->nroRegRem, sizeof(int), 1, arquivo);
}

void lerRegistrocsv(FILE* arquivo, registro_t* registro, char* tipo)
{
	if (strcmp(tipo, "tipo1") == 0)
		registro->tamRegistro = ESTATICOS1;
	else if (strcmp(tipo, "tipo2") == 0)
		registro->tamRegistro = ESTATICOS2;

	registro->sigla[0] = '\0';
	registro->sigla[1] = '\0';

	registro->id = readNumberUntil(arquivo, ',');
	registro->ano = readNumberUntil(arquivo, ',');
	registro->cidade = readUntil(arquivo, ',');
	registro->qtt = readNumberUntil(arquivo, ',');
	readStaticUntil(arquivo, 2, registro->sigla, ',');
	registro->marca = readUntil(arquivo, ',');
	registro->modelo = readUntil(arquivo, ',');

	registro->removido = '0';
	registro->proxA = -1;
	registro->proxB = -1;
	if (registro->cidade == NULL)
	{
		registro->tamCidade = 0;
		registro->tamRegistro -= 5;
	}
	else registro->tamCidade = strlen(registro->cidade);

	if (registro->marca == NULL)
	{
		registro->tamMarca = 0;
		registro->tamRegistro -= 5;
	}
	else registro->tamMarca = strlen(registro->marca);

	if (registro->modelo == NULL)
	{
		registro->tamModelo = 0;
		registro->tamRegistro -= 5;
	}
	else registro->tamModelo = strlen(registro->modelo);

	registro->tamRegistro += registro->tamCidade + registro->tamMarca + registro->tamModelo;
}

void escreverLixo(FILE* arquivo, int tam, char* lixo)
{
	for (int i = 0; i < tam; i++)
	{
		fwrite(lixo, 1, 1, arquivo);
	}
}

void escreverNoArquivo(FILE* arquivo, registro_t* registro, cabecalho_t* cabecalho, char* tipo)
{
	fwrite(&registro->removido, 1, 1, arquivo);
	if (strcmp(tipo, "tipo2") == 0)
		fwrite(&registro->tamRegistro, sizeof(int), 1, arquivo);
	if (strcmp(tipo, "tipo1") == 0)
		fwrite(&registro->proxA, sizeof(int), 1, arquivo);
	else if (strcmp(tipo, "tipo2") == 0)
		fwrite(&registro->proxB, sizeof(long int), 1, arquivo);
	fwrite(&registro->id, sizeof(int), 1, arquivo);
	fwrite(&registro->ano, sizeof(int), 1, arquivo);
	fwrite(&registro->qtt, sizeof(int), 1, arquivo);
	fwrite(registro->sigla, 1, 2, arquivo);
	
	if (registro->cidade != NULL)
	{
		fwrite(&registro->tamCidade, sizeof(int), 1, arquivo);
		fwrite(&cabecalho->codC5, 1, 1, arquivo);
		fwrite(registro->cidade, 1, registro->tamCidade, arquivo);
	}
	
	if (registro->marca != NULL)
	{
		fwrite(&registro->tamMarca, sizeof(int), 1, arquivo);
		fwrite(&cabecalho->codC6, 1, 1, arquivo);
		fwrite(registro->marca, 1, registro->tamMarca, arquivo);
	}
	
	if (registro->modelo != NULL)
	{
		fwrite(&registro->tamModelo, sizeof(int), 1, arquivo);
		fwrite(&cabecalho->codC7, 1, 1, arquivo);
		fwrite(registro->modelo, 1, registro->tamModelo, arquivo);
	}
	
	if (strcmp(tipo, "tipo1") == 0) escreverLixo(arquivo, 97 - registro->tamRegistro	, "$");
}

void liberar(registro_t* r)
{
	free(r->cidade);
	free(r->marca);
	free(r->modelo);
	free(r);
}

void imprimirRegistro(registro_t* r, cabecalho_t* c)
{
	if (r->removido == '1')
	{
		printf("Registro inexistente.\n");
		return;
	}
	printf("%s", c->desC6);
	if (r->marca != NULL)printf("%s\n", r->marca);
	else printf("NAO PREENCHIDO\n");
	printf("%s", c->desC7);
	if (r->modelo != NULL)printf("%s\n", r->modelo);
	else printf("NAO PREENCHIDO\n");
	printf("%s", c->desC5);
	if (r->ano != -1)printf("%d\n", r->ano);
	else printf("NAO PREENCHIDO\n");
	printf("%s", c->desC2);
	if (r->cidade != NULL)printf("%s\n", r->cidade);
	else printf("NAO PREENCHIDO\n");
	printf("%s", c->desC3);
	if (r->qtt != -1)printf("%d\n", r->qtt);
	else printf("NAO PREENCHIDO\n");
	printf("\n");
}

static void lerDescricoes(FILE* fptr, cabecalho_t* cab) {

	fread(&cab->descricao, 40, 1, fptr);
	fread(&cab->desC1, 22, 1, fptr);
	fread(&cab->desC2, 19, 1, fptr);
	fread(&cab->desC3, 24, 1, fptr);
	fread(&cab->desC4, 8, 1, fptr);
	fread(&cab->codC5, 1, 1, fptr);
	fread(&cab->desC5, 16, 1, fptr);
	fread(&cab->codC6, 1, 1, fptr);
	fread(&cab->desC6, 18, 1, fptr);
	fread(&cab->codC7, 1, 1, fptr);
	fread(&cab->desC7, 19, 1, fptr);

}

cabecalho_t* lerCabecalhoTipo1(FILE* fptr) {

	cabecalho_t* cab = malloc(sizeof(cabecalho_t));
	inicializarCabecalho(cab);

	fscanf(fptr, "%c%li", &cab->status, &cab->topoB);
	fseek(fptr,4,SEEK_CUR);
	lerDescricoes(fptr, cab);
	fread(&cab->proxRRN,4,1,fptr);	
	fread(&cab->nroRegRem,4,1,fptr);	

	return cab;

}
cabecalho_t* lerCabecalhoTipo2(FILE* fptr) {

	cabecalho_t* cab = malloc(sizeof(cabecalho_t));
	inicializarCabecalho(cab);

	fscanf(fptr, "%c%li", &cab->status, &cab->topoB);
	lerDescricoes(fptr, cab);
	fscanf(fptr, "%li%i", &cab->proxByteOffset, &cab->nroRegRem);

	return cab;

}

static void leEstaticos(FILE* fptr, registro_t* reg) {

	fread(&reg->id,4,1,fptr);
	fread(&reg->ano,4,1,fptr);
	fread(&reg->qtt,4,1,fptr);	
	fread(&reg->sigla,2,1,fptr);	

}

static void leVariaveis(FILE* fptr, registro_t* reg) {

	char codigo; 
	int tamanho;

	reg->codC5 = reg->codC6 = reg->codC7 = '$';

	for(int i=0 ; i < 3; i++){

		fread(&tamanho,4,1,fptr);	
		fread(&codigo,1,1,fptr);	

		if(codigo == '$'){
			break;
		}

		switch(codigo){
			case '0' : reg->tamCidade = tamanho;
				reg->cidade = malloc(sizeof(char) * tamanho);
				fread(reg->cidade,tamanho, 1, fptr);				
				break;
			case '1' : reg->tamMarca = tamanho;
				reg->marca = malloc(sizeof(char) * tamanho);
				fread(reg->marca,tamanho, 1, fptr);				
				break;
			case '2' : reg->tamModelo = tamanho;
				reg->modelo = malloc(sizeof(char) * tamanho);
				fread(reg->modelo,tamanho, 1, fptr);				
				break;
			default: return; //throwar erro ou algo do tipo?
			break;
		}

		switch (i){
			case 0 : reg->codC5 = codigo;
			break;
			case 1 : reg->codC6 = codigo;
			break; 
			case 2 : reg->codC7 = codigo;
			break; 
		}
	}
}


registro_t *lerRegistroTipo1(FILE *fptr){
	
	registro_t *reg = malloc(sizeof(registro_t));

	fread(&reg->removido,1,1,fptr);
	fread(&reg->prox,4,1,fptr);

	leEstaticos(fptr, reg);
	
	leVariaveis(fptr, reg);

	return reg;
}

registro_t* lerRegistroTipo2(FILE* fptr) {

	registro_t* reg = malloc(sizeof(registro_t));

	fscanf(fptr, "%c%i%li", &reg->removido, &reg->tamRegistro, &reg->proxB);

	leEstaticos(fptr, reg);

	leVariaveis(fptr, reg);

	return reg;

}

void printaRegistro(int tipoReg, registro_t *reg){

	printf("MARCA DO VEICULO: ");
	if(campoEstaPreenchido('1',reg)){
		printf("%.*s\n", reg->tamMarca, reg->marca);
	}else{
		printf("NAO PREENCHIDO\n");
	}

	printf("MODELO DO VEICULO: ");
	if(campoEstaPreenchido('2',reg)){
		printf("%.*s\n", reg->tamModelo, reg->modelo);
	}else{
		printf("NAO PREENCHIDO\n");
	}

	printf("ANO DE FABRICACAO: ");
	if(reg->ano != -1){
		printf("%i\n", reg->ano);
	}else{
		printf("NAO PREENCHIDO\n\n");
	}
	
	printf("NOME DA CIDADE: ");
	if(campoEstaPreenchido('0',reg)){
		printf("%.*s\n", reg->tamCidade, reg->cidade);
	}else{
		printf("NAO PREENCHIDO\n");
	}

	printf("QUANTIDADE DE VEICULOS:");
	if(reg->qtt != -1){
		printf("%i\n", reg->qtt);
	}else{
		printf("NAO PREENCHIDO\n");
	}
	
	printf("\n");
}

void liberaRegistro(int tipoReg, registro_t *reg){

	if(tipoReg == 1){
		if(campoEstaPreenchido('0',reg)){
			free(reg->cidade);
		}
		if(campoEstaPreenchido('1',reg)){
			free(reg->marca);
		}
		if(campoEstaPreenchido('2',reg)){
			free(reg->modelo);
		}

		free(reg);
	}
}

static int campoEstaPreenchido(char codigo, registro_t *reg){
	return reg->codC5 == codigo || reg->codC6 == codigo || reg->codC7 == codigo;
}
void binarioNaTela(char* nomeArquivoBinario) { /* Voc� n�o precisa entender o c�digo dessa fun��o. */

	/* Use essa fun��o para compara��o no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	*  Ela vai abrir de novo para leitura e depois fechar (voc� n�o vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char* mb;
	size_t fl;
	FILE* fs;
	if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb"))) {
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (fun��o binarioNaTela): n�o foi poss�vel abrir o arquivo que me passou para leitura. Ele existe e voc� t� passando o nome certo? Voc� lembrou de fechar ele com fclose depois de usar?\n");
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