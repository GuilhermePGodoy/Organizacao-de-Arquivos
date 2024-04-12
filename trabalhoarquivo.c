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
    printf("%s %d %ld %d %d %d %s %d %s %d %s\n", r.removido, r.tamanhoRegistro, r.prox, r.id, r.idade, r.tamNomeJogador, r.nomeJogador, r.tamNacionalidade, r.Nacionalidade, r.tamNomeClube, r.nomeClube);
}

int le_id(FILE **incsv){
    char id[6];

    fread(id, 6, 1, *incsv);

    return atoi(id);
}

int le_idade(FILE **incsv){
    char idade[2];

    fread(idade, 2, 1, *incsv);

    return atoi(idade);
}

char* le_nomeJogador(FILE **incsv, int tamNome){
    char *nome = malloc(sizeof(char)*tamNome);
    if(nome == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    fread(nome, tamNome, 1, *incsv);
    nome[tamNome - 1] = '\0';

    return nome;
}

char* le_nacionalidade(FILE **incsv, int tamNacionalidade){
    char *nacionalidade = malloc(sizeof(char)*tamNacionalidade);
    if(nacionalidade == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    fread(nacionalidade, tamNacionalidade, 1, *incsv);
    nacionalidade[tamNacionalidade - 1] = '\0';

    return nacionalidade;
}

char* le_nomeClube(FILE **incsv, int tamNomeClube){
    char *nomeClube = malloc(sizeof(char)*tamNomeClube);
    if(nomeClube == NULL){
        printf("Falha no processamento do arquivo.\n");
        exit(1);
    }

    fread(nomeClube, tamNomeClube, 1, *incsv);
    nomeClube[tamNomeClube - 1] = '\0';

    return nomeClube;
}

void funcao1(char* texto, char* bin){
    struct cabecalho cab;
    struct registro reg;

    FILE *incsv;
    incsv = fopen(texto,"r");
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


    while(!feof(incsv)){
        char aux[1]; //string auxiliar para contagem do tamanho dos campos de tamanho variável.
        
        reg.removido[0] = '0';
        reg.prox = -1;
        
        reg.id = le_id(&incsv);
        incsv += 1;

        reg.idade = le_idade(&incsv);
        incsv += 1;

        reg.tamNomeJogador = 0;

        fread(aux, 1, 1, incsv);
        while(aux[0] != 44){
            reg.tamNomeJogador++;
            incsv += 1;
            fread(aux, 1, 1, incsv);
        }
        incsv -= reg.tamNomeJogador;
        reg.nomeJogador = le_nomeJogador(&incsv, reg.tamNomeJogador + 1);

        incsv += 1;

        reg.tamNacionalidade = 0;

        fread(aux, 1, 1, incsv);
        while(aux[0] != 44){
            reg.tamNacionalidade++;
            incsv += 1;
            fread(aux, 1, 1, incsv);
        }
        incsv -= reg.tamNacionalidade;
        reg.Nacionalidade = le_nacionalidade(&incsv, reg.tamNacionalidade + 1);

        incsv += 1;


        reg.tamNomeClube = 0;

        fread(aux, 1, 1, incsv);

        if(aux[0] != 10){
            while(aux[0] != 10){
                reg.tamNomeClube++;
                incsv += 1;
                fread(aux, 1, 1, incsv);
            }
            incsv -= reg.tamNomeClube;
            reg.nomeClube = le_nomeClube(&incsv, reg.tamNomeClube + 1);
            incsv += 1;
        }

        fwrite(&reg, sizeof(struct registro), 1, outbin);

        free(reg.nomeJogador);
        free(reg.Nacionalidade);
        free(reg.nomeClube);
    }

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