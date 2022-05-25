/*******************************************************************************
 |                       USP - Universidade de São Paulo                       |
 |            ICMC - Instituto de Ciências Matemáticas e de Computação         |
 *******************************************************************************
 |                    Bacharelado em Ciências de Computação                    |
 |                                   2022/1                                    |
 |                                                                             |
 |              Autores: Maria Júlia Soares De Grandi (12542501)               |
 |                       Lucas Pereira Pacheco (12543930)                      |
 |                            Disciplina: SCC0215                              |
 *******************************************************************************
  > Primeiro Trabalho Prático
 ******************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include "utils.h"
#include "funcionalidades.h"

int main()
{
	int funcionalidade;
	scanf("%d ", &funcionalidade);
	switch(funcionalidade)
	{
	case 1:
		createTable();
		break;
	case 2:
		funcionalidade2();
		break;
	case 3:
		
		break;
	case 4:
		recuperarRegistro();
		break;	
	}

	return 0;
}