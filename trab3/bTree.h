#ifndef BTREE
#define BTREE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "funcionalidades.h"
#include "fileUtils.h"
#include "indexUtils.h"

long int buscaIndiceBtree(arquivo_t *arq, int id);
void inicializaCabBtree(arquivo_t *arq);


#endif