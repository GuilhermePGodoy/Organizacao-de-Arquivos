#include "registros.h"

REG_INDICE* carrega_indice(FILE *arq_indice, int nroReg){
    REG_INDICE *indice = malloc(nroReg*sizeof(REG_INDICE));
    if(indice == NULL)
        return NULL;

    rewind(arq_indice);

    for(int i = 0; i < nroReg; i++)
        indice[i] = le_registro_indice(arq_indice);

    return indice;
}

int busca_binaria(REG_INDICE indice[], int tamanho, int id) {
    int inicio = 0;
    int fim = tamanho - 1;

    while (inicio <= fim) {
        int meio = inicio + (fim - inicio) / 2;

        // Verifica se o id está no meio
        if ((indice[meio]).id == id) {
            return meio;
        }
        
        // Se o id for maior, ignore a metade esquerda
        if ((indice[meio]).id < id) {
            inicio = meio + 1;
        }
        // Se o id for menor, ignore a metade direita
        else {
            fim = meio - 1;
        }
    }

    // Alvo não encontrado
    return -1;
}

void quickSort(REG_INDICE vetor[], int menor, int maior);

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

	long int byteOffset = 25; //Variável que armazena o byteOffset do registro que está sendo tratado no momento. Ini
	int j = 0;

	for(int i = 0; i < cab.nroRegArq + cab.nroRegRem; i++){
		reg = le_registro_bin(dados);
		
		if((reg.removido)[0] == '0'){
			(registros[j]).id = reg.id;
			(registros[j]).byteOffset = byteOffset;
			j++;
		}

		byteOffset += reg.tamanhoRegistro;
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


//Struct que contém as informações necessárias para cada busca da funcionalidade 3.
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


void insere_encadeada(FILE *dados, long int byteOffset, REGISTRO reg){
    rewind(dados);
    CABECALHO cab = le_cabecalho(dados);
    long int prox = cab.topo;
    REGISTRO atual, proximo;

    if(prox == -1){
        cab.topo = byteOffset;
        reg.prox = -1;
    }

    rewind(dados);
    escreve_cabecalho(cab, dados);
    fseek(dados, byteOffset, SEEK_SET);
    escreve_registro(dados, reg);

    return;


    while(prox != -1){
        fseek(dados, prox, SEEK_SET);
        atual = le_registro_bin(dados);

        if(atual.prox == -1){
            atual.prox = byteOffset;
            reg.prox = -1;


            break;
        }
        else{ //Inserir no meio.
            fseek(dados, atual.prox, SEEK_SET);
            proximo = le_registro_bin(dados);

            if(reg.tamanhoRegistro <= proximo.tamanhoRegistro){
                reg.prox = atual.prox;
                atual.prox = byteOffset;
            }
        }

        fseek(dados, (-1)*(atual.tamanhoRegistro), SEEK_CUR);
        escreve_registro(dados, atual);

        fseek(dados, atual.prox, SEEK_SET);
        escreve_registro(dados, reg);
    }
}


//Funcionalidade 5:
void funcao5(FILE *dados, FILE *arq_indice, int n){
	CABECALHO cab = le_cabecalho(dados);

	if((cab.status)[0] == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}

    (cab.status)[0] = '0';
    rewind(dados);
    escreve_cabecalho(cab, dados);

    REG_INDICE *indice = carrega_indice(arq_indice, cab.nroRegArq);


    struct como_busca buscas[n]; //Vetor das structs com as informações das n buscas a serem realizadas.
    char nomeCampo[20]; //Vetor auxiliar para ler o nome do campo que deve ser usado na busca.
    REGISTRO reg; //Struct que receberá os registros do arquivo.


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

    char controle; //Variável auxiliar para monitorar o fim do arquivo.
    int cont = 0; //Variável que armazena o número de campos do registro atual que passam na busca (ou seja, são iguais aos buscados).
    int flag = 0; //Flag utilizada para indicar que a busca deve parar. Utilizada para buscas por id, visto que este campo é único para cada jogador.
    long int byteOffset;

    for(int i = 0; i < n; i++){ //Loop para as n buscas.
        flag = 0;

        printf("Busca %d\n\n", i + 1);

        byteOffset = 25;
        while((controle = getc(dados)) != EOF){ //Loop para percorrer o arquivo binário inteiro.
            fseek(dados, -1, SEEK_CUR); //Se não está no fim do arquivo, volta à posição lida por 'getc()'.

            cont = 0;


            reg = le_registro_bin(dados); 

            byteOffset += reg.tamanhoRegistro;


            if((buscas[i]).busca_id == 1){ //Verifica se deve fazer a busca pelo campo 'id'.
                if(reg.id == (buscas[i]).id){ //Se sim, verifica se o campo 'id' do registro é igual ao id buscado.
                    cont++; //Se sim, incrementa 'cont'.
                    flag = 1; //O id buscado foi encontrado e, por ser único para cada jogador, não é necessário continuar a busca pelo arquivo. Logo, 'flag' deve receber 1.
                    if((reg.removido)[0] == '1'){ //O id buscado foi encontrado, mas o registro está marcado como removido. Logo, a busca não foi bem-sucedida e deve parar.
                        free(reg.nomeJogador);
                        free(reg.Nacionalidade);
                        free(reg.nomeClube);

                        break; //Encerra a busca.
                    }
                }
            }

            if((reg.removido)[0] == '1'){ //Se o registro estiver marcado como removido, pula-se este registro.
                free(reg.nomeJogador);
                free(reg.Nacionalidade);
                free(reg.nomeClube);
                continue;
            }


            if((buscas[i]).busca_idade == 1){ //Verifica se a busca por 'idade' deve ser feita.
                if(reg.idade == (buscas[i]).idade) //Se sim, realiza a comparação.
                    cont++; //Se passar na busca, incrementa 'cont'
            }


            if((buscas[i]).busca_nome == 1){ //Verifica se a busca por 'nomeJogador' deve ser feita.
                if(strcmp(reg.nomeJogador, (buscas[i]).nome) == 0) //Se sim, realiza a comparação.
                    cont++; //Se passar na busca, incrementa 'cont'
            }


            if((buscas[i]).busca_nacionalidade == 1){ //Verifica se a busca por 'Nacionalidade' deve ser feita.
                if(strcmp(reg.Nacionalidade, (buscas[i]).nacionalidade) == 0) //Se sim, realiza a comparação.
                    cont++; //Se passar na busca, incrementa 'cont'
            }
            if((buscas[i]).busca_clube == 1){ //Verifica se a busca por 'nomeClube' deve ser feita.
                if(strcmp(reg.nomeClube, (buscas[i]).clube) == 0) //Se sim, realiza a comparação.
                    cont++; //Se passar na busca, incrementa 'cont'
            }


            if(cont == (buscas[i]).m){ //Se o registro atende a todas as buscas por campo feitas, então cont == m.
            	fseek(dados, (-1)*reg.tamanhoRegistro, SEEK_CUR);

       			(reg.removido)[0] = '1';

                insere_encadeada(dados, byteOffset, reg);

                int posicaoRegRemovido = busca_binaria(indice, cab.nroRegArq, reg.id);

                int i;
                for(i = 0; i < posicaoRegRemovido; i++)
                    escreve_registro_indice(arq_indice, indice[i]);

                for(i = posicaoRegRemovido + 1; i < cab.nroRegArq; i++)
                    escreve_registro_indice(arq_indice, indice[i]);

                cab.nroRegArq--;
                cab.nroRegRem++;
            }

            free(reg.nomeJogador);
            free(reg.Nacionalidade);
            free(reg.nomeClube);

            if(flag == 1)  //Se 'flag' == 1, encerra a busca.
                break;

        }
        fseek(dados, 25, SEEK_SET);
    }

    rewind(dados);
    escreve_cabecalho(cab, dados);

    free(indice);

    return;
}



void funcao6(FILE *dados, FILE *indice){
	CABECALHO cab = le_cabecalho(dados);

	if((cab.status)[0] == '0'){
		printf("Falha no processamento do arquivo.\n");
		return;
	}
}