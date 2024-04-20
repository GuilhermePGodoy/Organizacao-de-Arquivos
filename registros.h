#ifndef REGISTRO
#define _REGISTROS_ REGISTROS

	#include "funcoes_fornecidas.h"

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

	REGISTRO le_registro_csv(FILE *f);
	REGISTRO le_registro_bin(FILE *f);
	void escreve_registro(FILE *f, REGISTRO reg);


#endif