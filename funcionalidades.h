#ifndef FUNCIONALIDADES
#define _FUNCIONALIDADES_ FUNCIONALIDADES

	//Função que lê os registros do arquivo .csv e os escreve no arquivo binário
	void funcao1(FILE *incsv, FILE *outbin);

	//Função que imprime os dados dos registros do arquivo binário.
	void funcao2(FILE *f);

	//Função que realiza buscas por campos no arquivo binário.
	void funcao3(FILE *f, int n);

#endif