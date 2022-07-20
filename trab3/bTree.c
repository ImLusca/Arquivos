#include "bTree.h"
#include "funcionalidades.h"
#include <stdlib.h>
// trazer Nos Para memoria

void inicializaCabBtree(arquivo_t *arq)
{

  cabBtree_t *cab = malloc(sizeof(cabBtree_t));

  fseek(arq->indexfptr, 0, SEEK_SET);

  fread(&cab->status, 1, 1, arq->indexfptr);
  fread(&cab->noRaiz, 4, 1, arq->indexfptr);
  fread(&cab->proxRRN, 4, 1, arq->indexfptr);
  fread(&cab->nroNos, 4, 1, arq->indexfptr);

  if (arq->tipo == 1)
  {
    fseek(arq->indexfptr, TAM_CABECALHO_BTREE1, SEEK_SET);
  }
  else
  {
    fseek(arq->indexfptr, TAM_CABECALHO_BTREE2, SEEK_SET);
  }

  arq->cabBtree = cab;
}

noBtree_t *CarregaNoRam(arquivo_t *arq, int RRNNo)
{

  // Posicionar no inicio do Nó
  long int posinicial = ftell(arq->indexfptr);
  long int pos = arq->tipo == 1 ? TAM_NO_BTREE1 : TAM_NO_BTREE2;
  pos *= RRNNo + 1;
  
  fseek(arq->indexfptr, pos, SEEK_SET);

  noBtree_t *noBtree = malloc(sizeof(noBtree_t));

  // Ler nó
  fread(&noBtree->tipoNo, 1, 1, arq->indexfptr);
  fread(&noBtree->NroChaves, 4, 1, arq->indexfptr);

  // ler chaves
  cvNoBtree_t *chaves = malloc(sizeof(cvNoBtree_t) * 3);
  int *refSubArvores = malloc(sizeof(int) * 4);

  for (int i = 0; i < 3; i++)
  {
    fread(&chaves[i].chave, 4, 1, arq->indexfptr);
    if (arq->tipo == 1)
    {
      fread(&chaves[i].refRRN, 4, 1, arq->indexfptr);
    }
    else
    {
      fread(&chaves[i].refByteOffset, 8, 1, arq->indexfptr);
    }
  }

  for (int i = 0; i < 4; i++)
  {
    fread(&refSubArvores[i], 4, 1, arq->indexfptr);
  }

  noBtree->chaves = chaves;
  noBtree->refSubArvores = refSubArvores;

  // voltar para posicao inicial
  fseek(arq->indexfptr, posinicial, SEEK_SET);

  // retornar no
  return noBtree;
}

static void liberaNoBtree(noBtree_t *noBtree)
{
  free(noBtree->chaves);
  free(noBtree->refSubArvores);
  free(noBtree);
}

static long int buscaRecusivaBtree(arquivo_t *arq, int id, int RRN)
{

  if (RRN == -1)
  {
    return -1;
  }

  noBtree_t *noBtree = CarregaNoRam(arq, RRN);

  //@TODO: Implementar busca binaria
  int i = 0;

  for (; i < noBtree->NroChaves; i++)
  {
    if (noBtree->chaves[i].chave == id)
    {
      if (arq->tipo == 1)
        return noBtree->chaves[i].refRRN;
      else
        return noBtree->chaves[i].refByteOffset;
    }

    if (noBtree->chaves[i].chave > id)
    {
      break;
    }
  }

  // Liberar nó btree

  int chaveProxNo = noBtree->refSubArvores[i];

  liberaNoBtree(noBtree);

  return buscaRecusivaBtree(arq, id, chaveProxNo);
}

long int buscaIndiceBtree(arquivo_t *arq, int id)
{
  return buscaRecusivaBtree(arq, id, arq->cabBtree->noRaiz);
}