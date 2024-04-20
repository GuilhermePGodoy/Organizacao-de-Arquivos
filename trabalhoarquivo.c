/*
Trabalho prático introdutório da disciplina  SCC0215 - Organização de Arquivos.
Desenvolvido por:
    Guilherme Pascoale Godoy - nUSP: 14576277
    Guilherme Antonio Costa Bandeira - nUSP: 14575620
*/

#include "funcoes_fornecidas.h"
#include "registros.h"


struct cabecalho{
    char status[1];
    long int topo;
    long int proxByteOffset;
    int nroRegArq;
    int nroRegRem;
};

//FUNÇÃO DE TESTE PARA IMPRIMIR STRUCT REGISTRO
void print_struct(struct registro r){
    printf("removido %s  tamreg %d  prox %ld  id %d  idade %d  tamnome %d  nome%s  tamnac %d  nac %s  tamclube %d  clube %s\n", r.removido, r.tamanhoRegistro, r.prox, r.id, r.idade, r.tamNomeJogador, r.nomeJogador, r.tamNacionalidade, r.Nacionalidade, r.tamNomeClube, r.nomeClube);
}


void funcao1(char* texto, char* bin)
{
    struct cabecalho cab;
    struct registro reg;
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

/*
        fread(reg.removido, 1, 1, f);

        if((reg.removido)[0] != '1'){ //Se o registro não estiver removido, será lido.
            fseek(f, 20, SEEK_CUR); //Pula os campos 'tamanhoRegistro', 'prox', 'id' e 'idade', que não serão exibidos.

            fread(&reg.tamNomeJogador, 4, 1, f);
            reg.nomeJogador = (char *) malloc(sizeof(char)*reg.tamNomeJogador + 1);
            fread(reg.nomeJogador, reg.tamNomeJogador, 1, f);
            reg.nomeJogador[reg.tamNomeJogador] = '\0';

            fread(&reg.tamNacionalidade, 4, 1, f);
            reg.Nacionalidade = (char *) malloc(sizeof(char)*reg.tamNacionalidade + 1);
            fread(reg.Nacionalidade, reg.tamNacionalidade, 1, f);
            reg.Nacionalidade[reg.tamNacionalidade] = '\0';

            fread(&reg.tamNomeClube, 4, 1, f);
            reg.nomeClube = (char *) malloc(sizeof(char)*reg.tamNomeClube + 1);
            fread(reg.nomeClube, reg.tamNomeClube, 1, f);
            reg.nomeClube[reg.tamNomeClube] = '\0';
*/

        reg = le_registro_bin(f);

        if((reg.removido)[0] != '1'){
            printf("Nome do Jogador: ");
            if(reg.tamNomeJogador == 0)
                printf("SEM DADO\n");
            else{
                printf("%s\n", reg.nomeJogador);
                free(reg.nomeJogador);
            }

            printf("Nacionalidade do Jogador: ");
            if(reg.tamNacionalidade == 0)
                printf("SEM DADO\n");
            else{
                printf("%s\n", reg.Nacionalidade);
                free(reg.Nacionalidade);
            }

            printf("Clube do Jogador: ");
            if(reg.tamNomeClube == 0)
                printf("SEM DADO\n\n");
            else{
                printf("%s\n\n", reg.nomeClube);
                free(reg.nomeClube);
            }
        }
    }
}

void funcao3(){
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


            }


        }


    }







    return 0;
}