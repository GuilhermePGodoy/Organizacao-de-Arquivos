/*
Trabalho prático introdutório da disciplina  SCC0215 - Organização de Arquivos.
Desenvolvido por:
    Guilherme Pascoale Godoy - nUSP: 14576277
    Guilherme Antonio Costa Bandeira - nUSP: 14575620
*/

#include "funcoes_fornecidas.h"
#include "registros.h"
#include "funcionalidades.h"


struct cabecalho{
    char status[1];
    long int topo;
    long int proxByteOffset;
    int nroRegArq;
    int nroRegRem;
};


void funcao1(char* texto, char* bin)
{
    struct cabecalho cab;
    REGISTRO reg;
    char controle = 'a'; //Variável auxiliar para monitorar o fim do arquivo.

    FILE *incsv;
    incsv = fopen(texto, "rb");

    if(incsv == NULL){
        printf("Falha no processamento do arquivo de entrada.");
        return;
    }

    FILE *outbin;
    outbin = fopen(bin, "wb");
    
    if(outbin == NULL){
        printf("Falha no processamento do arquivo de saida.");
        return;
    }

    //Inicialização de campos do registro de cabeçalho.
    (cab.status)[0] = '0';
    cab.topo = -1;
    cab.proxByteOffset = 25;
    cab.nroRegArq = 0;
    cab.nroRegRem = 0;

    fseek(outbin, 25, SEEK_CUR); //Reserva espaço para o registro de cabecalho.

    fseek(incsv, 45, SEEK_CUR); //Pula a primeira linha.


    while((controle = getc(incsv)) != EOF){ //Tenta ler o próximo caractere com 'getc.'. Se não houver, 'getc' retorna  EOF, indicando que o arquivo acabou.
        fseek(incsv, -1, SEEK_CUR); //Se o arquivo não acabou, volta à posição lida por 'getc()'.

        cab.nroRegArq++;

        reg = le_registro_csv(incsv);

        //Próximo byte offset livre é logo após o último registro gravado.
        cab.proxByteOffset += reg.tamanhoRegistro;


        escreve_registro(outbin, reg);
    }


    rewind(outbin);

    (cab.status)[0] = '1';
    fwrite(cab.status, 1, 1, outbin);
    fwrite(&(cab.topo), 8, 1, outbin);
    fwrite(&(cab.proxByteOffset), 8, 1, outbin);
    fwrite(&(cab.nroRegArq), 4, 1, outbin);
    fwrite(&(cab.nroRegRem), 4, 1, outbin);

    fclose(incsv);
    fclose(outbin);
}

void funcao2(char *filename){
    FILE *f = fopen(filename, "rb");
    if(f == NULL){
        printf("Falha no processamento do arquivo.");
        return;
    }
    
    int nroRegArq;
    fseek(f, 17, SEEK_CUR); //Pula para o campo 'nroRegArq' do registro de cabeçalho.
    fread(&nroRegArq, 4, 1, f);
    if(nroRegArq == 0){
        printf("Registro inexistente.\n\n");
        return;
    }

    fseek(f, 4, SEEK_CUR); //Pula o campo 'nroRegRem' do registro de cabeçalho.

    REGISTRO reg;
    char controle;

    while((controle = getc(f)) != EOF){
        fseek(f, -1, SEEK_CUR);

        reg = le_registro_bin(f);

        if((reg.removido)[0] != '1'){
            printf("Nome do Jogador: ");
            if(reg.tamNomeJogador == 0)
                printf("SEM DADO\n");
            else{
                printf("%s\n", reg.nomeJogador);
            }
           free(reg.nomeJogador);

            printf("Nacionalidade do Jogador: ");
            if(reg.tamNacionalidade == 0)
                printf("SEM DADO\n");
            else{
                printf("%s\n", reg.Nacionalidade);
            }
            free(reg.Nacionalidade);

            printf("Clube do Jogador: ");
            if(reg.tamNomeClube == 0)
                printf("SEM DADO\n\n");
            else{
                printf("%s\n\n", reg.nomeClube);
            }
            free(reg.nomeClube);
        }
    }
}

struct como_busca{
    int m;
    int busca_id;
    int id;

    int busca_idade;
    int idade;

    int busca_nome;
    char nome[50];
    
    int busca_nacionalidade;
    char nacionalidade[50];

    int busca_clube;
    char clube[50];

    int deucerto;
};

void funcao3(FILE *f, int n){
    struct como_busca buscas[n];
    char nomeCampo[20];
    REGISTRO reg;


    for(int i = 0; i < n; i++){
        buscas[i].deucerto = 0;

        scanf(" %d", &((buscas[i]).m));

        for(int j = 0; j < (buscas[i]).m; j++){

            scanf(" %s", nomeCampo);

            if(strcmp(nomeCampo, "id") == 0){
                (buscas[i]).busca_id = 1;

                scanf(" %d", &((buscas[i]).id));

            }


            if(strcmp(nomeCampo, "idade") == 0){
                (buscas[i]).busca_idade = 1;

                scanf(" %d", &((buscas[i]).idade));
            }


            if(strcmp(nomeCampo, "nomeJogador") == 0){
                (buscas[i]).busca_nome = 1;

                scan_quote_string((buscas[i]).nome);
            }


            if(strcmp(nomeCampo, "nacionalidade") == 0){
                (buscas[i]).busca_nacionalidade = 1;

                scan_quote_string((buscas[i]).nacionalidade);
            }


            if(strcmp(nomeCampo, "nomeClube") == 0){
                (buscas[i]).busca_clube = 1;

                scan_quote_string((buscas[i]).clube);
            }
        }
    }

    char controle;
    int cont = 0;
    int flag = 0; //Flag utilizada para indicar que a busca deve parar. Utilizada para buscas por id, visto que este campo é único para cada jogador.

    for(int i = 0; i < n; i++){
        fseek(f, 25, SEEK_SET); //Pula o registro de cabeçalho.
        flag = 0;

        printf("Busca %d\n\n", i + 1);

        while((controle = getc(f)) != EOF){
            fseek(f, -1, SEEK_CUR);

            cont = 0;


            reg = le_registro_bin(f);


            if((buscas[i]).busca_id == 1){
                if(reg.id == (buscas[i]).id){
                    cont++;
                    flag = 1;
                    if((reg.removido)[0] == '1'){
                        free(reg.nomeJogador);
                        free(reg.Nacionalidade);
                        free(reg.nomeClube);

                        break;
                    }
                }
            }

            if((reg.removido)[0] == '1'){
                free(reg.nomeJogador);
                free(reg.Nacionalidade);
                free(reg.nomeClube);
                continue;
            }


            if((buscas[i]).busca_idade == 1){
                if(reg.idade == (buscas[i]).idade)
                    cont++;
            }


            if((buscas[i]).busca_nome == 1){
                if(strcmp(reg.nomeJogador, (buscas[i]).nome) == 0)
                    cont++;
            }


            if((buscas[i]).busca_nacionalidade == 1){
                if(strcmp(reg.Nacionalidade, (buscas[i]).nacionalidade) == 0)
                    cont++;
            }
            if((buscas[i]).busca_clube == 1){
                if(strcmp(reg.nomeClube, (buscas[i]).clube) == 0)
                    cont++;
            }


            if(cont == (buscas[i]).m){
                (buscas[i]).deucerto++;

                if(reg.tamNomeJogador != 0)
                    printf("Nome do Jogador: %s\n", reg.nomeJogador);
                else
                    printf("Nome do Jogador: SEM DADO\n");

                if(reg.tamNacionalidade != 0)
                    printf("Nacionalidade do Jogador: %s\n", reg.Nacionalidade);
                else
                    printf("Nacionalidade do Jogador: SEM DADO\n");

                if(reg.tamNomeClube != 0)
                    printf("Clube do Jogador: %s\n\n", reg.nomeClube);
                else
                    printf("Clube do Jogador: SEM DADO\n\n");
            }

            free(reg.nomeJogador);
            free(reg.Nacionalidade);
            free(reg.nomeClube);

            if(flag == 1)
                break;
        }

        if(buscas[i].deucerto == 0)
            printf("Registro inexistente.\n\n");
    }

    return;
}


int main(void){
    int numfuncao;
    char char1[100];
    char char2[100];

    scanf("%d", &numfuncao);

    if(numfuncao == 1){

        scanf(" %s", char1);
        scanf(" %s", char2);

        funcao1(char1, char2);


        binarioNaTela(char2);
    }
    else{
        if(numfuncao == 2){
            scanf(" %s", char1);

            funcao2(char1);
        }
        else{
            if(numfuncao == 3){
                int n;
                scanf(" %s", char1);
                scanf(" %d", &n);

                FILE *f = fopen(char1, "rb");
                if(f == NULL)
                    printf("Erro no processamento do arquivo.\n");
                else
                    funcao3(f, n);

                fclose(f);

            }


        }


    }







    return 0;
}