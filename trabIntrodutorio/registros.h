#ifndef REGISTROS
#define _REGISTROS_ REGISTROS

	#include "funcoes_fornecidas.h"

  	//Struct para armazenamento e manipulação do registro de cabeçalho.
	typedef struct cabecalho{
	    char status[1];
	    long int topo;
	    long int proxByteOffset;
	    int nroRegArq;
	    int nroRegRem;
	} CABECALHO;

	

	//Struct para armazenamento e manipulação dos registros de dados.
	typedef struct registro{
	    char removido[1];
	    int tamanhoRegistro;
	    long int prox;
	    int id;
	    int idade;
	    int tamNomeJogador;
	    char* nomeJogador;
	    int tamNacionalidade;
	    char* Nacionalidade;
	    int tamNomeClube;
	    char* nomeClube;
	} REGISTRO;

	//Função que lê um registro de um arquivo .csv e retorna uma struct REGISTRO com os campos.
	REGISTRO le_registro_csv(FILE *f);

	//Função que lê um registro de um arquivo .bin e retorna uma struct REGISTRO com os campos.
	REGISTRO le_registro_bin(FILE *f);

	//Função que escreve um registro passado como parâmetro em um arquivo .bin.
	void escreve_registro(FILE *f, REGISTRO reg);


#endif