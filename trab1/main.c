/*******************************************************************************
 |                       USP - Universidade de S�o Paulo                       |
 |            ICMC - Instituto de Ci�ncias Matem�ticas e de Computa��o         |
 *******************************************************************************
 |                    Bacharelado em Ci�ncias de Computa��o                    |
 |                                   2022/1                                    |
 |                                                                             |
 |              Autores: Maria J�lia Soares De Grandi (12542501)               |
 |                       Lucas Pereira Pacheco (12543930)                      |
 |                            Disciplina: SCC0215                              |
 *******************************************************************************
  > Primeiro Trabalho Pr�tico
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
		funcionalidade1();
		break;
	case 2:
		funcionalidade2();
		break;
	case 3:
		break;
	case 4:
		break;	
	}

	return 0;
}