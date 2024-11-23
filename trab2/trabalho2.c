/*
Trabalho prático 2 da disciplina  SCC0215 - Organização de Arquivos.
Desenvolvido por:
    Guilherme Pascoale Godoy - nUSP: 14576277
    Guilherme Antonio Costa Bandeira - nUSP: 14575620
*/

#include "funcoes_fornecidas.h"
#include "arvoreb.h"
#include "funcionalidades.h"
#include "registros.h"

int main(void){

	FILE *arq_dados, *arq_indice;
	int comando, n;
	char nomeArqDados[50], nomeArqIndice[50];

	scanf(" %d", &comando);

	switch(comando){
		case 7:
			scanf(" %s %s", nomeArqDados, nomeArqIndice);

			arq_dados = fopen(nomeArqDados, "rb");
			if(arq_dados == NULL){
				printf("Falha no processamento do arquivo.\n");
				return 0;
			}
			arq_indice = fopen(nomeArqIndice, "w+b");
			if(arq_indice == NULL){
				printf("Falha no processamento do arquivo.\n");
				fclose(arq_dados);
				return 0;
			}

			else{
				funcao7(arq_dados, arq_indice);

				fclose(arq_dados);
				fclose(arq_indice);

				binarioNaTela(nomeArqIndice);
			}
		break;

		case 8:
			scanf(" %s %s %d", nomeArqDados, nomeArqIndice, &n);

			arq_dados = fopen(nomeArqDados, "rb");
			if(arq_dados == NULL){
				printf("Falha no processamento do arquivo.\n");
				 return 0;
			}
			arq_indice = fopen(nomeArqIndice, "rb");
			if(arq_indice == NULL){
				printf("Falha no processamento do arquivo.\n");
				fclose(arq_dados);
				return 0;
			}
			if(!funcao8(arq_dados, arq_indice, n)){
				printf("Falha no processamento do arquivo.\n");
				fclose(arq_dados);
				fclose(arq_indice);

				return 0;
			}
			fclose(arq_dados);
			fclose(arq_indice);
		break;

		case 9:
			scanf(" %s %s %d", nomeArqDados, nomeArqIndice, &n);

			arq_dados = fopen(nomeArqDados, "rb");
			if(arq_dados == NULL){
				printf("Falha no processamento do arquivo.\n");
				 return 0;
			}
			arq_indice = fopen(nomeArqIndice, "rb");
			if(arq_indice == NULL){
				printf("Falha no processamento do arquivo.\n");
				fclose(arq_dados);
				return 0;
			}
			if(!funcao9(arq_dados, arq_indice, n)){
				printf("Falha no processamento do arquivo.\n");
				fclose(arq_dados);
				fclose(arq_indice);

				return 0;
			}
			fclose(arq_dados);
			fclose(arq_indice);
		break;

		case 10:
			scanf(" %s %s %d", nomeArqDados, nomeArqIndice, &n);

			arq_dados = fopen(nomeArqDados, "rb+");
			if(arq_dados == NULL){
				printf("Falha no processamento do arquivo.\n");
				 return 0;
			}
			arq_indice = fopen(nomeArqIndice, "rb+");
			if(arq_indice == NULL){
				printf("Falha no processamento do arquivo.\n");
				fclose(arq_dados);
				return 0;
			}

			if(!funcao10(arq_dados, arq_indice, n)){
				printf("Falha no processamento do arquivo.\n");
				fclose(arq_dados);
				fclose(arq_indice);

				return 0;
			}
			fclose(arq_dados);
			fclose(arq_indice);

			binarioNaTela(nomeArqDados);
			binarioNaTela(nomeArqIndice);
		break;
	}

	return 0;
}