#include "funcionalidades.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE* createTable()
{
    char* tipoArquivo = readUntil(stdin, ' ');
    char* arquivoEntrada = readUntil(stdin, ' ');
    char* arquivoSaida = readUntil(stdin, '\n');

    FILE* entrada = fopen(arquivoEntrada, "r");
    FILE* saida = fopen(arquivoSaida, "wb");

    if (entrada == NULL || saida == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        free(tipoArquivo);
        free(arquivoEntrada);
        free(arquivoSaida);
        return NULL;
    }    

    cabecalho_t* cabecalho = malloc(sizeof(cabecalho_t));
    cabecalho->status = '0';
    escreverCabecalho(saida, cabecalho, tipoArquivo);
    if (strcmp(tipoArquivo, "tipo2") == 0)
        cabecalho->proxByteOffset = TAM_CABECALHO2;

    char buffer;
    int count = 0;
    fseek(entrada, CABECALHO_CSV, 0);
    while (fread(&buffer, 1, 1, entrada) != 0)
    {
        registro_t* registro = malloc(sizeof(registro_t));
        ungetc(buffer, entrada);
        lerRegistrocsv(entrada, registro, tipoArquivo);
        escreverNoArquivo(saida, registro, cabecalho, tipoArquivo);
        count++;

        liberar(registro);
    }
    cabecalho->proxRRN = count;
    cabecalho->proxByteOffset = ftell(saida);
    cabecalho->status = '1';
    atualizarCabecalho(saida, cabecalho, tipoArquivo);

    fclose(entrada);
    fclose(saida);

    binarioNaTela(arquivoSaida);

    free(cabecalho);
    free(tipoArquivo);
    free(arquivoEntrada);
    free(arquivoSaida);

    return saida;
}

void funcionalidade2() {
    char* tipoArquivo = readUntil(stdin, ' ');
    char* arquivoEntrada = readUntil(stdin, ' ');

    FILE* binario = fopen(arquivoEntrada, "rb");

    if (!strcmp(tipoArquivo, "tipo1")) {

        cabecalho_t* cab = lerCabecalhoTipo1(binario);
        registro_t* reg = lerRegistroTipo1(binario);

    }
    else if (!strcmp(tipoArquivo, "tipo2")) {

        cabecalho_t* cab = lerCabecalhoTipo2(binario);
        registro_t* reg = lerRegistroTipo2(binario);

    }
    else {
        printf("Tipo de arquivo inválido\n");
    }

}

void recuperarRegistro()
{
    char* tipoArquivo = readUntil(stdin, ' ');
    char* arquivoEntrada = readUntil(stdin, ' ');
    int RRN = readNumberUntil(stdin, '\n');

    if (strcmp(tipoArquivo, "tipo2") == 0)
    {
        printf("A recuperação não pode ser realizada.\n");
        free(tipoArquivo);
        free(arquivoEntrada);
    }
    else if (strcmp(tipoArquivo, "tipo1") == 0)
    {
        FILE* entrada = fopen(arquivoEntrada, "rb");
        if (entrada == NULL)
        {
            printf("Falha no processamento do arquivo.\n");
            free(tipoArquivo);
            free(arquivoEntrada);
            return;
        }
        cabecalho_t* cabecalho = lerCabecalhoTipo1(entrada);
        if (cabecalho->status == 0)
        {
            printf("Falha no processamento do arquivo.\n");
            fclose(entrada);
            free(cabecalho);
            free(tipoArquivo);
            free(arquivoEntrada);
            return;
        }
        if (RRN < 0 || RRN >= cabecalho->proxRRN) 
            printf("Registro inexistente.\n");
        else
        {
            fseek(entrada, (RRN * TAM_REG) + TAM_CABECALHO1, 0);
            registro_t* reg = lerRegistroTipo1(entrada);
            imprimirRegistro(reg, cabecalho);
            liberar(reg);
        }
        fclose(entrada);
        free(cabecalho);
        free(tipoArquivo);
        free(arquivoEntrada);
    }
}