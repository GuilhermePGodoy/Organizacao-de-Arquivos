#include "registros.h"

//Função auxiliar para realizar a busca binária por um id no índice.
int busca_binaria(REG_INDICE *indice, int tamanho, int id) {
    int inicio = 0;
    int fim = tamanho - 1;
    int meio;

    while (inicio <= fim) {
        meio = (fim + inicio) / 2;

        // Verifica se o id está no meio
        if ((indice[meio]).id == id) {
            return meio;
        }
        
        // Se o id for maior, ignore a metade esquerda
        if ((indice[meio]).id < id) {
            inicio = meio + 1;
        }
        // Se o id for menor, ignore a metade direita
        else {
            fim = meio - 1;
        }
    }

    // Alvo não encontrado
    return -1;
}

REG_INDICE* carrega_indice(FILE *arq_indice, int nroRegistros){
    REG_INDICE *indice = (REG_INDICE *) malloc(nroRegistros*sizeof(REG_INDICE));
    if(indice == NULL)
        return NULL;

    fseek(arq_indice, 1, SEEK_SET);

    for(int i = 0; i < nroRegistros; i++)
        indice[i] = le_registro_indice(arq_indice);

    return indice;
}


long int busca_id_indice(REG_INDICE *indice, int tamanho, int id){
    int posicaoReg = busca_binaria(indice, tamanho, id);

    if(posicaoReg == -1)
        return -1;
    else
        return (indice[posicaoReg]).byteOffset;
}


void remove_registro_indice(REG_INDICE *indice, int tamanho_indice, int id){
	int posicao_registro = busca_binaria(indice, tamanho_indice, id);
	if(posicao_registro != -1)
    	for(int i = posicao_registro + 1; i < tamanho_indice; i++)
    		indice[i-1] = indice[i];
}