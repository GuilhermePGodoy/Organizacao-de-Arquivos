/* Funções para a manipulação (leitura e escrita) dos registros de dados em arquivos .csv e .bin */

#include "registros.h"


REGISTRO le_registro_csv(FILE *f){
    REGISTRO reg;
	char aux[1]; 
    int tam = 0; //Variável para a contagem do número de bytes dos campos 'id' e 'idade'.

	aux[0] = 0;
    reg.removido[0] = '0';
    reg.prox = -1;

    //Leitura do id.
    char id[10]; //Buffer para leitura do id.

    fread(aux, 1, 1, f);
    while(aux[0] != ','){
        tam++;
        fread(aux, 1, 1, f);
    }
    
    if(tam == 0)
        reg.id = -1;
    else{
    	fseek(f, (-1)*tam - 1, SEEK_CUR);
    	fread(id, tam, 1, f);
    	reg.id = atoi(id);
    	fseek(f, 1, SEEK_CUR); //Pula a vírgula.
	}


    //Leitura da idade.
    char idade[10]; //Buffer para leitura da idade.
    tam = 0;

    fread(aux, 1, 1, f);
    while(aux[0] != ','){
        tam++;
        fread(aux, 1, 1, f);
    }
    
    if(tam == 0)
        reg.idade = -1;
    else{
    	fseek(f, (-1)*tam - 1, SEEK_CUR);
    	fread(idade, tam, 1, f);
    	reg.idade = atoi(idade);
    	fseek(f, 1, SEEK_CUR); //Pula a vírgula.
    }


    //Leitura do campo 'nomeJogador'.
    reg.tamNomeJogador = 0;

    fread(aux, 1, 1, f);
    while(aux[0] != ','){
        reg.tamNomeJogador++;
        fread(aux, 1, 1, f);
    }
    if(reg.tamNomeJogador > 0){

        fseek(f, (-1)*reg.tamNomeJogador - 1, SEEK_CUR);

        reg.nomeJogador = (char *) malloc(sizeof(char)*reg.tamNomeJogador);
        if(reg.nomeJogador == NULL){
            printf("Falha no processamento do arquivo.\n");
            return reg;
        }

        fread(reg.nomeJogador, reg.tamNomeJogador, 1, f);
    }

    if(reg.tamNomeJogador > 0) //Se tamNomeJogador == 0, o ponteiro já está na posição correta (após a vírgula que marca o fim do campo).
        fseek(f, 1, SEEK_CUR); //Pula a vírgula após o nome do jogador.


    //Leitura do campo 'Nacionalidade'.
    reg.tamNacionalidade = 0;

    fread(aux, 1, 1, f);
    while(aux[0] != ','){
        reg.tamNacionalidade++;
        fread(aux, 1, 1, f);
    }

    if(reg.tamNacionalidade > 0){
        fseek(f, (-1)*reg.tamNacionalidade - 1, SEEK_CUR);

        reg.Nacionalidade = (char *) malloc(sizeof(char)*reg.tamNacionalidade);
        if(reg.Nacionalidade == NULL){
            printf("Falha no processamento do arquivo.\n");
            return reg;
        }

        fread(reg.Nacionalidade, reg.tamNacionalidade, 1, f);
    }

    if(reg.tamNacionalidade > 0) //Se tamNacionalidade == 0, o ponteiro já está na posição correta (após a vírgula que marca o fim do campo).
        fseek(f, 1, SEEK_CUR); //Pula a vírgula após a nacionalidade do jogador.


    //Leitura do campo 'nomeClube'.
    reg.tamNomeClube = 0;

    fread(aux, 1, 1, f);
    while(aux[0] != '\n'){
        reg.tamNomeClube++;
        fread(aux, 1, 1, f);
    }

    if(reg.tamNomeClube > 0){
        fseek(f, (-1)*reg.tamNomeClube - 1, SEEK_CUR);

        reg.nomeClube = (char *) malloc(sizeof(char)*reg.tamNomeClube);
        if(reg.nomeClube == NULL){
            printf("Falha no processamento do arquivo.\n");
            return reg;
        }

        fread(reg.nomeClube, reg.tamNomeClube, 1, f);
    }

    if(reg.tamNomeClube > 0)//Se tamNomeClube == 0, o ponteiro já está na posição correta (no primeiro byte do próximo registro).
        fseek(f, 1, SEEK_CUR); //Pula a quebra de linha após o nome do clube do jogador.


    //Tamanho do registro = 33 bytes dos campos de tamanho fixo + tamanhos dos campos de tamanho variável.
    reg.tamanhoRegistro = 33 + reg.tamNomeJogador + reg.tamNacionalidade + reg.tamNomeClube;

    return reg;
}

REGISTRO le_registro_bin(FILE *f){
	REGISTRO reg;

	fread(reg.removido, 1, 1, f);
	fread(&(reg.tamanhoRegistro), 4, 1, f);
	fread(&(reg.prox), 8, 1, f);
	fread(&(reg.id), 4, 1, f);
	fread(&(reg.idade), 4, 1, f);

    fread(&(reg.tamNomeJogador), 4, 1, f);
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

    return reg;
}

void escreve_registro(FILE *f, REGISTRO reg){

    fwrite(reg.removido, 1, 1, f);
    fwrite(&(reg.tamanhoRegistro), 4, 1, f);
    fwrite(&(reg.prox), 8, 1, f);
    fwrite(&(reg.id), 4, 1, f);
    fwrite(&(reg.idade), 4, 1, f);
    fwrite(&(reg.tamNomeJogador), 4, 1, f);
    if(reg.tamNomeJogador > 0){
        fwrite(reg.nomeJogador, reg.tamNomeJogador, 1, f);
        free(reg.nomeJogador);
    }
    fwrite(&(reg.tamNacionalidade), 4, 1, f);
    if(reg.tamNacionalidade > 0){
        fwrite(reg.Nacionalidade, reg.tamNacionalidade, 1, f);
        free(reg.Nacionalidade);
    }
    fwrite(&(reg.tamNomeClube), 4, 1, f);
    if(reg.tamNomeClube > 0){
        fwrite(reg.nomeClube, reg.tamNomeClube, 1, f);
        free(reg.nomeClube);
    }
}