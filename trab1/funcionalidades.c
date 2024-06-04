#include "registros.h"
#include "indice.h"

/*
Funções auxiliares para ordenação dos registros do índice por id.
*/

void trocar(REG_INDICE *a, REG_INDICE *b) {
    REG_INDICE temp = *a;
    *a = *b;
    *b = temp;
}

// Função para particionar o vetor
int particionar(REG_INDICE vetor[], int menor, int maior) {
    int pivot = (vetor[maior]).id;
    int i = (menor - 1);

    for (int j = menor; j <= maior - 1; j++) {
        if ((vetor[j]).id <= pivot) {
            i++;
            trocar(&vetor[i], &vetor[j]);
        }
    }
    trocar(&vetor[i + 1], &vetor[maior]);
    return (i + 1);
}

// Função principal que implementa o QuickSort
void quickSort(REG_INDICE vetor[], int menor, int maior) {
    if (menor < maior){
        // pi é o índice da partição, vetor[pi] está no lugar certo
        int pi = particionar(vetor, menor, maior);

        // Separadamente ordena os elementos antes e depois da partição
        quickSort(vetor, menor, pi - 1);
        quickSort(vetor, pi + 1, maior);
    }
}

//Funcionalidade 4: criação do índice.
int funcao4(FILE *dados, FILE *indice){
	CABECALHO cab = le_cabecalho(dados); //Struct para armazenar os campos do registro de cabeçalho do arquivo de dados.
	REGISTRO reg; //Struct para armazenar os campos de um registro do arquivo de dados.

	//Durante a criação, os dados do índice estão inconsistentes. Portanto, 'status' recebe 0.
	char status[1];
	status[0] = '0';
	fwrite(status, 1, 1, indice);

	//Verifica se o arquivo de dados está consistente.
	if((cab.status)[0] == '0'){
		return 0;
	}


	REG_INDICE registros[cab.nroRegArq]; //Vetor usado para armazenar todos os registros do índice, que serão ordenados por 'id' posteriormente.

	long int byteOffset = 25; //Variável que armazena o byteOffset do registro que está sendo tratado no momento.
	int j = 0;

	for(int i = 0; i < cab.nroRegArq + cab.nroRegRem; i++){

		reg = le_registro_bin(dados);

		if((reg.removido)[0] == '0'){
			(registros[j]).id = reg.id;
			(registros[j]).byteOffset = byteOffset;
			j++;
		}

		byteOffset += reg.tamanhoRegistro;

        libera_registro(reg);
	}

	quickSort(registros, 0, cab.nroRegArq - 1);

	for(int i = 0; i < cab.nroRegArq; i++){
		escreve_registro_indice(indice, registros[i]);
	}

	status[0] = '1';
	rewind(indice);
	fwrite(status, 1, 1, indice);

    return 1;
}


//Struct que contém as informações necessárias para cada busca da funcionalidade 5.
struct como_busca{
    int m; //Número de campos que serão usados na busca.

    int busca_id; //Vale 1 se o campo 'id' deve ser utilizado, e 0 caso contrário.
    int id; //Se 'busca_id' == 1, esta variável contém o id buscado.

    int busca_idade; //Vale 1 se o campo 'idade' deve ser utilizado, e 0 caso contrário.
    int idade; //Se 'busca_idade' == 1, esta variável contém a idade buscada.

    int busca_nome; //Indica se o campo 'nomeJogador' deve ser utilizado na busca.
    char nome[50];  //Contém o valor do campo buscado.
    
    int busca_nacionalidade; //Indica se o campo 'Nacionalidade' deve ser utilizado na busca.
    char nacionalidade[50]; //Contém o valor do campo buscado.

    int busca_clube; //Indica se o campo 'nomeClube' deve ser utilizado na busca.
    char clube[50]; //Contém o valor do campo buscado.
};


void insere_listaRemovidos(FILE *dados, long int byteOffset, REGISTRO reg){

    long int byteOffset_atual;
    REGISTRO atual, proximo;

    rewind(dados);
    CABECALHO cab = le_cabecalho(dados);

    //Inserção na lista vazia.
    if(cab.topo == -1){
        cab.topo = byteOffset;
        reg.prox = -1;

        rewind(dados);
        escreve_cabecalho(dados, cab);
        fseek(dados, byteOffset, SEEK_SET);
        escreve_registro(dados, reg);

        return;
    }

    fseek(dados, cab.topo, SEEK_SET);
    atual = le_registro_bin(dados);
    byteOffset_atual = cab.topo;
    
    //Inserção no início da lista.
    if(reg.tamanhoRegistro <= atual.tamanhoRegistro){
        reg.prox = byteOffset_atual;
        cab.topo = byteOffset;

        rewind(dados);
        escreve_cabecalho(dados, cab);
        fseek(dados, byteOffset, SEEK_SET);
        escreve_registro(dados, reg);

        libera_registro(atual);

        return;
    }

    //Inserção no fim da lista com apenas um elemento.
    if(atual.prox == -1){
        atual.prox = byteOffset;
        reg.prox = -1;

        fseek(dados, byteOffset_atual, SEEK_SET);
        escreve_registro(dados, atual);
        fseek(dados, byteOffset, SEEK_SET);
        escreve_registro(dados, reg);

        libera_registro(atual);

        return;
    }

    fseek(dados, atual.prox, SEEK_SET);
    proximo = le_registro_bin(dados);

    while(proximo.prox != -1 && reg.tamanhoRegistro > proximo.tamanhoRegistro){

        libera_registro(atual);

        byteOffset_atual = atual.prox;
        atual = proximo;
        fseek(dados, proximo.prox, SEEK_SET);
        proximo = le_registro_bin(dados);
    }

    //Inserção no fim.
    if(proximo.prox == -1 && reg.tamanhoRegistro > proximo.tamanhoRegistro){
        proximo.prox = byteOffset;
        reg.prox = -1;

        fseek(dados, (-1)*proximo.tamanhoRegistro, SEEK_CUR);
        escreve_registro(dados, proximo);
        fseek(dados, byteOffset, SEEK_SET);
        escreve_registro(dados, reg);

        libera_registro(atual);
        libera_registro(proximo);

        return;
    }

    //Inserção no meio.
    reg.prox = atual.prox;
    atual.prox = byteOffset;

    fseek(dados, byteOffset_atual, SEEK_SET);
    escreve_registro(dados, atual);
    fseek(dados, byteOffset, SEEK_SET);
    escreve_registro(dados, reg);

    libera_registro(atual);
    libera_registro(proximo);
}

int compara(REGISTRO reg, struct como_busca busca){

    int camposIguais = 0;

    if(busca.busca_id == 1){
        if(reg.id == busca.id)
            camposIguais++;
    }

    if(busca.busca_idade == 1){ //Verifica se a busca por 'idade' deve ser feita.
        if(reg.idade == busca.idade) //Se sim, realiza a comparação.
            camposIguais++; //Se passar na busca, incrementa 'camposIguais'.
    }

    if(busca.busca_nome == 1){ //Verifica se a busca por 'nomeJogador' deve ser feita.
        if(strcmp(reg.nomeJogador, busca.nome) == 0) //Se sim, realiza a comparação.
            camposIguais++; //Se passar na busca, incrementa 'camposIguais'.
    }

    if(busca.busca_nacionalidade == 1){ //Verifica se a busca por 'Nacionalidade' deve ser feita.
        if(strcmp(reg.Nacionalidade, busca.nacionalidade) == 0) //Se sim, realiza a comparação.
            camposIguais++; //Se passar na busca, incrementa 'camposIguais'.
    }

    if(busca.busca_clube == 1){ //Verifica se a busca por 'nomeClube' deve ser feita.
        if(strcmp(reg.nomeClube, busca.clube) == 0) //Se sim, realiza a comparação.
            camposIguais++; //Se passar na busca, incrementa 'camposIguais'.
    }

    if(camposIguais == busca.m){ //Se o registro atende a todas as buscas por campo feitas, então camposIguais == m.
        return 1;
    }

    return 0;
}


//Funcionalidade 5: realiza a remoção de registros por qualquer campo.
int funcao5(FILE *dados, FILE *arq_indice, int n){
	CABECALHO cab = le_cabecalho(dados);

/* O status já está como 0 nos arquivos dados
	if((cab.status)[0] == '0'){
		return 0;
	}
*/
    (cab.status)[0] = '0';
    rewind(dados);
    escreve_cabecalho(dados, cab);

    REG_INDICE *indice = carrega_indice(arq_indice, cab.nroRegArq);


    struct como_busca buscas[n]; //Vetor das structs com as informações das n buscas a serem realizadas.
    char nomeCampo[50]; //Vetor auxiliar para ler o nome do campo que deve ser usado na busca.
    REGISTRO reg; //Struct que receberá os registros do arquivo.


    //Inicialização dos campos das structs do vetor 'buscas'.
    for(int i = 0; i < n; i++){
        (buscas[i]).busca_id = 0;
        (buscas[i]).busca_idade = 0;
        (buscas[i]).busca_nome = 0;
        (buscas[i]).busca_nacionalidade = 0;
        (buscas[i]).busca_clube = 0;
    }


    for(int i = 0; i < n; i++){ //Leitura da entrada com as especificações das buscas.

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

    long int byteOffset;

    for(int i = 0; i < n; i++){ //Loop para as n buscas.

        byteOffset = 25;

        if((buscas[i]).busca_id == 1){

            byteOffset = busca_id_indice(indice, cab.nroRegArq, (buscas[i]).id);

            if(byteOffset == -1){
                fseek(dados, 25, SEEK_SET);
                continue;
            }

            fseek(dados, byteOffset, SEEK_SET);
            reg = le_registro_bin(dados);

            if(compara(reg, buscas[i])){ //Se o registro atende a todas as buscas por campo feitas, então camposIguais == m.

                (reg.removido)[0] = '1';

                insere_listaRemovidos(dados, byteOffset, reg);

                remove_registro_indice(indice, cab.nroRegArq, reg.id);

                cab.nroRegArq--;
                cab.nroRegRem++;
            }

            libera_registro(reg);
        }
        else{
            for(int j = 0; j < cab.nroRegArq + cab.nroRegRem; j++){ //Loop para percorrer o arquivo binário inteiro.

                reg = le_registro_bin(dados); 

                if((reg.removido)[0] == '1'){ //Se o registro estiver marcado como removido, pula-se este registro.
                    byteOffset += (long int) reg.tamanhoRegistro;
                    libera_registro(reg);

                    continue;
                }

                if(compara(reg, buscas[i])){ //Se o registro atende a todas as buscas por campo feitas, então camposIguais == m.

           			(reg.removido)[0] = '1';

                    insere_listaRemovidos(dados, byteOffset, reg);

                    remove_registro_indice(indice, cab.nroRegArq, reg.id);

                    cab.nroRegArq--;
                    cab.nroRegRem++;
                }

                byteOffset += (long int) reg.tamanhoRegistro;
                
                libera_registro(reg);

                fseek(dados, byteOffset, SEEK_SET);
            }
        }

        fseek(dados, 25, SEEK_SET);
    }

    rewind(dados);
    (cab.status)[0] = '1';
    fwrite(cab.status, 1, 1, dados);


    FILE *novo_indice = fopen("novoindice.bin", "wb");
    if(novo_indice == NULL)
        return 0;


    rewind(arq_indice);

    char status[1];
    status[0] = '1';
    fwrite(status, 1, 1, novo_indice);

    for(int i = 0; i < cab.nroRegArq; i++)
        escreve_registro_indice(novo_indice, indice[i]);

    free(indice);

    fclose(novo_indice);
    printf("novo_indice: ");
    binarioNaTela("novoindice.bin");


    return 1;
}


void funcao6(FILE *dados, FILE *indice){
	CABECALHO cab = le_cabecalho(dados);

	if((cab.status)[0] == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
}