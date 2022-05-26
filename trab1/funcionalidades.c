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

void selectSemWhere() {
    char* tipoArquivo = readUntil(stdin, ' ');
    char* arquivoEntrada = readUntil(stdin, ' ');

    FILE* fptr = fopen(arquivoEntrada, "rb");
    cabecalho_t* cab = NULL;

    int tArquivo;
    long int final;

    if(!strcmp(tipoArquivo, "tipo1")){
        tArquivo = 1;
        cab = lerCabecalhoTipo1(fptr);
        final = TAM_CABECALHO1 + cab->proxRRN * TAM_REG;
    }else if(!strcmp(tipoArquivo, "tipo2")){
        tArquivo == 2;
        cab = lerCabecalhoTipo2(fptr);
        final = cab->proxByteOffset;
    }else{
        printf("Falha no processamento do arquivo.\n");
        free(arquivoEntrada);
        free(tipoArquivo);
        fclose(fptr);
        return;
    }

    if(cab->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        free(arquivoEntrada);
        free(tipoArquivo);
        fclose(fptr);
        free(cab);
        return;
    }

    if((tArquivo == 2 && !cab->proxByteOffset)||
       (tArquivo == 1 && !cab->proxRRN)){
        printf("Registro inexistente.\n");
        free(arquivoEntrada);
        free(tipoArquivo);
        fclose(fptr);
        free(cab);
        return;
    }

    do{
        long int posInicio = ftell(fptr);
        
        registro_t *reg = lerRegistro(tArquivo,fptr);

        if(reg->removido == '0'){
            printaRegistro(reg);
        }

        if(tArquivo == 1){
            fseek(fptr, posInicio + TAM_REG, SEEK_SET);
        }else{
            fseek(fptr, posInicio + reg->tamRegistro + 5, SEEK_SET);
        }

        liberaRegistro(reg);
    }while(ftell(fptr) < final);

    free(cab);
    free(arquivoEntrada);
    free(tipoArquivo);
    fclose(fptr);
}

void selectCWhere(){
    char* tipoArquivo = readUntil(stdin, ' ');
    char* arquivoEntrada = readUntil(stdin, ' ');
    int n = readNumberUntil(stdin, '\n');
    buscaParams_t *busca = malloc(sizeof(buscaParams_t));

    FILE* fptr = fopen(arquivoEntrada, "rb");
    cabecalho_t* cab = NULL;

    int tArquivo;
    long int final;

    if(!strcmp(tipoArquivo, "tipo1")){
        tArquivo = 1;
        cab = lerCabecalhoTipo1(fptr);
        final = TAM_CABECALHO1 + cab->proxRRN * TAM_REG;
    }else if(!strcmp(tipoArquivo, "tipo2")){
        tArquivo == 2;
        cab = lerCabecalhoTipo2(fptr);
        final = cab->proxByteOffset;
    }else{
        printf("Falha no processamento do arquivo.\n");
        free(arquivoEntrada);
        free(tipoArquivo);
        fclose(fptr);
        return;
    }

    if(cab->status == '0'){
        printf("Falha no processamento do arquivo.\n");
        free(arquivoEntrada);
        free(tipoArquivo);
        fclose(fptr);
        free(cab);
        return;
    }

    if((tArquivo == 2 && !cab->proxByteOffset)||
       (tArquivo == 1 && !cab->proxRRN)){
        printf("Registro inexistente.\n");
        free(arquivoEntrada);
        free(tipoArquivo);
        fclose(fptr);
        free(cab);
        return;
    }

    inicializaStructBusca(busca);

    for(int i =0;i < n; i++){
        char *nomeCampo = readUntil(stdin, ' ');
        for(int j =0; j < NUM_PARAMETROS; j++){
            if(!strcmp(busca->filtros[j],nomeCampo)){
                
                // free(busca->filtros[j]);
                
                busca->ehBuscado[j] = 1;

                if(j < 3){
                    busca->filtros[j] = readUntil(stdin,'\n');
                }else{
                    getc(stdin);
                    busca->filtros[j] = readUntil(stdin,'"');; 
                    getc(stdin);
                }
            }
        }
    }

    do{
        long int posInicio = ftell(fptr);
        
        registro_t *reg = lerRegistro(tArquivo,fptr);

        if(reg->removido == '0'){
            if(ehValidoFiltro(reg,busca)){
                printaRegistro(reg);
            }
        }

        if(tArquivo == 1){
            fseek(fptr, posInicio + TAM_REG, SEEK_SET);
        }else{
            fseek(fptr, posInicio + reg->tamRegistro + 5, SEEK_SET);
        }

        liberaRegistro(reg);
    }while(ftell(fptr) < final);

    free(cab);
    free(arquivoEntrada);
    free(tipoArquivo);
    fclose(fptr);
    liberaStructBusca(busca);
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
            registro_t* reg = lerRegistro(1,entrada);
            imprimirRegistro(reg, cabecalho);
            liberar(reg);
        }
        fclose(entrada);
        free(cabecalho);
        free(tipoArquivo);
        free(arquivoEntrada);
    }
}