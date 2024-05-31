#include "registros.h"
#include "funcionalidades.h"

int main(void){
	FILE *dados, *indice;
	char nome_arq_dados[100], nome_arq_indice[100];
	int comando;

	scanf(" %d", &comando);

	switch(comando){
		case 4:
			scanf(" %s %s", nome_arq_dados, nome_arq_indice);

			dados = fopen(nome_arq_dados, "rb");
			indice = fopen(nome_arq_indice, "wb");
			
			if(dados == NULL || indice == NULL){
				printf("Falha no processamento do arquivo.\n");

				fclose(dados);
				fclose(indice);
			}
			else{
				if(funcao4(dados, indice) == 1){
					fclose(dados);
					fclose(indice);
					binarioNaTela(nome_arq_indice);
				}
				else{
					printf("Falha no processamento do arquivo.\n");
					
					fclose(dados);
					fclose(indice);
				}
			}

			break;

		case 5:
			int n;

			scanf(" %s %s", nome_arq_dados, nome_arq_indice);
			scanf(" %d", &n);

			dados = fopen(nome_arq_dados, "rb+");
			indice = fopen(nome_arq_indice, "rb+");

			if(dados == NULL || indice == NULL){
				printf("Falha no processamento do arquivo.\n");

				fclose(dados);
				fclose(indice);
			}
			else{
				if(funcao5(dados, indice, n) == 1){
					fclose(dados);
					fclose(indice);

					binarioNaTela(nome_arq_dados);
					binarioNaTela(nome_arq_indice);
				}
				else{
					printf("Falha no processamento do arquivo.\n");
					
					fclose(dados);
					fclose(indice);
				}
			}
			break;
		case 6:
			break;
	}

	return 0;
}