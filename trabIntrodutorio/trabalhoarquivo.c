/*
Trabalho prático introdutório da disciplina  SCC0215 - Organização de Arquivos.
Desenvolvido por:
    Guilherme Pascoale Godoy - nUSP: 14576277
    Guilherme Antonio Costa Bandeira - nUSP: 14575620
*/

#include "funcoes_fornecidas.h"
#include "registros.h"
#include "funcionalidades.h"

int main(void){
    int numfuncao;
    char nomearq1[100]; 
    char nomearq2[100];

    scanf("%d", &numfuncao);

    if(numfuncao == 1){

        scanf(" %s", nomearq1);
        scanf(" %s", nomearq2);

        FILE *incsv, *outbin;

        incsv = fopen(nomearq1, "rb");

        outbin = fopen(nomearq2, "wb");

        if(incsv == NULL || outbin == NULL){
            printf("Falha no processamento do arquivo.\n");
            return 0;
        }

        funcao1(incsv, outbin);

        fclose(incsv);
        fclose(outbin);

        binarioNaTela(nomearq2);
    }
    else{
        if(numfuncao == 2){
            scanf(" %s", nomearq1);

            FILE *outbin;
            outbin = fopen(nomearq1,"rb");
            if(outbin == NULL){
            printf("Falha no processamento do arquivo.\n");
            return 0; 
            }

            funcao2(outbin);

            fclose(outbin);
        }
        else{
            if(numfuncao == 3){
                int n;
                scanf(" %s", nomearq1);
                scanf(" %d", &n);

                FILE *f = fopen(nomearq1, "rb");
                if(f == NULL)
                    printf("Falha no processamento do arquivo.\n");
                else
                    funcao3(f, n);

                fclose(f);

            }


        }


    }

    return 0;
}