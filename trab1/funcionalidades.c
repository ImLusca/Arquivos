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
    registro_t* registro = malloc(sizeof(registro_t));

    if (strcmp(tipoArquivo, "tipo1") == 0)
        lerArquivoTipo1(entrada, registro);
    else if (strcmp(tipoArquivo, "tipo2") == 0)
        lerArquivoTipo2(entrada, registro);

    printf("id: %d\nano: %d\nqtt: %d\nsigla: %c%c", registro->id, registro->ano, registro->qtt, registro->sigla[0], registro->sigla[1]);

    fclose(entrada);
    FILE* saida = fopen(arquivoSaida, "wb");

    return saida;
}

void funcionalidade2(){
    char* tipoArquivo = readUntil(stdin, ' ');
    char* arquivoEntrada = readUntil(stdin, ' ');
    
    FILE *binario = fopen(arquivoEntrada, "rb");

    if(!strcmp(tipoArquivo, "tipo1")){        

        cabecalho_t *cab = lerCabecalhoTipo1(binario);
        registro_t *reg = lerRegistroTipo1(binario);
                
    }else if(!strcmp(tipoArquivo,"tipo2")){
        
        cabecalho_t *cab = lerCabecalhoTipo2(binario);
        registro_t *reg = lerRegistroTipo2(binario);

    }else{
        printf("Tipo de arquivo inv�lido\n");
    }

}
