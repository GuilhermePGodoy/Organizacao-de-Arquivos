#include "registros.h"

//Struct para armazenar os campos de um registro do índice.
typedef struct reg_indice{
	int id;
	long int proxByteOffset;
}REG_INDICE;

//Funcionalidade 4: criação do índice.
void funcao4(FILE *dados, FILE *indice){
	CABECALHO cab = le_cabecalho(dados); //Struct para armazenar os campos do registro de cabeçalho do arquivo de dados.
	REGISTRO reg; //Struct para armazenar os campos de um registro do arquivo de dados.

	if((cab.status)[0] == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

	REG_INDICE registros[cab.nroRegArq];

	long int byteOffset = 25;

	for(int i = 0; i < cab.nroRegArq; i++){
		reg = le_registro_bin(dados);
		
		(registros[i]).id = reg.id;
		(registros[i]).byteOffset = byteOffset;

		byteOffset += reg.tamanhoRegistro;
	}

	for(int i = 0; i < cab.nroRegArq; i++){
		printf("%d\n%d\n\n", ((registros[i]).id, (registros[i]).byteOffset);
	}

	//PROXIMO PASSO: ORDENAR O VETOR POR ORDEM CRESCENTE DE ID.
}


void funcao5(FILE *dados, FILE *indice){
	CABECALHO cab = le_cabecalho(dados);

	if((cab.status)[0] == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
}


void funcao6(FILE *dados, FILE *indice){
	CABECALHO cab = le_cabecalho(dados);

	if((cab.status)[0] == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
}