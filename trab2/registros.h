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
	} CABECALHO_DADOS;


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
	} REGISTRO_DADOS;

	//Função auxiliar que imprime os valores dos campos de um registro.
	void imprimeRegistro(REGISTRO_DADOS reg);

	//Função que lê um registro de cabeçalho de um arquivo .bin e retorna uma struct CABECALHO_DADOS com os campos.
	CABECALHO_DADOS le_cabecalho_dados(FILE *f);

	//Função que escreve o registro de cabeçalho no arquivo .bin.
	void escreve_cabecalho_dados(FILE *dados, CABECALHO_DADOS cab);

	//Função que lê um registro de um arquivo .csv e retorna uma struct REGISTRO_DADOS com os campos.
	REGISTRO_DADOS le_registro_csv(FILE *f);

	//Função que lê um registro_DADOS de um arquivo .bin e retorna uma struct REGISTRO_DADOS com os campos.
	REGISTRO_DADOS le_registro_dados(FILE *f);

	//Função que escreve um registro_DADOS passado como parâmetro em um arquivo .bin.
	void escreve_registro_dados(FILE *f, REGISTRO_DADOS reg);

	//Função que libera a memória alocada para os campos de tamanho variável de um registro.
	void libera_registro_dados(REGISTRO_DADOS reg);

#endif