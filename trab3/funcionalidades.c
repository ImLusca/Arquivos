﻿#include "funcionalidades.h"
#include "utils.h"
#include "indexUtils.h"
#include "fileUtils.h"
#include "bTree.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief Equivalente ao comando SQL, cria uma tabela (arquivo binario)
 * de registros, a partir de um arquivo csv dado como entrada
 */
void createTable()
{
    char *tipoArquivo = readUntil(stdin, ' ');
    char *arquivoEntrada = readUntil(stdin, ' ');
    char *arquivoSaida = readUntil(stdin, '\n');

    int tipo = 0;
    if (strcmp(tipoArquivo, "tipo1") == TIPOS_IGUAIS)
        tipo = 1;
    else if (strcmp(tipoArquivo, "tipo2") == TIPOS_IGUAIS)
        tipo = 2;

    FILE *entrada = fopen(arquivoEntrada, "r");
    FILE *saida = fopen(arquivoSaida, "wb");

    if (confereArquivos(entrada, saida) == false)
    {
        liberaStrings(tipoArquivo, arquivoEntrada, arquivoSaida);
        return;
    }

    cabecalho_t *cabecalho = malloc(sizeof(cabecalho_t));
    cabecalho->status = ARQUIVO_INCONSISTENTE;
    escreverCabecalho(saida, cabecalho, tipo);
    if (tipo == 2)
        cabecalho->proxByteOffset = TAM_CABECALHO2;

    char buffer;
    int count = 0;
    fseek(entrada, CABECALHO_CSV, 0);
    while (fread(&buffer, 1, 1, entrada) != 0)
    {
        registro_t *registro = malloc(sizeof(registro_t));
        ungetc(buffer, entrada);
        lerRegistrocsv(entrada, registro, tipo);
        escreverNoArquivo(saida, registro, cabecalho, tipo);
        count++;

        liberar(registro);
    }
    cabecalho->proxRRN = count;
    cabecalho->proxByteOffset = ftell(saida);
    cabecalho->status = ARQUIVO_CONSISTENTE;
    atualizarCabecalho(saida, cabecalho, tipo);

    fclose(entrada);
    fclose(saida);

    binarioNaTela(arquivoSaida);

    free(cabecalho);
    liberaStrings(tipoArquivo, arquivoEntrada, arquivoSaida);
}

/**
 * @brief Equivalente ao comando SELECT * FROM, percorre um arquivo binário e
 * exibe na tela todos os registros contidos nele.
 */
void selectSemWhere()
{
    char *tipoArquivo = readUntil(stdin, ' ');
    char *arquivoEntrada = readUntil(stdin, ' ');

    FILE *fptr = fopen(arquivoEntrada, "rb");
    cabecalho_t *cab = NULL;

    int tArquivo;
    long int final;

    if (!strcmp(tipoArquivo, "tipo1"))
    {
        tArquivo = 1;
        cab = lerCabecalho(1, fptr);
        final = TAM_CABECALHO1 + cab->proxRRN * TAM_REG;
    }
    else if (!strcmp(tipoArquivo, "tipo2"))
    {
        tArquivo == 2;
        cab = lerCabecalho(2, fptr);
        final = cab->proxByteOffset;
    }
    else
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(fptr);
        free(cab);
        liberaStrings(tipoArquivo, arquivoEntrada, NULL);
        return;
    }

    if (cab->status == '0')
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(fptr);
        free(cab);
        liberaStrings(tipoArquivo, arquivoEntrada, NULL);
        return;
    }

    if ((tArquivo == 2 && !cab->proxByteOffset) ||
        (tArquivo == 1 && !cab->proxRRN))
    {
        printf("Registro inexistente.\n");
        fclose(fptr);
        free(cab);
        liberaStrings(tipoArquivo, arquivoEntrada, NULL);
        return;
    }

    do
    {
        long int posInicio = ftell(fptr);

        registro_t *reg = lerRegistro(tArquivo, fptr, final);

        if (reg->removido == '0')
        {
            imprimirRegistro(reg, cab);
        }

        liberaRegistro(reg);
    } while (ftell(fptr) < final);

    fclose(fptr);
    free(cab);
    liberaStrings(tipoArquivo, arquivoEntrada, NULL);
}

/**
 * @brief Equivalente ao comando SELECT * FROM ... WHERE, percorre um arquivo binário e
 * exibe na tela todos os registros contidos nele.
 */
void selectCWhere()
{
    char *tipoArquivo = readUntil(stdin, ' ');
    char *arquivoEntrada = readUntil(stdin, ' ');
    int numFiltros = readNumberUntil(stdin, '\n');
    buscaParams_t *busca = malloc(sizeof(buscaParams_t));

    FILE *fptr = fopen(arquivoEntrada, "rb");
    cabecalho_t *cab = NULL;

    int tArquivo;
    long int final;

    if (!strcmp(tipoArquivo, "tipo1"))
    {
        tArquivo = 1;
        cab = lerCabecalho(1, fptr);
        final = TAM_CABECALHO1 + cab->proxRRN * TAM_REG;
    }
    else if (!strcmp(tipoArquivo, "tipo2"))
    {
        tArquivo == 2;
        cab = lerCabecalho(2, fptr);
        final = cab->proxByteOffset;
    }
    else
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(fptr);
        free(cab);
        liberaStrings(tipoArquivo, arquivoEntrada, NULL);
        return;
    }

    if (cab->status == '0')
    {
        printf("Falha no processamento do arquivo.\n");
        fclose(fptr);
        free(cab);
        liberaStrings(tipoArquivo, arquivoEntrada, NULL);
        return;
    }

    if ((tArquivo == 2 && !cab->proxByteOffset) ||
        (tArquivo == 1 && !cab->proxRRN))
    {
        printf("Registro inexistente.\n");
        fclose(fptr);
        free(cab);
        liberaStrings(tipoArquivo, arquivoEntrada, NULL);
        return;
    }

    inicializaStructBusca(busca, numFiltros, stdin);

    percorreArquivoAplicandofiltro(fptr, busca, tArquivo, cab, final, &acaoFiltro_imprimirRegistro, NULL, NULL, NULL);

    fclose(fptr);
    free(cab);
    liberaStrings(tipoArquivo, arquivoEntrada, NULL);
}

/**
 * @brief Recupera um registro de tamanho FIXO de um arquivo
 * binário a partir de seu RRN
 */
void recuperarRegistro()
{
    char *tipoArquivo = readUntil(stdin, ' ');
    char *arquivoEntrada = readUntil(stdin, ' ');
    int RRN = readNumberUntil(stdin, '\n');

    int tipo = 0;
    if (strcmp(tipoArquivo, "tipo1") == TIPOS_IGUAIS)
        tipo = 1;
    else if (strcmp(tipoArquivo, "tipo2") == TIPOS_IGUAIS)
        tipo = 2;

    if (tipo == 2)
    {
        printf("A recuperação não pode ser realizada.\n");
        liberaStrings(tipoArquivo, arquivoEntrada, NULL);
    }
    else if (tipo == 1)
    {
        FILE *entrada = fopen(arquivoEntrada, "rb");
        if (entrada == NULL)
        {
            printf("Falha no processamento do arquivo.\n");
            fclose(entrada);
            liberaStrings(tipoArquivo, arquivoEntrada, NULL);
            return;
        }
        cabecalho_t *cabecalho = lerCabecalho(1, entrada);
        if (arquivoConsistente(cabecalho->status) == false)
        {
            fclose(entrada);
            free(cabecalho);
            liberaStrings(tipoArquivo, arquivoEntrada, NULL);
            return;
        }
        if (RRN < 0 || RRN >= cabecalho->proxRRN)
            printf("Registro inexistente.\n");
        else
        {
            fseek(entrada, (RRN * TAM_REG) + TAM_CABECALHO1, 0);
            registro_t *reg = lerRegistro(1, entrada, (cabecalho->proxRRN * TAM_REG) + TAM_CABECALHO1);
            if (reg->removido == REGISTRO_REMOVIDO)
                printf("Registro inexistente.\n");
            else
                imprimirRegistro(reg, cabecalho);
            liberaRegistro(reg);
        }
        fclose(entrada);
        free(cabecalho);
        liberaStrings(tipoArquivo, arquivoEntrada, NULL);
    }
}

/**
 * @brief Equivalente ao comando SQL, cria uma tabela de Índices (arquivo binario)
 * a partir de um arquivo de dados
 */
void createIndex()
{
    // leitura da entrada
    char *tipoArquivo = readUntil(stdin, ' ');
    char *arquivoDados = readUntil(stdin, ' ');
    char *arquivoIndice = readUntil(stdin, '\n');

    // definindo tipo do arquivo
    int tipo = 0;
    if (strcmp(tipoArquivo, "tipo1") == TIPOS_IGUAIS)
        tipo = 1;
    else if (strcmp(tipoArquivo, "tipo2") == TIPOS_IGUAIS)
        tipo = 2;

    // abrindo e conferindo arquivos
    FILE *dados = fopen(arquivoDados, "rb");
    FILE *index_fptr = fopen(arquivoIndice, "wb");

    if (confereArquivos(dados, index_fptr) == false)
    {
        liberaStrings(tipoArquivo, arquivoDados, arquivoIndice);
        return;
    }

    cabecalho_t *cabecalho = NULL;
    if (tipo == 1)
        cabecalho = lerCabecalho(1, dados);

    else if (tipo == 2)
        cabecalho = lerCabecalho(2, dados);

    if (arquivoConsistente(cabecalho->status) == false)
    {
        fclose(dados);
        free(cabecalho);
        liberaStrings(tipoArquivo, arquivoDados, arquivoIndice);
        return;
    }
    long int final;

    if (tipo == 1)
        final = ((cabecalho->proxRRN * TAM_REG) + TAM_CABECALHO1);

    else if (tipo == 2)
        final = cabecalho->proxByteOffset;

    free(cabecalho);

    cabecalhoIndex_t *cabecalhoIndice = malloc(sizeof(cabecalhoIndex_t));
    cabecalhoIndice->status = ARQUIVO_INCONSISTENTE;
    escreverCabecalhoIndex(index_fptr, cabecalhoIndice);

    indice_t *indice = inicializarIndice();

    char buffer;
    int count = 0;
    while (fread(&buffer, 1, 1, dados) != 0)
    {
        ungetc(buffer, dados);

        registro_t *registro = NULL;
        if (tipo == 1)
            registro = lerRegistro(1, dados, final);

        else if (tipo == 2)
            registro = lerRegistro(2, dados, final);

        if (registro->removido == REGISTRO_REMOVIDO)
        {
            liberaRegistro(registro);
            continue;
        }

        registroIndice_t *registroIndice = malloc(sizeof(registroIndice_t));

        if (tipo == 1)
            registroIndice->RRN = ((ftell(dados) - TAM_CABECALHO1) / TAM_REG) - 1;

        else if (tipo == 2)
            registroIndice->byteOffset = ftell(dados) - registro->tamRegistro - 5;

        registroIndice->id = registro->id;

        // adicionar registro do tipo índice ao vetor que contitui o índice
        indice->registros = realloc(indice->registros, (count + 1) * sizeof(registroIndice_t));
        indice->registros[count] = (*registroIndice);

        count++;

        liberaRegistro(registro);
        free(registroIndice);
    }

    indice->tamanho = count;
    cabecalhoIndice->status = ARQUIVO_CONSISTENTE;

    qsort(indice->registros, indice->tamanho, sizeof(registroIndice_t), compara);

    escreverCabecalhoIndex(index_fptr, cabecalhoIndice);
    escreverIndice(index_fptr, indice, tipo);

    fclose(dados);
    fclose(index_fptr);

    binarioNaTela(arquivoIndice);

    liberarIndice(indice);
    free(cabecalhoIndice);
    liberaStrings(tipoArquivo, arquivoDados, arquivoIndice);
}

/**
 * @brief Equivalente ao comando SQL, remove um registro do arquivo de dados,
 * de forma lógica
 */
void delete ()
{
    // leitura da entrada
    char *tipoArquivo = readUntil(stdin, ' ');
    char *arquivoDados = readUntil(stdin, ' ');
    char *arquivoIndice = readUntil(stdin, ' ');
    int numRemocoes = readNumberUntil(stdin, '\n');

    // definindo tipo do arquivo
    int tipo = 0;
    if (strcmp(tipoArquivo, "tipo1") == TIPOS_IGUAIS)
        tipo = 1;
    else if (strcmp(tipoArquivo, "tipo2") == TIPOS_IGUAIS)
        tipo = 2;

    // abrir e conferir os arquivos
    FILE *dados = fopen(arquivoDados, "rb+");
    FILE *index_fptr = fopen(arquivoIndice, "rb+");

    if (confereArquivos(dados, index_fptr) == false)
    {
        liberaStrings(tipoArquivo, arquivoDados, arquivoIndice);
        return;
    }

    cabecalho_t *cabecalho = NULL;
    if (tipo == 1)
        cabecalho = lerCabecalho(1, dados);

    else if (tipo == 2)
        cabecalho = lerCabecalho(2, dados);

    if (arquivoConsistente(cabecalho->status) == false)
    {
        fclose(dados);
        free(cabecalho);
        liberaStrings(tipoArquivo, arquivoDados, arquivoIndice);
        return;
    }

    cabecalhoIndex_t *cabecalhoIndex = lerCabecalhoIndex(index_fptr);

    if (arquivoConsistente(cabecalhoIndex->status) == false)
    {
        fclose(dados);
        fclose(index_fptr);
        free(cabecalhoIndex);
        free(cabecalho);
        liberaStrings(tipoArquivo, arquivoDados, arquivoIndice);
        return;
    }

    cabecalho->status = ARQUIVO_INCONSISTENTE;
    cabecalhoIndex->status = ARQUIVO_INCONSISTENTE;
    atualizarCabecalho(dados, cabecalho, tipo);
    escreverCabecalhoIndex(index_fptr, cabecalhoIndex);

    indice_t *indice = indiceParaRAM(index_fptr, tipo);
    idsRemovidos_t *removidos = NULL;

    for (int i = 0; i < numRemocoes; i++)
    {
        removidos = removerRegistro(dados, index_fptr, cabecalho, tipo);
        indice = removerRegistroIndex(indice, removidos->ids, tipo, removidos->tamanho);
    }

    index_fptr = freopen(NULL, "wb", index_fptr);
    cabecalhoIndex->status = ARQUIVO_CONSISTENTE;
    escreverCabecalhoIndex(index_fptr, cabecalhoIndex);
    escreverIndice(index_fptr, indice, tipo);

    fclose(index_fptr);

    cabecalho->status = ARQUIVO_CONSISTENTE;
    atualizarCabecalho(dados, cabecalho, tipo);

    fclose(dados);

    binarioNaTela(arquivoDados);
    binarioNaTela(arquivoIndice);

    free(cabecalho);
    free(cabecalhoIndex);
    liberarIndice(indice);
    liberaStrings(tipoArquivo, arquivoDados, arquivoIndice);
}

/**
 * @brief Equivalente ao insertInto do SQL, insere um registro
 * no arquivo
 */
void insertInto()
{
    // armazena informações do arquivo de dados
    arquivo_t *arq = inicializaStrcArquivo();

    // armazena informações do arquivo de indice
    arq->nomeArqIndes = readUntil(stdin, ' ');
    int numInserts = readNumberUntil(stdin, '\n');
    arq->indexfptr = fopen(arq->nomeArqIndes, "r+b");

    if (!arq->indexfptr)
    {
        printf("Falha ao abrir o arquivo\n");
        return;
    }

    // Vai até o inicio do arquivo e marca seu status como inconsistente
    fseek(arq->fptr, 0, SEEK_SET);
    fwrite("0", 1, 1, arq->fptr);

    // le arquivo de indices e armazena na RAM
    cabecalhoIndex_t *cabIndex = lerCabecalhoIndex(arq->indexfptr);
    cabIndex->status = ARQUIVO_INCONSISTENTE;
    escreverCabecalhoIndex(arq->indexfptr, cabIndex);
    arq->indice = indiceParaRAM(arq->indexfptr, arq->tipo);

    for (int i = 0; i < numInserts; i++)
    {
        arq->reg = lerRegistroStdin(arq->tipo);
        insereRegistroNoArquivo(arq, arq->indice);
        liberaRegistro(arq->reg);
    }

    // escreve dados no arquivo de indices
    escreverCabecalhoIndex(arq->indexfptr, cabIndex);
    atualizarCabecalho(arq->fptr, arq->cab, arq->tipo);

    arq->indexfptr = freopen(arq->nomeArqIndes, "wb", arq->indexfptr);
    escreverIndice(arq->indexfptr, arq->indice, arq->tipo);
    cabIndex->status = ARQUIVO_CONSISTENTE;
    escreverCabecalhoIndex(arq->indexfptr, cabIndex);

    // Volta ao começo do arquivo e marca o status como consistente
    fseek(arq->fptr, 0, SEEK_SET);
    fwrite("1", 1, 1, arq->fptr);

    // Libera memória da struct e printa dados na tela
    fclose(arq->fptr);
    fclose(arq->indexfptr);

    binarioNaTela(arq->nomeArqDados);
    binarioNaTela(arq->nomeArqIndes);

    liberarStrctArq(arq);
}

/**
 * @brief Equivalente ao update do SQL, atualiza um registro
 * no arquivo
 */
void update()
{

    // inicializa arquivo de dados e de indice
    arquivo_t *arq = inicializaStrcArquivo();

    arq->nomeArqIndes = readUntil(stdin, ' ');
    int numUpdates = readNumberUntil(stdin, '\n');
    arq->indexfptr = fopen(arq->nomeArqIndes, "r+b");

    // Vai até o inicio do arquivo e marca seu status como inconsistente
    fseek(arq->fptr, 0, SEEK_SET);
    fwrite("0", 1, 1, arq->fptr);

    cabecalhoIndex_t *cabIndex = lerCabecalhoIndex(arq->indexfptr);
    cabIndex->status = ARQUIVO_INCONSISTENTE;
    escreverCabecalhoIndex(arq->indexfptr, cabIndex);
    arq->indice = indiceParaRAM(arq->indexfptr, arq->tipo);

    // para cada busca, cria dois arquivos de filtro
    //  um para armazenar campos que serão filtrados
    //  e o segundo para armazenar os novos valores
    for (int i = 0; i < numUpdates; i++)
    {
        buscaParams_t *busca = lerFiltrosUpdate();
        buscaParams_t *parametrosUpdate = lerFiltrosUpdate();

        updateRegistros(arq, busca, parametrosUpdate);
        liberaStructBusca(busca);
        liberaStructBusca(parametrosUpdate);
    }

    cabIndex->status = ARQUIVO_CONSISTENTE;
    escreverCabecalhoIndex(arq->indexfptr, cabIndex);

    escreverIndice(arq->indexfptr, arq->indice, arq->tipo);

    // Volta ao começo do arquivo e marca o status como consistente
    fseek(arq->fptr, 0, SEEK_SET);
    fwrite("1", 1, 1, arq->fptr);

    fclose(arq->fptr);
    fclose(arq->indexfptr);

    binarioNaTela(arq->nomeArqDados);
    binarioNaTela(arq->nomeArqIndes);

    liberarStrctArq(arq);
}

void selectWhereIndex()
{
    arquivo_t *arq = inicializaStrcArquivo();
    arq->nomeArqIndes = readUntil(stdin, ' ');
    readUntil(stdin, ' '); // Descartando o nome id
    int valId = readNumberUntil(stdin, '\n');

    // referenciando arquivo de indice
    arq->indexfptr = fopen(arq->nomeArqIndes, "rb");
    if (!arq->indexfptr)
    {
        printf("Falha no processamento do arquivo");
        return;
    }

    inicializaCabBtree(arq);

    int indice = buscaIndiceBtree(arq, valId);

    if (indice == -1)
    {
        printf("Registro inexistente.");
    }
    else
    {

        int pos = arq->tipo == 1 ? TAM_CABECALHO1 + (TAM_REG * indice) : indice;

        fseek(arq->fptr, pos, SEEK_SET);

        registro_t *reg = lerRegistro(arq->tipo, arq->fptr, arq->final);

        if (reg->removido == '0')
        {
            imprimirRegistro(reg, arq->cab);
        }
        else
        {
            printf("Registro inexistente.");
        }

        liberaRegistro(reg);
        
    }

    // função de busca por posicao no arquivo

    // printar resultado

    fclose(arq->fptr);
    fclose(arq->indexfptr);
}