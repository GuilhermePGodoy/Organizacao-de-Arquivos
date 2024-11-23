/* Funções para a manipulação (leitura e escrita) dos registros de dados em arquivos .csv e .bin */

#include "registros.h"

//Função auxiliar para imprimir os dados de um registro.
void imprimeRegistro(REGISTRO_DADOS reg){
    printf("\nRemovido: %c\n", (reg.removido)[0]);
    printf("tamanho: %d\n", reg.tamanhoRegistro);
    printf("prox: %ld\n", reg.prox);
    printf("id: %d\n", reg.id);
    printf("idade: %d\n", reg.idade);
    printf("tamNome: %d\n", reg.tamNomeJogador);
    printf("nome: %s\n", reg.nomeJogador);
    printf("tamClube: %d\n", reg.tamNacionalidade);
    printf("clube: %s\n", reg.Nacionalidade);
    printf("tamNac: %d\n", reg.tamNomeClube);
    printf("nac: %s\n", reg.nomeClube);
}

//Lê os campos do registro de cabeçalho e retorna uma struct CABECALHO_DADOS com os dados.
CABECALHO_DADOS le_cabecalho_dados(FILE *f){
    CABECALHO_DADOS cab;

    rewind(f);

    fread(cab.status, 1, 1, f);
    fread(&(cab.topo), 8, 1, f);
    fread(&(cab.proxByteOffset), 8, 1, f);
    fread((&cab.nroRegArq), 4, 1, f);
    fread((&cab.nroRegRem), 4, 1, f);

    return cab;
}

//Recebe uma struct CABECALHO_DADOS e escreve seus campos no arquivo de dados.
void escreve_cabecalho_dados(FILE *dados, CABECALHO_DADOS cab){
    rewind(dados);

    fwrite(cab.status, 1, 1, dados);
    fwrite(&(cab.topo), 8, 1, dados);
    fwrite(&(cab.proxByteOffset), 8, 1, dados);
    fwrite(&(cab.nroRegArq), 4, 1, dados);
    fwrite(&(cab.nroRegRem), 4, 1, dados);
}


//Função que lê um registro de um arquivo .bin e retorna uma struct REGISTRO_DADOS com os campos.
REGISTRO_DADOS le_registro_dados(FILE *f){
	REGISTRO_DADOS reg;

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
void escreve_registro_dados(FILE *f, REGISTRO_DADOS reg){

	//Escrita dos 5 primeiros campos, todos de tamanho fixo.
    fwrite(reg.removido, 1, 1, f);
    fwrite(&(reg.tamanhoRegistro), 4, 1, f);
    fwrite(&(reg.prox), 8, 1, f);
    fwrite(&(reg.id), 4, 1, f);
    fwrite(&(reg.idade), 4, 1, f);

    //Escrita dos campos de tamanho variável:
    //Escreve o campo 'tamCampo' e, se o campo não for vazio, escreve-o e desaloca o ponteiro da struct.
    fwrite(&(reg.tamNomeJogador), 4, 1, f);
    if(reg.tamNomeJogador > 0)
        fwrite(reg.nomeJogador, reg.tamNomeJogador, 1, f);

    fwrite(&(reg.tamNacionalidade), 4, 1, f);
    if(reg.tamNacionalidade > 0)
        fwrite(reg.Nacionalidade, reg.tamNacionalidade, 1, f);

    fwrite(&(reg.tamNomeClube), 4, 1, f);
    if(reg.tamNomeClube > 0)
        fwrite(reg.nomeClube, reg.tamNomeClube, 1, f);
}


//Libera a memória alocada pela função 'le_registro_dados'.
void libera_registro_dados(REGISTRO_DADOS reg){
    free(reg.nomeJogador);
    free(reg.Nacionalidade);
    free(reg.nomeClube);
}