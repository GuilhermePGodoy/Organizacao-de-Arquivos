#ifndef INDICE
#define _INDICE_ INDICE

	REG_INDICE* carrega_indice(FILE *arq_indice, int nroRegistros);
	long int busca_id_indice(REG_INDICE *indice, int tamanho_indice, int id);
	void remove_registro_indice(REG_INDICE *indice, int tamanho, int id); //Remove do índice o registro que tem o id passado como parâmetro.

#endif