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


//Struct para armazenar os campos de um registro do índice.
typedef struct reg_indice{
	int id;
	long int byteOffset;
}REG_INDICE;


	//Função que lê um registro de cabeçalho de um arquivo .bin e retorna uma struct CABECALHO com os campos.
	CABECALHO le_cabecalho(FILE *f);

	//Função que escreve o registro de cabeçalho no arquivo .bin.
	void escreve_cabecalho(CABECALHO cab, FILE *dados);

	//Função que lê um registro de um arquivo .csv e retorna uma struct REGISTRO com os campos.
	REGISTRO le_registro_csv(FILE *f);

	//Função que lê um registro de um arquivo .bin e retorna uma struct REGISTRO com os campos.
	REGISTRO le_registro_bin(FILE *f);

	//Função que escreve um registro passado como parâmetro em um arquivo .bin.
	void escreve_registro(FILE *f, REGISTRO reg);

	//Função que lê um registro do índice e retorna uma struct REG_INDICE com os campos.
	REG_INDICE le_registro_indice(FILE *f);

	//Função que recebe um registro e o escreve no índice.
	void escreve_registro_indice(FILE *f, REG_INDICE reg);


#endif