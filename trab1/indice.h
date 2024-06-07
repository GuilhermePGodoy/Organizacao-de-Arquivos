#ifndef INDICE
#define _INDICE_ INDICE

	//Função para ordenação dos registros por ordem crescente de id.
	void ordena_id(REG_INDICE vetor[], int menor, int maior);

	//Funcionalidades básicas sobre um arquivo de índice simples.
	REG_INDICE* carrega_indice(FILE *arq_indice, int nroRegistros); //Carregamento para memória principal.
	long int busca_id_indice(REG_INDICE *indice, int tamanho_indice, int id); //Busca por id.
	void insere_registros_indice(FILE *arq_indice, REG_INDICE *indice, int tamIndice, REG_INDICE *insercoes, int tamInsercoes); //Atualização do índice após inserções no arquivo de dados.
	void remove_registro_indice(REG_INDICE *indice, int tamanho, int id); //Remove do índice o registro que tem o id passado como parâmetro.
	void reescrita_indice(FILE *arq_indice, REG_INDICE *indice, int nroRegistros); //Atualização do índice após remoções no arquivo de dados.

#endif