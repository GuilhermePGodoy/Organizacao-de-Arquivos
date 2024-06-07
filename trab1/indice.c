#include "registros.h"


/*
Funções auxiliares para ordenação dos registros do índice por id.
Uso do algoritmo Quick Sort.
*/
void trocar(REG_INDICE *a, REG_INDICE *b ){
    REG_INDICE temp = *a;
    *a = *b;
    *b = temp;
}

//Função para particionar o vetor
int particionar(REG_INDICE vetor[], int menor, int maior){
    int pivot = (vetor[maior]).id;
    int i = (menor - 1);

    for (int j = menor; j <= maior - 1; j++) {
        if ((vetor[j]).id <= pivot) {
            i++;
            trocar(&vetor[i], &vetor[j]);
        }
    }
    trocar(&vetor[i + 1], &vetor[maior]);

    return (i + 1);
}


void ordena_id(REG_INDICE vetor[], int menor, int maior){
    if (menor < maior){
        int pi = particionar(vetor, menor, maior);

        ordena_id(vetor, menor, pi - 1);
        ordena_id(vetor, pi + 1, maior);
    }
}

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

//Busca por 'id' no índice. Retorna o byte offset do registro correspondente no arquivo de dados.
long int busca_id_indice(REG_INDICE *indice, int tamanho, int id){
    int posicaoReg = busca_binaria(indice, tamanho, id);

    if(posicaoReg == -1)
        return -1;
    else
        return (indice[posicaoReg]).byteOffset;
}


//Carregamento do índice para memória principal.
REG_INDICE* carrega_indice(FILE *arq_indice, int nroRegistros){
    REG_INDICE *indice = (REG_INDICE *) malloc(nroRegistros*sizeof(REG_INDICE)); //Vetor que armazenará os registros do índice.
    if(indice == NULL)
        return NULL;

    fseek(arq_indice, 1, SEEK_SET);

    for(int i = 0; i < nroRegistros; i++)
        indice[i] = le_registro_indice(arq_indice);

    return indice;
}


//Inserção de novos registros no índice.
void insere_registros_indice(FILE *arq_indice, REG_INDICE *indice, int tamIndice, REG_INDICE *insercoes, int tamInsercoes){
    ordena_id(insercoes, 0, tamInsercoes - 1);
    
    fseek(arq_indice, 1, SEEK_SET);

    int i = 0, j = 0;

    while(j < tamIndice){
        if(i < tamInsercoes && (insercoes[i]).id <= (indice[j]).id){
            escreve_registro_indice(arq_indice, insercoes[i]);
            i++;
        }
        else{
            escreve_registro_indice(arq_indice, indice[j]);
            j++;
        }
    }

    for(; i < tamInsercoes; i++)
        escreve_registro_indice(arq_indice, insercoes[i]);
}


//Remoção de registros do índice em memória primária.
void remove_registro_indice(REG_INDICE *indice, int tamanho_indice, int id){
	int posicao_registro = busca_binaria(indice, tamanho_indice, id);
	if(posicao_registro != -1)
    	for(int i = posicao_registro + 1; i < tamanho_indice; i++)
    		indice[i-1] = indice[i];
}


//Função que escreve os registros de 'indice' (que é o índice carregado em memória principal) no arquivo de índice.
void reescrita_indice(FILE *arq_indice, REG_INDICE *indice, int nroRegistros){

    fseek(arq_indice, 1, SEEK_SET);

    for(int i = 0; i < nroRegistros; i++)
        escreve_registro_indice(arq_indice, indice[i]);
}