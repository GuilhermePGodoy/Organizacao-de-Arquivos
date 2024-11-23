#ifndef ARVORE_B
	#define ARVORE_B

	//Definição da ordem e tamanho das páginas da árvore B.
	#define ORDEM 4
	#define TAM_PAGINA 60

	#include <stdio.h>
	#include <stdlib.h>
	#include <string.h>

	//Struct para representar uma chave da árvore B.
	typedef struct chave_arv_b{
		int id;
		long int byteOffset;
	}CHAVE_ARV_B;

	//Struct para representar uma página (nó) da árvore B.
	typedef struct pagina{
		int alturaNo;
		int nroChaves;
		CHAVE_ARV_B chaves[ORDEM - 1];
		int ponteiros[ORDEM];
	}PAGINA;

	//Struct para representar o cabeçalho da árvore B.
	typedef struct cabecalho_arv_b{
		char status[1];
		int noRaiz;
		int proxRRN;
		int nroChaves;
	}CABECALHO_ARV_B;

	//Função que inverte o status do arquivo de índice.
	void modificaStatus_arvB(FILE *arvB);

	//Função para inserir uma chave, formada por id e byteOffset, na árvore B.
	void insere_arvoreB(FILE *arvB, int id, long int byteOffset);

	//Função que lê o cabeçalho  da árvore B e retorna seus campos em uma struct.
	CABECALHO_ARV_B le_cabecalho_arvB(FILE *arvB);
	
	//Função que lê a página localizada no RRN passado e retorna seus campos em uma struct.
	PAGINA le_pagina_arvB(FILE *arvB, int RRN);

	//Escreve cabeçalho no arquivo.
	void escreve_cabecalho_arvB(FILE *arvB, CABECALHO_ARV_B cab);

	//Escreve uma página no RRN passado.
	void escreve_pagina(FILE *arvB, PAGINA pag, int RRN);

	//Busca por id na árvore. Retorna seu byteOffset no arquivo de dados caso encontre, e -1 caso contrário.
	long int busca_id_arvB(FILE *arvB, int id);


	//FUNÇÕES AUXILIARES PARA IMPRESSÃO DA ÁRVORE B.
	void imprime_arvB(FILE *arvB);
	void imprimeCab(CABECALHO_ARV_B cab);
	void imprimePagina(PAGINA pag);

#endif