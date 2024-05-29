#include "registros.h"
#include "funcionalidades.h"

int main(void){
	FILE *bin, *indice;
	char nomebin[100], nomeindice[100];
	int comando;

	scanf(" %d", &comando);

	switch(comando){
		case 4:
			scanf(" %s %s", nomebin, nomeindice);

			bin = fopen(nomebin, "rb");
			indice = fopen(nomeindice, "wb");
			
			if(bin == NULL || indice == NULL){
				printf("Falha no processamento do arquivo.\n");
			}
			else{
				if(funcao4(bin, indice) == 1){
					binarioNaTela(nomeindice);
				}
				else
					printf("Falha no processamento do arquivo.\n");

			}
			fclose(bin);
			fclose(indice);

			break;

		case 5:
			scanf(" %s %s", nomebin, nomeindice);
			bin = fopen(nomebin, "rb+");
			indice = fopen(nomeindice, "rb+");

			funcao5(bin, indice);

			break;

		case 6:
			break;
	}

	return 0;
}