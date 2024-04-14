#include "funcoes_fornecidas.h"

struct cabecalho{
    char status[1];
    long int topo;
    long int proxByteOffset;
    int nroRegArq;
    int nroRegRem;
};

struct registro{
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
};

//FUNÇÃO DE TESTE PARA IMPRIMIR STRUCT REGISTRO
void print_struct(struct registro r){
    printf("removido %s  tamreg %d  prox %ld  id %d  idade %d  tamnome %d  nome%s  tamnac %d  nac %s  tamclube %d  clube %s\n", r.removido, r.tamanhoRegistro, r.prox, r.id, r.idade, r.tamNomeJogador, r.nomeJogador, r.tamNacionalidade, r.Nacionalidade, r.tamNomeClube, r.nomeClube);
}

int le_id(FILE **incsv){
    char id[10];
    char aux[0]; //variável auxiliar para a contagem do tamanho do id.
    int tam = 0; //tamanho, em bytes, do id do jogador.

    fread(aux, 1, 1, *incsv);
    while(aux[0] != ','){
        tam++;
        fread(aux, 1, 1, *incsv);
    }

    if(tam == 0)
        return -1;

    fseek(*incsv, (-1)*tam - 1, SEEK_CUR);
    fread(id, tam, 1, *incsv);

    return atoi(id);
}

int le_idade(FILE **incsv){
    char id[10];
    char aux[0]; //variável auxiliar para a contagem do tamanho da idade.
    int tam = 0; //tamanho, em bytes, da idade do jogador.

    fread(aux, 1, 1, *incsv);
    while(aux[0] != ','){
        tam++;
        fread(aux, 1, 1, *incsv);
    }
    
    if(tam == 0)
        return -1;

    fseek(*incsv, (-1)*tam - 1, SEEK_CUR);
    fread(id, tam, 1, *incsv);

    return atoi(id);
}

char* le_nomeJogador(FILE **incsv, int tamNome){
    char *nome = malloc(sizeof(char)*tamNome);
    if(nome == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    fread(nome, tamNome, 1, *incsv);

    printf("nome: %s\n", nome);
    printf("nome: %s\n", nome);


    return nome;
}

char* le_nacionalidade(FILE **incsv, int tamNacionalidade){
    char *nacionalidade = malloc(sizeof(char)*tamNacionalidade);
    if(nacionalidade == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    fread(nacionalidade, tamNacionalidade, 1, *incsv);

    return nacionalidade;
}

char* le_nomeClube(FILE **incsv, int tamNomeClube){
    char *nomeClube = malloc(sizeof(char)*tamNomeClube);
    if(nomeClube == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    fread(nomeClube, tamNomeClube, 1, *incsv);

    return nomeClube;
}

void funcao1(char* texto, char* bin)
{
    struct cabecalho cab;
    struct registro reg;

    FILE *incsv;
    incsv = fopen(texto, "rb");

    if(incsv == NULL){
        printf("Falha no processamento do arquivo de entrada.");
        exit(1);
    }

    FILE *outbin;
    outbin = fopen(bin, "wb");
    
    if(outbin == NULL){
        printf("Falha no processamento do arquivo de saida.");
        exit(1);
    }

    (cab.status)[0] = '0';
    cab.topo = -1;
    cab.proxByteOffset = 25;
    cab.nroRegArq = 0;
    cab.nroRegRem = 0;

    fseek(outbin, 25, SEEK_CUR); //Reserva espaço para o registro de cabecalho.

    fseek(incsv, 45, SEEK_CUR); //Pula a primeira linha.

    char aux[1]; //string auxiliar para contagem do tamanho dos campos de tamanho variável.
    char controle = 'a'; //Variável auxiliar para monitorar o fim do arquivo.
    int j = 0;
    while(controle != EOF){
        j++;

        cab.nroRegArq++;


        //Inicialização de campos que recebem um valor padrão.
        aux[0] = 0;
        reg.removido[0] = '0';
        reg.prox = -1;

        //Leitura do id.
        reg.id = le_id(&incsv);
        if(reg.id != -1)
            fseek(incsv, 1, SEEK_CUR);

        //Leitura da idade.
        reg.idade = le_idade(&incsv);
        if(reg.idade != -1)
            fseek(incsv, 1, SEEK_CUR);

        //Início da função 'le_nomeJogador'.
        reg.tamNomeJogador = 0;

        fread(aux, 1, 1, incsv);
        while(aux[0] != ','){
            reg.tamNomeJogador++;
            fread(aux, 1, 1, incsv);
        }
        if(reg.tamNomeJogador > 0){

            fseek(incsv, (-1)*reg.tamNomeJogador - 1, SEEK_CUR);

            reg.nomeJogador = (char *) malloc(sizeof(char)*reg.tamNomeJogador);
            if(reg.nomeJogador == NULL){
                printf("Falha no processamento do arquivo.\n");
                exit(1);
            }

            fread(reg.nomeJogador, reg.tamNomeJogador, 1, incsv);
        }
        //Fim da função 'le_nomeJogador'.
        if(reg.tamNomeJogador > 0) //Se tamNomeJogador == 0, o ponteiro já está na posição correta (na vírgula que marca o fim do campo).
            fseek(incsv, 1, SEEK_CUR); //Pula a vírgula após o nome do jogador.

        //Início da função 'le_Nacionalidade'.
        reg.tamNacionalidade = 0;

        fread(aux, 1, 1, incsv);
        int i = 0;
        while(aux[0] != ','){
            reg.tamNacionalidade++;
            fread(aux, 1, 1, incsv);
            i++;
        }

        if(reg.tamNacionalidade > 0){
            fseek(incsv, (-1)*reg.tamNacionalidade - 1, SEEK_CUR);

            reg.Nacionalidade = (char *) malloc(sizeof(char)*reg.tamNacionalidade);
            if(reg.Nacionalidade == NULL){
                printf("Falha no processamento do arquivo.\n");
                exit(1);
            }

            fread(reg.Nacionalidade, reg.tamNacionalidade, 1, incsv);
        }
        //Fim da função 'le_nomeJogador'.
        if(reg.tamNacionalidade > 0) //Se tamNacionalidade == 0, o ponteiro já está na posição correta (na vírgula que marca o fim do campo).
            fseek(incsv, 1, SEEK_CUR); //Pula a vírgula após a nacionalidade do jogador.

        //Início da função 'le_nomeClube'.
        reg.tamNomeClube = 0;

        fread(aux, 1, 1, incsv);
        while(aux[0] != '\n'){
            reg.tamNomeClube++;
            fread(aux, 1, 1, incsv);
        }

        if(reg.tamNomeClube > 0){
            fseek(incsv, (-1)*reg.tamNomeClube - 1, SEEK_CUR);

            reg.nomeClube = (char *) malloc(sizeof(char)*reg.tamNomeClube);
            if(reg.nomeClube == NULL){
                printf("Falha no processamento do arquivo.\n");
                exit(1);
            }

            fread(reg.nomeClube, reg.tamNomeClube, 1, incsv);
        }
        //Fim da função 'le_nomeClube'.
        if(reg.tamNomeClube > 0)//Se tamNomeClube == 0, o ponteiro já está na posição correta (no primeiro byte do próximo registro).
        fseek(incsv, 1, SEEK_CUR); //Pula a quebra de linha após o nome do clube do jogador.
        controle = getc(incsv); //Tenta ler o próximo caractere com 'getc.'. Se não houver, 'getc' retorna  EOF, indicando que o arquivo acabou.
        fseek(incsv, -1, SEEK_CUR);

        //Tamanho do registro = 33 bytes dos campos de tamanho fixo + tamanhos dos campos de tamanho variável.
        reg.tamanhoRegistro = 33 + reg.tamNomeJogador + reg.tamNacionalidade + reg.tamNomeClube;

        //Próximo byte offset livre é logo após o último registro gravado.
        cab.proxByteOffset += reg.tamanhoRegistro;

        //Função para testes.
        //print_struct(reg);

        //Escrita dos dados um por um no arquivo de saída.
        fwrite(reg.removido, 1, 1, outbin);
        fwrite(&(reg.tamanhoRegistro), 4, 1, outbin);
        fwrite(&(reg.prox), 8, 1, outbin);
        fwrite(&(reg.id), 4, 1, outbin);
        fwrite(&(reg.idade), 4, 1, outbin);
        fwrite(&(reg.tamNomeJogador), 4, 1, outbin);
        if(reg.tamNomeJogador > 0){
            fwrite(reg.nomeJogador, reg.tamNomeJogador, 1, outbin);
            free(reg.nomeJogador);
        }
        fwrite(&(reg.tamNacionalidade), 4, 1, outbin);
        if(reg.tamNacionalidade > 0){
            fwrite(reg.Nacionalidade, reg.tamNacionalidade, 1, outbin);
            free(reg.Nacionalidade);
        }
        fwrite(&(reg.tamNomeClube), 4, 1, outbin);
        if(reg.tamNomeClube > 0){
            fwrite(reg.nomeClube, reg.tamNomeClube, 1, outbin);
            free(reg.nomeClube);
        }

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
void funcao2(){

}

void funcao3(){

}


int main(){
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



        }
        else{
            if(numfuncao == 3){


            }


        }


    }







    return 0;
}