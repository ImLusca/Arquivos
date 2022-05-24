#include "funcionalidades.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

FILE* funcionalidade1()
{
	char* tipoArquivo = readUntil(stdin, ' ');
	char* arquivoEntrada = readUntil(stdin, ' ');
	char* arquivoSaida = readUntil(stdin, '\n');

	FILE* entrada = fopen(arquivoEntrada, "r");
    FILE* saida = fopen(arquivoSaida, "wb");
    if (entrada == NULL || saida == NULL)
    {
        printf("Falha no processamento do arquivo.\n");
        return NULL;
    }
    registro_t* registro = malloc(sizeof(registro_t));

    long counter = 0;
    char buffer;
    fseek(entrada, 61, 0);
    while (fread(&buffer, 1, 1, entrada) != 0) 
    {
        ungetc(buffer, entrada);
        counter++;
        lerArquivo(entrada, registro);
        //escreverArquivo(saida, registro, tipoArquivo);
    }

    fclose(entrada);
    fclose(saida);
    binarioNaTela(arquivoSaida);

    return saida;
}

void funcionalidade2(){
    char* tipoArquivo = readUntil(stdin, ' ');
    char* arquivoEntrada = readUntil(stdin, ' ');

    if(!strcmp(tipoArquivo, "tipo1")){
        registro_t *reg = lerRegistroTipo1(fopen(arquivoEntrada, "rb"));
                
    }else if(!strcmp(tipoArquivo,"tipo2")){
        

    }else{
        printf("Tipo de arquivo inválido\n");
    }

}
