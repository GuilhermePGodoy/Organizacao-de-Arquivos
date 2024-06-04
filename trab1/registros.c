/* Funções para a manipulação (leitura e escrita) dos registros de dados em arquivos .csv e .bin */

#include "registros.h"

CABECALHO le_cabecalho(FILE *f){
    CABECALHO cab;

    fread(cab.status, 1, 1, f);
    fread(&(cab.topo), 8, 1, f);
    fread(&(cab.proxByteOffset), 8, 1, f);
    fread((&cab.nroRegArq), 4, 1, f);
    fread((&cab.nroRegRem), 4, 1, f);

    return cab;
}

void escreve_cabecalho(FILE *dados, CABECALHO cab){
    fwrite(cab.status, 1, 1, dados);
    fwrite(&(cab.topo), 8, 1, dados);
    fwrite(&(cab.proxByteOffset), 8, 1, dados);
    fwrite(&(cab.nroRegArq), 4, 1, dados);
    fwrite(&(cab.nroRegRem), 4, 1, dados);
}

//Função que lê um registro de um arquivo .csv e retorna uma struct REGISTRO com os campos.
REGISTRO le_registro_csv(FILE *f){
    REGISTRO reg;
	char aux[1]; //Variável auxiliar para ler os campos até a vírgula.
    int tam = 0; //Variável para a contagem do número de bytes dos campos 'id' e 'idade'.

	aux[0] = 0;
	//Inicialização de campos do registro.
    reg.removido[0] = '0';
    reg.prox = -1;

    //Leitura do id.
    char id[10]; //Buffer para leitura do id.

    fread(aux, 1, 1, f);
    while(aux[0] != ','){ //Lê o campo 'id' até a vírgula.
        tam++; //Contagem do número de bytes do campo.
        fread(aux, 1, 1, f);
    }
    
    if(tam == 0) //Se o campo estiver vazio, 'id' recebe o valor -1.
        reg.id = -1;
    else{
    	fseek(f, (-1)*tam - 1, SEEK_CUR); //Volta para o início do campo.
    	fread(id, tam, 1, f); //Lê o campo e armazena no buffer.
    	reg.id = atoi(id); //Converte a string em inteiro e armazena na struct.
    	fseek(f, 1, SEEK_CUR); //Pula a vírgula após o id.
	}


    //Leitura da idade.
    char idade[10]; //Buffer para leitura da idade.
    tam = 0;

    fread(aux, 1, 1, f);
    while(aux[0] != ','){ //Lê o campo até a vírgula.
        tam++; //Contagem do número de bytes do campo.
        fread(aux, 1, 1, f);
    }
    
    if(tam == 0) //Se o campo estiver vazio, 'idade' recebe o valor -1.
        reg.idade = -1;
    else{
    	fseek(f, (-1)*tam - 1, SEEK_CUR); //Volta para o início do campo.
    	fread(idade, tam, 1, f); //Lê o campo e armazena no buffer.
    	reg.idade = atoi(idade); //Converte a string em inteiro e armazena na struct.
    	fseek(f, 1, SEEK_CUR); //Pula a vírgula após a idade.
    }


    //Leitura do campo 'nomeJogador'.
    reg.tamNomeJogador = 0;

    fread(aux, 1, 1, f);
    while(aux[0] != ','){ //Lê o campo até a vírgula, contando o tamanho, em bytes.
        reg.tamNomeJogador++;
        fread(aux, 1, 1, f);
    }

    if(reg.tamNomeJogador > 0){ //Se o campo não for vazio, aloca dinamicamente o espaço para o valor do campo e o armazena na struct.

        fseek(f, (-1)*reg.tamNomeJogador - 1, SEEK_CUR); //Volta para o início do campo.

        reg.nomeJogador = (char *) malloc(sizeof(char)*reg.tamNomeJogador);
        if(reg.nomeJogador == NULL){
            printf("Falha no processamento do arquivo.\n");
            return reg;
        }

        fread(reg.nomeJogador, reg.tamNomeJogador, 1, f); //Armazena o campo na struct.
        fseek(f, 1, SEEK_CUR); //Pula a vírgula após o nome do jogador.
    }

    //Leitura do campo 'Nacionalidade'.
    reg.tamNacionalidade = 0;

    fread(aux, 1, 1, f);
    while(aux[0] != ','){ //Lê o campo até a vírgula, contando o tamanho, em bytes.
        reg.tamNacionalidade++;
        fread(aux, 1, 1, f);
    }

    if(reg.tamNacionalidade > 0){ //Se o campo não for vazio, aloca dinamicamente o espaço para o valor do campo e o armazena na struct.

        fseek(f, (-1)*reg.tamNacionalidade - 1, SEEK_CUR); //Volta para o início do campo.

        reg.Nacionalidade = (char *) malloc(sizeof(char)*reg.tamNacionalidade);
        if(reg.Nacionalidade == NULL){
            printf("Falha no processamento do arquivo.\n");
            return reg;
        }

        fread(reg.Nacionalidade, reg.tamNacionalidade, 1, f); //Armazena o campo na struct.
        fseek(f, 1, SEEK_CUR); //Pula a vírgula após a nacionalidade do jogador.
    }

    //Leitura do campo 'nomeClube'.
    reg.tamNomeClube = 0;

    fread(aux, 1, 1, f);
    while(aux[0] != '\n'){ //Lê o campo até a vírgula, contando o tamanho, em bytes.
        reg.tamNomeClube++;
        fread(aux, 1, 1, f);
    }

    if(reg.tamNomeClube > 0){ //Se o campo não for vazio, aloca dinamicamente o espaço para o valor do campo e o armazena na struct.

        fseek(f, (-1)*reg.tamNomeClube - 1, SEEK_CUR); //Volta para o início do campo.

        reg.nomeClube = (char *) malloc(sizeof(char)*reg.tamNomeClube);
        if(reg.nomeClube == NULL){
            printf("Falha no processamento do arquivo.\n");
            return reg;
        }

        fread(reg.nomeClube, reg.tamNomeClube, 1, f); //Armazena o campo na struct.
        fseek(f, 1, SEEK_CUR); //Pula a quebra de linha após o nome do clube do jogador.
    }

    //Tamanho do registro = 33 bytes dos campos de tamanho fixo + tamanhos dos campos de tamanho variável.
    reg.tamanhoRegistro = 33 + reg.tamNomeJogador + reg.tamNacionalidade + reg.tamNomeClube;

    return reg;
}

//Função que lê um registro de um arquivo .bin e retorna uma struct REGISTRO com os campos.
REGISTRO le_registro_bin(FILE *f){
	REGISTRO reg;

    reg.tamNomeJogador = 0;
    reg.Nacionalidade = 0;
    reg.tamNomeClube = 0;

    int tamUsado = 33; //Variável que armazenará o tamanho efetivamente usado de 'tamanhoRegistro', visto que pode haver lixo no fim do registro. Começa com 33, que é a soma dos tamanhos dos campos de tamanho fixo.

	//Leitura dos 5 primeiros campos, todos de tamanho fixo.
	fread(reg.removido, 1, 1, f);
	fread(&(reg.tamanhoRegistro), 4, 1, f);
	fread(&(reg.prox), 8, 1, f);
	fread(&(reg.id), 4, 1, f);
	fread(&(reg.idade), 4, 1, f);

	//Leitura dos campos de tamanho variável:
	//Lê o campo 'tamCampo' e aloca dinamicamente espaço para o valor do campo e para o '\0' (para facilitar operações como o print do campo na tela).
    fread(&(reg.tamNomeJogador), 4, 1, f);
    tamUsado += reg.tamNomeJogador;
    reg.nomeJogador = (char *) malloc(sizeof(char)*(reg.tamNomeJogador) + 1);
    fread(reg.nomeJogador, reg.tamNomeJogador, 1, f);
    reg.nomeJogador[reg.tamNomeJogador] = '\0';

    fread(&reg.tamNacionalidade, 4, 1, f);
    tamUsado += reg.tamNacionalidade;
    reg.Nacionalidade = (char *) malloc(sizeof(char)*(reg.tamNacionalidade) + 1);
    fread(reg.Nacionalidade, reg.tamNacionalidade, 1, f);
    reg.Nacionalidade[reg.tamNacionalidade] = '\0';

    fread(&reg.tamNomeClube, 4, 1, f);
    tamUsado += reg.tamNomeClube;
    reg.nomeClube = (char *) malloc(sizeof(char)*(reg.tamNomeClube) + 1);
    fread(reg.nomeClube, reg.tamNomeClube, 1, f);
    reg.nomeClube[reg.tamNomeClube] = '\0';

    if(tamUsado != reg.tamanhoRegistro){
        fseek(f, reg.tamanhoRegistro - tamUsado, SEEK_CUR); //Posiciona o cursor após o lixo do fim do registro (se não há lixo, tamanhoRegistro - tamUsado == 0 e o cursor está na posição correta).
    }
    
    return reg;
}

//Função que escreve um registro passado como parâmetro em um arquivo .bin
void escreve_registro(FILE *f, REGISTRO reg){

	//Escrita dos 5 primeiros campos, todos de tamanho fixo.
    fwrite(reg.removido, 1, 1, f);
    fwrite(&(reg.tamanhoRegistro), 4, 1, f);
    fwrite(&(reg.prox), 8, 1, f);
    fwrite(&(reg.id), 4, 1, f);
    fwrite(&(reg.idade), 4, 1, f);

    //Escrita dos campos de tamanho variável:
    //Escreve o campo 'tamCampo' e, se o campo não for vazio, escreve-o e desaloca o ponteiro da struct.
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

REG_INDICE le_registro_indice(FILE *f){
    REG_INDICE reg;
    fread(&(reg.id), 4, 1, f);
    fread(&(reg.byteOffset), 8, 1, f);

    return reg;
}

void escreve_registro_indice(FILE *f, REG_INDICE reg){
    fwrite(&(reg.id), 4, 1, f);
    fwrite(&(reg.byteOffset), 8, 1, f);
}