#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"

static int campoEstaPreenchido(char codigo, registro_t *reg);

/**
 * @brief Lê uma string de tamanho variável a partir do arquivo 'a', 
 * até que seja lido o caractere delimitador
 *
 * @param a : Arquivo fonte
 * @param delimiter : caractere delimitador
 * @return char* : string lida
 */
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

/**
 * @brief Lê um número inteiro a partir de um arquivo, até que o 
 * caractere delimitador seja encontrado
 *
 * @param a : Arquivo fonte
 * @param delimiter : caractere delimitador
 * @return int : inteiro lido
 */
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

/**
 * @brief Lê uma string estática a partir de um arquivo, até que o 
 * caractere delimitador seja encontrado
 *
 * @param a : Arquivo fonte
 * @param tamanho : tamanho da string estática
 * @param s : ponteiro para a string
 * @param delimiter : caractere delimitador
 */
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

/**
 * @brief Inicializa a struct que contém os valores do cabeçalho
 *
 * @param cabecalho : ponteiro para a struct
 */
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

/**
 * @brief Escreve o cabeçalho em um arquivo binário 
 *
 * @param binario : ponteiro para o arquivo
 * @param cabecalho : struct contendo os dados do cabeçalho
 * @param tipo : tipo do arquivo
 */
void escreverCabecalho(FILE* binario, cabecalho_t* cabecalho, char* tipo)
{
	inicializarCabecalho(cabecalho);

	fseek(binario, 0, 0);
	fwrite(&cabecalho->status, 1, 1, binario);

	if (strcmp(tipo, "tipo1") == TIPOS_IGUAIS)
		fwrite(&cabecalho->topoA, sizeof(int), 1, binario);
	else if (strcmp(tipo, "tipo2") == TIPOS_IGUAIS)
		fwrite(&cabecalho->topoB, sizeof(long int), 1, binario);

	fwrite(cabecalho->descricao, 1, 40, binario);
	fwrite(cabecalho->desC1, 1, 22, binario);
	fwrite(cabecalho->desC2, 1, 19, binario);
	fwrite(cabecalho->desC3, 1, 24, binario);
	fwrite(cabecalho->desC4, 1, 8, binario);
	fwrite(&cabecalho->codC5, 1, 1, binario);
	fwrite(cabecalho->desC5, 1, 16, binario);
	fwrite(&cabecalho->codC6, 1, 1, binario);
	fwrite(cabecalho->desC6, 1, 18, binario);
	fwrite(&cabecalho->codC7, 1, 1, binario);
	fwrite(cabecalho->desC7, 1, 19, binario);

	if (strcmp(tipo, "tipo1") == 0)
		fwrite(&cabecalho->proxRRN, sizeof(int), 1, binario);
	else if (strcmp(tipo, "tipo2") == 0)
		fwrite(&cabecalho->proxByteOffset, sizeof(long int), 1, binario);

	fwrite(&cabecalho->nroRegRem, sizeof(int), 1, binario);
}

/**
 * @brief Atualiza o cabeçalho após alterações feitas na tabela (arquivo binário)
 *
 * @param binario : ponteiro para o arquivo
 * @param cabecalho : ponteiro para a struct que contém os dados do cabeçalho
 * @param tipo : tipo do arquivo
 */
void atualizarCabecalho(FILE* binario, cabecalho_t* cabecalho, char* tipo)
{
	fseek(binario, 0, 0);
	fwrite(&cabecalho->status, 1, 1, binario);

	if (strcmp(tipo, "tipo1") == TIPOS_IGUAIS)
	{
		fseek(binario, 174, 0);
		fwrite(&cabecalho->proxRRN, sizeof(int), 1, binario);
	}
	else if (strcmp(tipo, "tipo2") == TIPOS_IGUAIS)
	{
		fseek(binario, 178, 0);
		fwrite(&cabecalho->proxByteOffset, sizeof(long int), 1, binario);
	}
	fwrite(&cabecalho->nroRegRem, sizeof(int), 1, binario);
}

/**
 * @brief Lê um único registro a partir de um arquivo csv
 *
 * @param csv : Arquivo fonte
 * @param registro : Ponteiro para a strutc na qual serão armazenados
 * os dados lidos
 * @param tipo : Tipo do arquivo
 */
void lerRegistrocsv(FILE* csv, registro_t* registro, char* tipo)
{
	if (strcmp(tipo, "tipo1") == TIPOS_IGUAIS)
		registro->tamRegistro = ESTATICOS1;
	else if (strcmp(tipo, "tipo2") == TIPOS_IGUAIS)
		registro->tamRegistro = ESTATICOS2;

	registro->sigla[0] = '\0';
	registro->sigla[1] = '\0';

	registro->id = readNumberUntil(csv, ',');
	registro->ano = readNumberUntil(csv, ',');
	registro->cidade = readUntil(csv, ',');
	registro->qtt = readNumberUntil(csv, ',');
	readStaticUntil(csv, 2, registro->sigla, ',');
	registro->marca = readUntil(csv, ',');
	registro->modelo = readUntil(csv, ',');

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

/**
 * @brief Escreve uma quantidade solicitada de lixo em arquivo binário
 *
 * @param binario : Ponteiro para o arquivo
 * @param tam : Quantidade de lixo a ser escrita
 * @param lixo : String que indica o lixo
 */
void escreverLixo(FILE* binario, int tam, char* lixo)
{
	for (int i = 0; i < tam; i++)
	{
		fwrite(lixo, 1, 1, binario);
	}
}

/**
 * @brief Escreve um único registro em um arquivo binário
 *
 * @param binario : Ponteiro para o arquivo
 * @param registro : Ponteiro para a struct que contém os dados
 * do registro
 * @param cabecalho : Ponteiro para a struct que contém os dados
 * do cabeçalho
 * @param tipo : Tipo do arquivo
 */
void escreverNoArquivo(FILE* binario, registro_t* registro, cabecalho_t* cabecalho, char* tipo)
{
	fwrite(&registro->removido, 1, 1, binario);
	if (strcmp(tipo, "tipo2") == TIPOS_IGUAIS)
		fwrite(&registro->tamRegistro, sizeof(int), 1, binario);
	if (strcmp(tipo, "tipo1") == TIPOS_IGUAIS)
		fwrite(&registro->proxA, sizeof(int), 1, binario);
	else if (strcmp(tipo, "tipo2") == TIPOS_IGUAIS)
		fwrite(&registro->proxB, sizeof(long int), 1, binario);
	fwrite(&registro->id, sizeof(int), 1, binario);
	fwrite(&registro->ano, sizeof(int), 1, binario);
	fwrite(&registro->qtt, sizeof(int), 1, binario);
	fwrite(registro->sigla, 1, 2, binario);

	if (registro->cidade != NULL)
	{
		fwrite(&registro->tamCidade, sizeof(int), 1, binario);
		fwrite(&cabecalho->codC5, 1, 1, binario);
		fwrite(registro->cidade, 1, registro->tamCidade, binario);
	}

	if (registro->marca != NULL)
	{
		fwrite(&registro->tamMarca, sizeof(int), 1, binario);
		fwrite(&cabecalho->codC6, 1, 1, binario);
		fwrite(registro->marca, 1, registro->tamMarca, binario);
	}

	if (registro->modelo != NULL)
	{
		fwrite(&registro->tamModelo, sizeof(int), 1, binario);
		fwrite(&cabecalho->codC7, 1, 1, binario);
		fwrite(registro->modelo, 1, registro->tamModelo, binario);
	}

	if (strcmp(tipo, "tipo1") == TIPOS_IGUAIS) escreverLixo(binario, 97 - registro->tamRegistro, "$");
}

/**
 * @brief Libera a memória alocada para armazenar os dados
 * de um registro
 *
 * @param r : Ponteiro para a struct que contém os dados do 
 * registro
 */
void liberar(registro_t* r)
{
	free(r->cidade);
	free(r->marca);
	free(r->modelo);
	free(r);
}

/**
 * @brief Imprime, de acordo com as especificações, um único registro
 *
 * @param r : Ponteiro para a struct com os dados do registro
 * @param c : Ponteiro para a struct com os dados do cabeçalho
 */
void imprimirRegistro(registro_t* r, cabecalho_t* c)
{
	if (r->removido == REGISTRO_REMOVIDO)
	{
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

	fread(&cab->status,1,1,fptr);	
	fread(&cab->topoB,8,1,fptr);	

	lerDescricoes(fptr, cab);

	fread(&cab->proxByteOffset,8,1,fptr);	
	fread(&cab->nroRegRem,4,1,fptr);	

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

		if(feof(fptr)) return;

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


registro_t *lerRegistro(int tipo, FILE *fptr){
	
	registro_t *reg = malloc(sizeof(registro_t));

	fread(&reg->removido,1,1,fptr);
	if(tipo == 1){
		fread(&reg->proxA,4,1,fptr);
	}else{
		fread(&reg->tamRegistro,4,1,fptr);
		fread(&reg->proxB,8,1,fptr);
	}

	leEstaticos(fptr, reg);
	
	leVariaveis(fptr, reg);

	return reg;
}

void printaRegistro(registro_t *reg){

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
		printf("NAO PREENCHIDO\n");
	}
	
	printf("NOME DA CIDADE: ");
	if(campoEstaPreenchido('0',reg)){
		printf("%.*s\n", reg->tamCidade, reg->cidade);
	}else{
		printf("NAO PREENCHIDO\n");
	}

	printf("QUANTIDADE DE VEICULOS: ");
	if(reg->qtt != -1){
		printf("%i\n", reg->qtt);
	}else{
		printf("NAO PREENCHIDO\n");
	}
	
	printf("\n");
}

void liberaRegistro(registro_t *reg){

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

static int campoEstaPreenchido(char codigo, registro_t *reg){
	return reg->codC5 == codigo || reg->codC6 == codigo || reg->codC7 == codigo;
}


void inicializaStructBusca(buscaParams_t *busca){
	busca->filtros = malloc(sizeof(char *) * NUM_PARAMETROS);

	for(int i =0 ; i < NUM_PARAMETROS; i++){
		busca->ehBuscado[i] = 0;
		busca->filtros[i] = malloc(sizeof(char)*11);
	}

	busca->filtros[0] = "id";
	busca->filtros[1] = "ano";
	busca->filtros[2] = "quantidade";
	busca->filtros[3] = "sigla";
	busca->filtros[4] = "cidade";
	busca->filtros[5] = "marca";
	busca->filtros[6] = "modelo";
}

void liberaStructBusca(buscaParams_t *busca){

	for(int i =0; i < NUM_PARAMETROS; i++){
		free(busca->filtros[i]);
	}
	free(busca->filtros);
}

static int validaFiltroInt(registro_t *reg, buscaParams_t *busca, int filtro){
	int val;
	switch (filtro){
		case 0: val = reg->id;
			break;
		case 1: val = reg->ano;
			break;
		case 2: val = reg->qtt;
	}

	return atoi(busca->filtros[filtro]) == val;
}

static int strcmpWLen(char* num1, char* num2, int sz1,int sz2){
	if(sz2 != sz1){
		return 0;
	}

	for(int i =0; i < sz1; i++){
		if(num1[i] != num2[i]){
			return 0;
		}
	}

	return 1;
}

static int validaFiltroStr(registro_t *reg, buscaParams_t *busca, int filtro){
	char *val = NULL;
	int tam = 0;

	if(filtro > 3 && !campoEstaPreenchido(filtro + 44,reg)){
		return 0;
	}

	switch(filtro){
		case 3: val = reg->sigla;
			break;
		case 4: val = reg->cidade; 
				tam = reg->tamCidade;
			break;
		case 5: val = reg->marca;
				tam = reg->tamMarca;
			break;
		case 6: val = reg->modelo;
				tam = reg->tamModelo;
			break;
	}
	char *ref = busca->filtros[filtro];
	
	return strcmpWLen(ref,val,tam,strlen(ref));
}

int ehValidoFiltro(registro_t *reg, buscaParams_t *busca){

	for(int i =0; i < NUM_PARAMETROS; i++){
		if(busca->ehBuscado[i] && i < 3){
			if(!validaFiltroInt(reg,busca,i)){
				return 0;
			}
		}else if(busca->ehBuscado[i]){
			if(!validaFiltroStr(reg,busca,i)){
				return 0;
			}
		}
	}

	return 1;
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
