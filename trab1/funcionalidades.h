#ifndef FUNCIONALIDADES
#define _FUNCIONALIDADES_ FUNCIONALIDADES

	//Funcionalidade 4: criação do arquivo de índice simples que indexa o arquivo de dados usando o campo 'id'.
	int funcao4(FILE *dados, FILE *indice);

	//Funcionalidade 5: permite a remoção de registros do arquivo de dados, utilizando a abordagem de reaproveitamento de espaço.
	int funcao5(FILE *dados, char *nome_arq_indice, int n);

	//Funcionalidade 6: permite a inserção de novos registros no arquivo de dados, utilizando a abordagem de reaproveitamento de espaço, com a estratégia Best Fit.
	int funcao6(FILE *dados, char *nome_arq_indice, int n);

#endif