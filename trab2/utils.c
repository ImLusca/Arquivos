#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "funcionalidades.h"

static int campoEstaPreenchido(char codigo, registro_t *reg);
static char *readStrQuote(FILE *fptr);
/**
 * @brief Lê uma string de tamanho variável a partir do arquivo 'a',
 * até que seja lido o caractere delimitador
 *
 * @param a : Arquivo fonte
 * @param delimiter : caractere delimitador
 * @return char* : string lida
 */
char *readUntil(FILE *a, char delimiter)
{
	char *word = NULL;

	int i = 0;
	do
	{
		word = realloc(word, (i + 1) * sizeof(char));
		char newchr = fgetc(a);
		if (newchr == '\"')
			newchr = fgetc(a);
		word[i] = newchr;

		if (word[i] == '\r')
			word[i] = fgetc(a);

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
int readNumberUntil(FILE *a, char delimiter)
{
	char *num = readUntil(a, delimiter);
	if (num == NULL || num[0] == '\0')
	{
		if (num != NULL)
			free(num);
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
void readStaticUntil(FILE *a, int tamanho, char *s, char delimiter)
{
	int d = 0;
	for (int i = 0; i < tamanho; i++)
	{
		char newchr = fgetc(a);
		if (newchr == '\"')
			newchr = fgetc(a);
		s[i] = newchr;
		if (s[i] == delimiter)
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
	if (d == 0)
		delim = fgetc(a);
}

/**
 * @brief Inicializa a struct que contém os valores do cabeçalho
 *
 * @param cabecalho : ponteiro para a struct
 */
void inicializarCabecalho(cabecalho_t *cabecalho)
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
void escreverCabecalho(FILE *binario, cabecalho_t *cabecalho, char *tipo)
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
void atualizarCabecalho(FILE *binario, cabecalho_t *cabecalho, char *tipo)
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
void lerRegistrocsv(FILE *csv, registro_t *registro, char *tipo)
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
	else
		registro->tamCidade = strlen(registro->cidade);

	if (registro->marca == NULL)
	{
		registro->tamMarca = 0;
		registro->tamRegistro -= 5;
	}
	else
		registro->tamMarca = strlen(registro->marca);

	if (registro->modelo == NULL)
	{
		registro->tamModelo = 0;
		registro->tamRegistro -= 5;
	}
	else
		registro->tamModelo = strlen(registro->modelo);

	registro->tamRegistro += registro->tamCidade + registro->tamMarca + registro->tamModelo;
}

/**
 * @brief Escreve uma quantidade solicitada de lixo em arquivo binário
 *
 * @param binario : Ponteiro para o arquivo
 * @param tam : Quantidade de lixo a ser escrita
 * @param lixo : String que indica o lixo
 */
void escreverLixo(FILE *binario, int tam, char *lixo)
{
	for (int i = 0; i < tam; i++)
	{
		fwrite(lixo, 1, 1, binario);
	}
}

/**
 * @brief Atribui o código ao campo codC* correto
 *
 * @param tam : Qunatidade de códigos já atribuidos ao registro fornecido
 * @param reg : Ponteiro para o registro
 * @param cod : Codigo do campo que será atribuído ao campo codC*
 */
void registraCodigos(int numCodigosUtilizados, registro_t *reg, int cod)
{
	switch (numCodigosUtilizados)
	{
	case 0:
		reg->codC5 = cod;
		break;
	case 1:
		reg->codC6 = cod;
		break;
	case 2:
		reg->codC7 = cod;
		break;
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
void escreverNoArquivo(FILE *binario, registro_t *registro, cabecalho_t *cabecalho, char *tipo)
{
	int tamanhoRegAntigo;
	fwrite(&registro->removido, 1, 1, binario);
	if(!strcmp(tipo, "tipo2")){
		fread(&tamanhoRegAntigo,4,1,binario);
		fseek(binario, -4, SEEK_CUR);
	}
	if (strcmp(tipo, "tipo2") == TIPOS_IGUAIS)
		fwrite(&tamanhoRegAntigo, sizeof(int), 1, binario);
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

	if (strcmp(tipo, "tipo1") == TIPOS_IGUAIS)
		escreverLixo(binario, 97 - registro->tamRegistro, "$");
	else{
		escreverLixo(binario, tamanhoRegAntigo - registro->tamRegistro + 5, "$");
	}
}

/**
 * @brief Libera a memória alocada para armazenar os dados
 * de um registro
 *
 * @param r : Ponteiro para a struct que contém os dados do
 * registro
 */
void liberar(registro_t *r)
{
	free(r->cidade);
	free(r->modelo);
	free(r->marca);
	free(r);
}

/**
 * @brief lê campos d descrição de um arquivo binário e atribui ao cabeçalho fornecido
 * @param fptr ponteiro para o arquivo binário
 * @param cab ponteiro para o cabeçalho
 */
static void lerDescricoes(FILE *fptr, cabecalho_t *cab)
{
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

/**
 * @brief lê um arquivo binário e retorna o cabeçalho inicializado
 * @param tipo tipo do arquivo que está sendo lido
 * @param fptr ponteiro para o arquivo binário
 */
cabecalho_t *lerCabecalho(int tipo, FILE *fptr)
{

	cabecalho_t *cab = malloc(sizeof(cabecalho_t));
	inicializarCabecalho(cab);
	fread(&cab->status, 1, 1, fptr);

	if (tipo == 1)
	{
		fread(&cab->topoA, 4, 1, fptr);
	}
	else if (tipo == 2)
	{
		fread(&cab->topoB, 8, 1, fptr);
	}

	lerDescricoes(fptr, cab);

	if (tipo == 1)
	{
		fread(&cab->proxRRN, 4, 1, fptr);
	}
	else
	{
		fread(&cab->proxByteOffset, 8, 1, fptr);
	}

	fread(&cab->nroRegRem, 4, 1, fptr);

	return cab;
}

/**
 * @brief lê campos estáticos de um arquivo binário e atribui a um registro
 * @param fptr ponteiro para o arquivo binário
 * @param reg ponteiro para o registro que recebe-ra os dados
 */
static void leEstaticos(FILE *fptr, registro_t *reg)
{

	fread(&reg->id, 4, 1, fptr);
	fread(&reg->ano, 4, 1, fptr);
	fread(&reg->qtt, 4, 1, fptr);
	fread(&reg->sigla, 2, 1, fptr);
}

/**
 * @brief lê campos variáveis de um arquivo binário e atribui a um registro
 * @param fptr ponteiro para o arquivo binário
 * @param reg ponteiro para o registro que recebe-ra os dados
 */
static void leVariaveis(FILE *fptr, registro_t *reg)
{

	char codigo;
	int tamanho;

	reg->codC5 = reg->codC6 = reg->codC7 = '$';

	for (int i = 0; i < 3; i++)
	{

		fread(&tamanho, 4, 1, fptr);
		fread(&codigo, 1, 1, fptr);

		if (feof(fptr))
			return;

		if (codigo == '$')
		{
			break;
		}

		switch (codigo)
		{
		case '0':
			reg->tamCidade = tamanho;
			reg->cidade = malloc(sizeof(char) * tamanho);
			fread(reg->cidade, tamanho, 1, fptr);
			break;
		case '1':
			reg->tamMarca = tamanho;
			reg->marca = malloc(sizeof(char) * tamanho);
			fread(reg->marca, tamanho, 1, fptr);
			break;
		case '2':
			reg->tamModelo = tamanho;
			reg->modelo = malloc(sizeof(char) * tamanho);
			fread(reg->modelo, tamanho, 1, fptr);
			break;
		default:
			return;
			break;
		}

		registraCodigos(i, reg, codigo);
	}
}

/**
 * @brief lê um registro e retorna a struct registro_t com os dados recebido
 * @param tipo tipo 1 ou 2 corresponde ao tipo do arquivo
 * @param fptr filepointer do arquivo binário que está sendo lido
 */
registro_t *lerRegistro(int tipo, FILE *fptr)
{

	registro_t *reg = malloc(sizeof(registro_t));

	fread(&reg->removido, 1, 1, fptr);
	if (tipo == 1)
	{
		fread(&reg->proxA, 4, 1, fptr);
	}
	else
	{
		fread(&reg->tamRegistro, 4, 1, fptr);
		fread(&reg->proxB, 8, 1, fptr);
	}

	leEstaticos(fptr, reg);

	leVariaveis(fptr, reg);

	return reg;
}

/**
 * @brief Imprime, de acordo com as especificações, um único registro
 *
 * @param r : Ponteiro para a struct com os dados do registro
 * @param c : Ponteiro para a struct com os dados do cabeçalho
 */
void imprimirRegistro(registro_t *reg, cabecalho_t *cab)
{

	printf("%s", cab->desC6);
	if (campoEstaPreenchido('1', reg))
	{
		printf("%.*s\n", reg->tamMarca, reg->marca);
	}
	else
	{
		printf("NAO PREENCHIDO\n");
	}

	printf("%s", cab->desC7);
	if (campoEstaPreenchido('2', reg))
	{
		printf("%.*s\n", reg->tamModelo, reg->modelo);
	}
	else
	{
		printf("NAO PREENCHIDO\n");
	}

	printf("%s", cab->desC2);
	if (reg->ano != -1)
	{
		printf("%i\n", reg->ano);
	}
	else
	{
		printf("NAO PREENCHIDO\n");
	}

	printf("%s", cab->desC5);
	if (campoEstaPreenchido('0', reg))
	{
		printf("%.*s\n", reg->tamCidade, reg->cidade);
	}
	else
	{
		printf("NAO PREENCHIDO\n");
	}

	printf("%s", cab->desC3);
	if (reg->qtt != -1)
	{
		printf("%i\n", reg->qtt);
	}
	else
	{
		printf("NAO PREENCHIDO\n");
	}

	printf("\n");
}

/**
 * @brief Libera a memória alocada para armazenar os dados
 * de um registro
 *
 * @param reg : Ponteiro para a struct que contém os dados do
 * registro
 */
void liberaRegistro(registro_t *reg)
{

	if (campoEstaPreenchido('0', reg))
	{
		free(reg->cidade);
	}
	if (campoEstaPreenchido('1', reg))
	{
		free(reg->marca);
	}
	if (campoEstaPreenchido('2', reg))
	{
		free(reg->modelo);
	}

	free(reg);
}

/**
 * @brief informa se o campo do código recebeu alugm valor
 * ou não foi alocado
 */
static int campoEstaPreenchido(char codigo, registro_t *reg)
{
	return reg->codC5 == codigo || reg->codC6 == codigo || reg->codC7 == codigo;
}

static void RecebeFiltros(int numFiltros, buscaParams_t *busca, FILE *arquivo)
{
	for (int i = 0; i < numFiltros; i++)
	{
		char *nomeCampo = readUntil(arquivo, ' ');
		for (int j = 0; j < NUM_PARAMETROS; j++)
		{
			if (!strcmp(busca->filtros[j], nomeCampo))
			{
				busca->ehBuscado[j] = 1;

				if (j < 3)
				{
					busca->filtros[j] = readUntil(arquivo, '\n');
				}
				else
				{
					getc(arquivo);
					busca->filtros[j] = readUntil(arquivo, '"');
					getc(arquivo);
				}
			}
		}
	}
}

/**
 * @brief Inicializa os campos da struct de filtro para
 * ser utilizada no select com where
 */
void inicializaStructBusca(buscaParams_t *busca, int numFiltros, FILE *fptr)
{
	busca->filtros = malloc(sizeof(char *) * NUM_PARAMETROS);

	for (int i = 0; i < NUM_PARAMETROS; i++)
	{
		busca->ehBuscado[i] = 0;
		busca->filtros[i] = malloc(sizeof(char) * 11);
	}

	busca->filtros[0] = "id";
	busca->filtros[1] = "ano";
	busca->filtros[2] = "quantidade";
	busca->filtros[3] = "sigla";
	busca->filtros[4] = "cidade";
	busca->filtros[5] = "marca";
	busca->filtros[6] = "modelo";

	RecebeFiltros(numFiltros, busca, fptr);
}

/**
 * @brief Faz a comparação entre os campos do tipo inteiro dos registros
 * e do filtro.
 */
static int validaFiltroInt(registro_t *reg, buscaParams_t *busca, int filtro)
{
	int val;
	switch (filtro)
	{
	case 0:
		val = reg->id;
		break;
	case 1:
		val = reg->ano;
		break;
	case 2:
		val = reg->qtt;
	}

	return atoi(busca->filtros[filtro]) == val;
}
/**
 * @brief Como os registros são armazenados sem o caracter terminador,
 * precisamos implementar um strcmp que itera pelo tamanho informado.
 */
static int strcmpWLen(char *num1, char *num2, int sz1, int sz2)
{
	if (sz2 != sz1)
	{
		return 0;
	}

	for (int i = 0; i < sz1; i++)
	{
		if (num1[i] != num2[i])
		{
			return 0;
		}
	}

	return 1;
}

/**
 * @brief Faz a validação se os campos de string estão de acordo
 * com os filtros. O parâmetro filtro indica qual campo deseja ser filtrado
 * e os parâmetros reg e busca são ponteiros para o registro e filtro respectivamente
 */
static int validaFiltroStr(registro_t *reg, buscaParams_t *busca, int filtro)
{
	char *val = NULL;
	int tam = 0;

	if (filtro > 3 && !campoEstaPreenchido(filtro + 44, reg))
	{
		return 0;
	}

	switch (filtro)
	{
	case 3:
		val = reg->sigla;
		break;
	case 4:
		val = reg->cidade;
		tam = reg->tamCidade;
		break;
	case 5:
		val = reg->marca;
		tam = reg->tamMarca;
		break;
	case 6:
		val = reg->modelo;
		tam = reg->tamModelo;
		break;
	}
	char *ref = busca->filtros[filtro];

	return strcmpWLen(ref, val, tam, strlen(ref));
}

/**
 * @brief retorna se o registro está de acordo com os
 * filtros estabelecidos
 */
int ehValidoFiltro(registro_t *reg, buscaParams_t *busca)
{

	for (int i = 0; i < NUM_PARAMETROS; i++)
	{
		if (busca->ehBuscado[i] && i < 3)
		{
			if (!validaFiltroInt(reg, busca, i))
			{
				return 0;
			}
		}
		else if (busca->ehBuscado[i])
		{
			if (!validaFiltroStr(reg, busca, i))
			{
				return 0;
			}
		}
	}

	return 1;
}

typedef void (*acaoFiltro)(cabecalho_t *cab, registro_t *reg, FILE *fptr);

void acaoFiltro_imprimirRegistro(cabecalho_t *cab, registro_t *reg, FILE *fptr)
{
	imprimirRegistro(reg, cab);
}

void percorreArquivoAplicandofiltro(FILE *fptr, buscaParams_t *busca, int tArquivo, cabecalho_t *cab, int final, acaoFiltro acao)
{

	do
	{
		// Lendo registro
		registro_t *reg = lerRegistro(tArquivo, fptr);

		// Se registro não estiver removido e bater com o filtro, executa ação
		if (reg->removido == '0')
		{
			if (ehValidoFiltro(reg, busca))
			{
				acao(cab, reg, fptr);
			}
		}

		// Lendo todos caracteres de lixo
		while (fgetc(fptr) == '$');
		// Voltando uma posição para alinhar com primeiro caracter válido
		fseek(fptr, -1, SEEK_CUR);

		liberaRegistro(reg);
	} while (ftell(fptr) < final);
}

/**
 * @brief Libera a memória alocada nas funções
 * o parâmetro delCab indica se o Cabeçalho está alocado e pode ser liberado sem ter segfault
 * os demais parâmetros são ponteiros para os endereços que serão liberados
 */
void liberaMemSelect(int delCab, char *tArq, char *aEnt, FILE *fptr, cabecalho_t *cab)
{
	free(tArq);
	free(aEnt);
	fclose(fptr);
	if (delCab)
	{
		free(cab);
	}
}

long int retornaIndiceDisponivel(cabecalho_t *cab, FILE *fptr, int tipoArq, int *finalDoArquivo)
{

	if (tipoArq == 1)
	{
		if (cab->topoA == -1)
		{
			*finalDoArquivo = 1;
			return TAM_CABECALHO1 + cab->proxRRN * TAM_REG;
		}
		*finalDoArquivo = 0;
		return TAM_CABECALHO1 + cab->topoA * TAM_REG;
	}

	if (cab->topoB == -1)
	{
		*finalDoArquivo = 1;
		return cab->proxByteOffset;
	}
	*finalDoArquivo = 0;
	return cab->topoB;
}

// Quebrar essa função em outras
registro_t *lerRegistroStdin(int tipoRegistro)
{

	registro_t *registro = malloc(sizeof(registro_t));
	char *tmpSigla = malloc(sizeof(char) * 2);

	registro->removido = '0';
	registro->proxA = -1;
	registro->proxB = -1;

	char tmp = fgetc(stdin);
	ungetc(tmp, stdin);
	
	registro->id = readNumberUntil(stdin, ' ');
	registro->ano = readNumberUntil(stdin, ' ');
	registro->qtt = readNumberUntil(stdin, ' ');

	if(registro->ano == 0){registro->ano = -1;}
	if(registro->qtt == 0){registro->qtt = -1;}

	tmpSigla = readStrQuote(stdin);
	if (tmpSigla)
	{
		registro->sigla[0] = tmpSigla[0];
		registro->sigla[1] = tmpSigla[1];
	}
	else
	{
		registro->sigla[0] = '$';
		registro->sigla[1] = '$';
	}

	registro->cidade = readStrQuote(stdin);
	registro->marca = readStrQuote(stdin);
	registro->modelo = readStrQuote(stdin);

	registro->tamRegistro = tipoRegistro == 1 ? ESTATICOS1 : 42;

	registro->codC5 = registro->codC6 = registro->codC7 = '$';
	int contadorCodigos = 0;

	if (registro->cidade == NULL)
	{
		registro->tamCidade = 0;
		registro->tamRegistro -= 5;
	}
	else
	{
		registro->tamCidade = strlen(registro->cidade);
		registro->tamRegistro += registro->tamCidade;
		registraCodigos(contadorCodigos++, registro, '0');
	}

	if (registro->marca == NULL)
	{
		registro->tamMarca = 0;
		registro->tamRegistro -= 5;
	}
	else
	{
		registro->tamMarca = strlen(registro->marca);
		registro->tamRegistro += registro->tamMarca;
		registraCodigos(contadorCodigos++, registro, '1');
	}

	if (registro->modelo == NULL)
	{
		registro->tamModelo = 0;
		registro->tamRegistro -= 5;
	}
	else
	{
		registro->tamModelo = strlen(registro->modelo);
		registro->tamRegistro += registro->tamModelo;
		registraCodigos(contadorCodigos++, registro, '2');
	}

	char descartN = fgetc(stdin);
	free(tmpSigla);

	return registro;
}

void insereRegistroRRN(registro_t *reg, cabecalho_t *cab, FILE *fptr, int posicaoInsercao, int fimArquivo)
{

	fseek(fptr, posicaoInsercao, SEEK_SET);

	if (!fimArquivo)
	{
		// verificando se ele está deletado
		if (fgetc(fptr) != REGISTRO_REMOVIDO)
		{
			printf("Falha no processamento do arquivo.");
		}
		// Armazenando a posição do próx RRN livre
		fread(&cab->topoA, 4, 1, fptr);
		fseek(fptr, -5, SEEK_CUR);
	}

	escreverNoArquivo(fptr, reg, cab, "tipo1");

	if (fimArquivo)
	{
		cab->proxRRN += 1;
	}
	cab->nroRegRem -= 1;
}

void insereRegistroOffset(registro_t *reg, cabecalho_t *cab, FILE *fptr, long int posicaoInsercao, int fimArquivo)
{

	fseek(fptr, posicaoInsercao, SEEK_SET);

	long int proxbyteOffset;

	if (!fimArquivo)
	{

		// verificando se ele está deletado
		if (fgetc(fptr) != REGISTRO_REMOVIDO)
		{
			printf("Falha no processamento do arquivo.");
		}
		fseek(fptr, 4, SEEK_CUR);

		fread(&cab->topoB, 8, 1, fptr);
		fseek(fptr, -13, SEEK_CUR);
	}
	else
	{
		cab->proxByteOffset += reg->tamRegistro;
	}

	escreverNoArquivo(fptr, reg, cab, "tipo2");

	cab->nroRegRem -= 1;
}

void atualizaCabecariot1(cabecalho_t *cab, FILE *fptr)
{
	fseek(fptr, 1, SEEK_SET);
	fwrite(&cab->topoA, 4, 1, fptr);

	fseek(fptr, 174, SEEK_SET);
	fwrite(&cab->proxRRN, 4, 1, fptr);
	fwrite(&cab->nroRegRem, 4, 1, fptr);
}

void atualizaCabecariot2(cabecalho_t *cab, FILE *fptr)
{
	fseek(fptr, 1, SEEK_SET);
	fwrite(&cab->topoB, 8, 1, fptr);

	fseek(fptr, 178, SEEK_SET);
	fwrite(&cab->proxByteOffset, 8, 1, fptr);
	fwrite(&cab->nroRegRem, 4, 1, fptr);
}

void inserirRegistroLinkedList(FILE *fptr, long int noAnterior, long int proximoNo, long int posRegistro){

	//Gravando no nó anterior
	fseek(fptr, noAnterior + 5, SEEK_SET);
	fwrite(&posRegistro,8,1,fptr);
	//Gravando no novo nó
	fseek(fptr,posRegistro + 5,SEEK_SET);
	fwrite(&proximoNo,8,1,fptr);

}

void aplicaWorstFit(int tamanhoNovo, cabecalho_t *cab, FILE *fptr, long int comecoLL)
{
	long int posNovoReg = ftell(fptr);
	// Criar novo registradorVazioDeletado com flag 0 e espaço livre
	fwrite("0", 1, 1, fptr);
	fwrite(&tamanhoNovo, 4, 1, fptr);

	long int noAnterior;
	long int noLinkedList = comecoLL;

	if(noLinkedList == -1){
		inserirRegistroLinkedList(fptr,noAnterior,noLinkedList,posNovoReg);
		return;
	}

	fseek(fptr, noLinkedList, SEEK_SET);
	registro_t *reg = lerRegistro(2,fptr);

	//Verifica se o novo header deve ser oque era o segundo maior item da lista
	//ou se o resto do antigo maior registro ainda é maior que o segundo maior espaço livre
	if(tamanhoNovo < reg->tamRegistro){
		cab->proxByteOffset = comecoLL;
		noAnterior = comecoLL;
		noLinkedList = reg->proxB;
		liberaRegistro(reg);
	}else{
		cab->proxByteOffset = posNovoReg;
		liberaRegistro(reg);
		return;
	}

	// encaixar ele na linked list
	for(;;){

		//Se chegou no final da lista, insere nela
		if(noLinkedList == -1){
			inserirRegistroLinkedList(fptr,noAnterior,noLinkedList,posNovoReg);
			break;
		}

		//Vai até o próximo nó, instancia ele e veirifica se é maior que o novo espaço disponível
		fseek(fptr, noLinkedList, SEEK_SET);

		registro_t *reg = lerRegistro(2,fptr);

		if(reg->tamRegistro < tamanhoNovo){
			inserirRegistroLinkedList(fptr,noAnterior,noLinkedList,posNovoReg);
			liberaRegistro(reg);
			break;
		}

		noAnterior = noLinkedList;
		noLinkedList = reg->proxB;
		liberaRegistro(reg);
	}
}

void atualizaArquivoIndexacaoTipo1(FILE *arqIndex, int id, int RRN){


}

void insereRegistroNoArquivo(registro_t *reg, cabecalho_t *cab, FILE *fptr, int tipoArquivo, FILE *fptrIndex)
{
	int flagFimDeArquivo = 0;
	if (tipoArquivo == 1)
	{
		int posicaoInsercao = (int)retornaIndiceDisponivel(cab, fptr, 1, &flagFimDeArquivo);
		insereRegistroRRN(reg, cab, fptr, posicaoInsercao, flagFimDeArquivo);
		atualizaArquivoIndexacaoTipo1(fptrIndex,reg->id,);
		return;
	}

	long int posicaoInsercao = retornaIndiceDisponivel(cab, fptr, 2, &flagFimDeArquivo);
	insereRegistroOffset(reg, cab, fptr, posicaoInsercao, flagFimDeArquivo);

}

static char *readStrQuote(FILE *fptr)
{

	char tmpchr;
	char nulo[] = {'N', 'U', 'L', 'O'};
	int contagemNulo = 0;
	// Lê tudo até o primeiro "
	while ((tmpchr = fgetc(fptr)) != '\"')
	{
		if (tmpchr == nulo[contagemNulo])
		{
			contagemNulo++;
		}
		else
		{
			contagemNulo = 0;
		}

		if (contagemNulo == 4)
		{
			return NULL;
		}
	};
	tmpchr = fgetc(fptr);

	int i = 0;
	char *palavra = NULL;

	do
	{
		palavra = realloc(palavra, (i + 1) * sizeof(char));
		palavra[i++] = tmpchr;
	} while ((tmpchr = fgetc(fptr)) != '\"');

	palavra[i] = '\0';

	return palavra;
}

void binarioNaTela(char *nomeArquivoBinario)
{ /* Voc� n�o precisa entender o c�digo dessa fun��o. */

	/* Use essa fun��o para compara��o no run.codes. Lembre-se de ter fechado (fclose) o arquivo anteriormente.
	 *  Ela vai abrir de novo para leitura e depois fechar (voc� n�o vai perder pontos por isso se usar ela). */

	unsigned long i, cs;
	unsigned char *mb;
	size_t fl;
	FILE *fs;
	if (nomeArquivoBinario == NULL || !(fs = fopen(nomeArquivoBinario, "rb")))
	{
		fprintf(stderr, "ERRO AO ESCREVER O BINARIO NA TELA (fun��o binarioNaTela): n�o foi poss�vel abrir o arquivo que me passou para leitura. Ele existe e voc� t� passando o nome certo? Voc� lembrou de fechar ele com fclose depois de usar?\n");
		return;
	}
	fseek(fs, 0, SEEK_END);
	fl = ftell(fs);
	fseek(fs, 0, SEEK_SET);
	mb = (unsigned char *)malloc(fl);
	fread(mb, 1, fl, fs);

	cs = 0;
	for (i = 0; i < fl; i++)
	{
		cs += (unsigned long)mb[i];
	}
	printf("%lf\n", (cs / (double)100));
	free(mb);
	fclose(fs);
}

int buscaBinaria(int chave, registroIndice_t* vetor, int esq, int dir)
{
	if (dir >= esq) 
	{
		int mid = esq + (dir - esq) / 2;

		if (vetor[mid].id == chave)
			return mid;

		if (vetor[mid].id > chave)
			return buscaBinaria(chave, vetor, esq, mid - 1);

		return buscaBinaria(chave, vetor, mid + 1, dir);
	}
	return -1;
}
