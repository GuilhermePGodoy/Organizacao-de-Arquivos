#include "registros.h"
#include "indice.h"

//Funcionalidade 4: criação do índice.
int funcao4(FILE *dados, FILE *indice){

	CABECALHO cab = le_cabecalho(dados); //Struct para armazenar os campos do registro de cabeçalho do arquivo de dados.
	REGISTRO reg; //Struct para armazenar os campos de um registro do arquivo de dados.

	//Durante a criação, os dados do índice estão inconsistentes. Portanto, 'status' recebe 0.
	char status[1];
	status[0] = '0';
	fwrite(status, 1, 1, indice);

	//Verifica se o arquivo de dados está consistente.
	if((cab.status)[0] == '0')
		return 0;


	REG_INDICE registros[cab.nroRegArq]; //Vetor usado para armazenar todos os registros do índice, que serão ordenados por 'id' posteriormente.

	long int byteOffset = 25; //Variável que armazena o byteOffset do registro que está sendo tratado no momento.
	int j = 0; //variável para percorrer o vetor com os registros do índice.

	for(int i = 0; i < cab.nroRegArq + cab.nroRegRem; i++){ //Loop para percorrer o arquivo de dados inteiro.

		reg = le_registro_bin(dados); //Lê o próximo registro do arquivo.

		if((reg.removido)[0] == '0'){ //Se o registro não está removido, armazena seu id e seu byteOffset na posição atual de 'indice'
			(registros[j]).id = reg.id;
			(registros[j]).byteOffset = byteOffset;
			j++; //Incrementa 'j' para acessar a próxima posição livre no vetor 'indice'.
		}

		byteOffset += reg.tamanhoRegistro; //Atualiza 'byteOffset' para o byte offset do próximo registro do arquivo.

        libera_registro(reg); //Libera a memória alocada para o registro.
	}

    //Ordenação dos registros do índice por id.
	ordena_id(registros, 0, cab.nroRegArq - 1);

    //Escrita dos registros do vetor no arquivo de índice.
	for(int i = 0; i < cab.nroRegArq; i++){
		escreve_registro_indice(indice, registros[i]);
	}

    //As escritas no arquivo acabaram, logo 'status' recebe 1.
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


//Função auxiliar que insere um registro removido na lista encadeada de removidos.
void insere_listaRemovidos(FILE *dados, long int byteOffset, REGISTRO reg){

    long int byteOffset_atual;
    REGISTRO atual, proximo;

    rewind(dados);
    CABECALHO cab = le_cabecalho(dados);

    //Inserção na lista vazia.
    if(cab.topo == -1){
        //Atualiza o campo 'topo' do cabeçalho para o byte offset do registro a ser inserido na lista.
        cab.topo = byteOffset;
        //'reg' é o último registro da lista, logo 'prox' deve receber -1.
        reg.prox = -1;

        //Escreve as alterações no arquivo.
        rewind(dados);
        escreve_cabecalho(dados, cab);
        fseek(dados, byteOffset, SEEK_SET);
        escreve_registro(dados, reg);

        return;
    }

    fseek(dados, cab.topo, SEEK_SET);
    atual = le_registro_bin(dados); //Neste ponto, 'atual' contém o primeiro registro da lista.
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

    //Percorre a lista até encontrar o local apropriado para a inserção do registro ou até o fim dela.
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


//Função auxiliar que realiza a comparação entre os campos buscados e os campos de um registro. Retorna 1 se o registro atende às buscas, e 0 caso contrário.
int compara(REGISTRO reg, struct como_busca busca){

    int camposIguais = 0; //Variável que armazena quantos campos do registro são iguais aos campos buscados até o momento.

    if(busca.busca_id == 1){ //Verifica se a busca por 'id' deve ser feita.
        if(reg.id == busca.id) //Se sim, realiza a comparação.
            camposIguais++; //Se passar na busca, incrementa 'camposIguais'.
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

    if(camposIguais == busca.m){ //Se o registro atende a todas as buscas por campo feitas, então camposIguais == m (sendo m o número de campos que estão sendo usados na busca).
        return 1;
    }

    return 0;
}


//Funcionalidade 5: realiza a remoção de registros por qualquer campo.
int funcao5(FILE *dados, char *nome_arq_indice, int n){

    FILE *arq_indice = fopen(nome_arq_indice, "wb");
    if(arq_indice == NULL)
        return 0;

    //Criação do arquivo de índice utilizando a funcionalidade 4.
    if(funcao4(dados, arq_indice) == 0) //Verifica se a criação do índice foi bem sucedida.
        return 0;

    fclose(arq_indice);
    arq_indice = fopen(nome_arq_indice, "rb");

    rewind(dados);
    CABECALHO cab = le_cabecalho(dados);


    //O arquivo de dados será modificado, logo 'status' recebe o valor 0 e é escrito no arquivo.
    (cab.status)[0] = '0';
    rewind(dados);
    escreve_cabecalho(dados, cab);

    //Carregamento do índice para a memória primária.
    REG_INDICE *indice = carrega_indice(arq_indice, cab.nroRegArq);
    if(indice == NULL)
        return 0;


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


    long int byteOffset; //Variável que armazenará o byte offset do registro lido no momento.

    for(int i = 0; i < n; i++){ //Loop para as n buscas.

        byteOffset = 25; //O byte offset do primeiro registro de dados é 25, logo após o registro de cabeçalho.

        if((buscas[i]).busca_id == 1){ //Verifica se a busca por 'id' deve ser feita.

            byteOffset = busca_id_indice(indice, cab.nroRegArq, (buscas[i]).id); //Realiza a busca do id no índice.

            if(byteOffset == -1){ //Verifica se o id buscado foi encontrado no índice.
                //Se não foi encontrado, esta busca não foi bem sucedida e pode-se passar para a próxima.
                fseek(dados, 25, SEEK_SET); //Posiciona o cursor no primeiro registro de dados, preparando para a próxima busca.
                continue; //Passa para a próxima busca.
            }

            //Se o id foi encontrado, lê o registro correspondente.
            fseek(dados, byteOffset, SEEK_SET);
            reg = le_registro_bin(dados);

            //Realiza a comparação dos campos buscados com os campos do registro encontrado.
            if(compara(reg, buscas[i])){ //Se o registro atende a todas as buscas por campo feitas, então 'compara' retorna 1. Nesse caso, o registro deve ser removido.

                //Altera o campo 'removido' para 1 e o insere na lista de removidos (o registro com as alterações nos campos 'removido' e 'prox' são escritos no arquivo pela função 'insere_listaRemovidos').
                (reg.removido)[0] = '1';
                insere_listaRemovidos(dados, byteOffset, reg);

                //Reflete a remoção no índice.
                remove_registro_indice(indice, cab.nroRegArq, reg.id);

                cab.nroRegArq--;
                cab.nroRegRem++;
            }

            //Liberação da memória alocada para o registro lido.
            libera_registro(reg);
        }
        else{ //Caso em que o id não é usado na busca.
            for(int j = 0; j < cab.nroRegArq + cab.nroRegRem; j++){ //Loop para percorrer o arquivo binário inteiro.

                //Lê um registro do arquivo de dados.
                reg = le_registro_bin(dados); 

                if((reg.removido)[0] == '1'){ //Se o registro estiver marcado como removido, pula-se este registro.
                    byteOffset += (long int) reg.tamanhoRegistro;
                    libera_registro(reg);

                    continue;
                }

                if(compara(reg, buscas[i])){ //Se o registro atende a todas as buscas por campo feitas, então 'compara' retorna 1. Nesse caso, o registro deve ser removido.

                    //Realiza a mudança nos campos 'removido' e 'prox', e o insere na lista de removidos.
                    (reg.removido)[0] = '1';
                    insere_listaRemovidos(dados, byteOffset, reg);

                    //Reflete a remoção no índice.
                    remove_registro_indice(indice, cab.nroRegArq, reg.id);

                    cab.nroRegArq--;
                    cab.nroRegRem++;
                }

                //Atualiza 'byteOffset' para o byte offset do próximo registro a ser lido.
                byteOffset += (long int) reg.tamanhoRegistro;
                
                //Liberação da memória alocada para o registro.
                libera_registro(reg);

                fseek(dados, byteOffset, SEEK_SET);
            }
        }
        //Volta ao primeiro registro de dados, preparando para a próxima busca.
        fseek(dados, 25, SEEK_SET);
    }

    //As mudanças no arquivo de dados acabaram, logo 'status' recebe 1 e o registro de cabeçalho com as mudanças é escrito no arquivo.
    rewind(dados);
    (cab.status)[0] = '1';
    fwrite(cab.status, 1, 1, dados);

    //Processo de reescrita do arquivo de índice.

    //O arquivo é fechado e aberto em modo de escrita. Dessa forma, o conteúdo anterior é apagado. Como o tamanho do novo arquivo de índice é menor ou igual ao anterior, garante-se que não fiquem
    //dados antigos no fim do arquivo.
    fclose(arq_indice);
    arq_indice = fopen(nome_arq_indice, "wb");
    
    if(arq_indice == NULL)
        return 0;

    reescrita_indice(arq_indice, indice, cab.nroRegArq);

    //As mudanças no índice acabaram, logo 'status' recebe 1.
    char status[1];
    status[0] = '1';
    rewind(arq_indice);
    fwrite(status, 1, 1, arq_indice);

    fclose(arq_indice);

    //Liberação da memória alocada para o índice.
    free(indice);

    return 1;
}

//Função que lê a entrada e retorna um vetor com as structs que contêm os registros a serem inseridos.
REGISTRO* le_entrada_funcao6(int n){
    REGISTRO *registros = malloc(n*sizeof(REGISTRO)); //Vetor de registros a serem inseridos.
    
    if(registros == NULL)
        return NULL;

    char valorCampo[100];
    for(int i = 0; i < n; i++){

        //Inicialização padrão de alguns campos.
        ((registros[i]).removido)[0] = '0';
        (registros[i]).tamanhoRegistro = 33;
        (registros[i]).prox = -1;

        //Leitura dos valores dos campos, com tratamento de campos nulos.
        scanf(" %s", valorCampo);
        if(strcmp(valorCampo, "NULO") == 0)
            (registros[i]).id = -1;
        else
            (registros[i]).id = atoi(valorCampo);

        scanf(" %s", valorCampo);
        if(strcmp(valorCampo, "NULO") == 0)
            (registros[i]).idade = -1;
        else
            (registros[i]).idade = atoi(valorCampo);

        scan_quote_string(valorCampo);// leitura sem aspas das entradas.
        (registros[i]).tamNomeJogador = strlen(valorCampo);
        (registros[i]).nomeJogador = (char *) malloc((registros[i]).tamNomeJogador*sizeof(char) + 1);

        if((registros[i]).nomeJogador == NULL)
            return NULL;

        strcpy((registros[i]).nomeJogador, valorCampo);

        scan_quote_string(valorCampo);
        (registros[i]).tamNacionalidade = strlen(valorCampo);
        (registros[i]).Nacionalidade = (char *) malloc((registros[i]).tamNacionalidade*sizeof(char) + 1);

        if((registros[i]).Nacionalidade == NULL)
            return NULL;

        strcpy((registros[i]).Nacionalidade, valorCampo);

        scan_quote_string(valorCampo);
        (registros[i]).tamNomeClube = strlen(valorCampo);
        (registros[i]).nomeClube = (char *) malloc((registros[i]).tamNomeClube*sizeof(char) + 1);

        if((registros[i]).nomeClube == NULL)
            return NULL;

        strcpy((registros[i]).nomeClube, valorCampo);

        (registros[i]).tamanhoRegistro += (registros[i]).tamNomeJogador + (registros[i]).tamNacionalidade + (registros[i]).tamNomeClube;
    }

    return registros;
}


//Funcionalidade 6: inserção de novos registros com reaproveitamento de espaço, utilizando a estratégia Best Fit.
int funcao6(FILE *dados, char *nome_arq_indice, int nroInsercoes){
    
    FILE *arq_indice = fopen(nome_arq_indice, "wb");

    //Criação do arquivo de índice utilizando a funcionalidade 4.
    if(funcao4(dados, arq_indice) == 0) //Verifica se a criação do índice foi bem sucedida.
        return 0;

    fclose(arq_indice);
    arq_indice = fopen(nome_arq_indice, "rb+");

    rewind(dados);
	CABECALHO cab = le_cabecalho(dados);

    char status_indice[1];
    rewind(arq_indice);
    fread(status_indice, 1, 1, arq_indice);

    //Verificação da consistência dos dados dos arquivos.
	if((cab.status)[0] == '0' || status_indice[0] == '0')
		return 0;

    status_indice[0] = '0';
    (cab.status)[0] = '0';

    //Os arquivos de dados e de índice sofrerão alterações. Logo, 'status' recebe 0 durante a execução da funcionalidade.
    rewind(dados);
    escreve_cabecalho(dados, cab);
    rewind(arq_indice);
    fwrite(status_indice, 1, 1, arq_indice);

    //Carregamento do índice para a memória primária.
    REG_INDICE *indice = carrega_indice(arq_indice, cab.nroRegArq);

    //Leitura da entrada.
    REGISTRO *insercoes = le_entrada_funcao6(nroInsercoes);

    if(insercoes == NULL || indice == NULL)
        return 0;


    //Vetor que armazenará os registros a serem inseridos no índice.
    REG_INDICE *insercoes_indice = (REG_INDICE *) malloc(nroInsercoes*sizeof(REG_INDICE));
    if(insercoes_indice == NULL)
        return 0;

    long int byteOffset_atual, byteOffset_anterior;
    REGISTRO removidoAtual, removidoAnterior;
    int i = 0;
    char lixo = '$';
    int tamanho_lixo; //Variável que contém o número de bytes no fim do registro inserido que deve receber o caractere '$'.
    
    byteOffset_atual = cab.topo;

    for(i = 0; i < nroInsercoes; i++){ //Loop para as n inserções.
        byteOffset_atual = cab.topo;

        (insercoes_indice[i]).id = (insercoes[i]).id; //Montagem do vetor de registros para inserção no índice.
        
        while(byteOffset_atual != -1){ //Loop para percorrer a lista de removidos, procurando o lugar apropriado para inserir o novo registro.

            fseek(dados, byteOffset_atual, SEEK_SET);
            removidoAtual = le_registro_bin(dados);

            if((insercoes[i]).tamanhoRegistro <= removidoAtual.tamanhoRegistro){ //Testa se o novo registro cabe no registro atual da lista de removidos.
                //Se couber, escreve-o sobre o registro removido.

                (insercoes_indice[i]).byteOffset = byteOffset_atual;

                tamanho_lixo = removidoAtual.tamanhoRegistro - (insercoes[i]).tamanhoRegistro; //Calcula o número de '$' a ser acrescentado ao final do registro inserido.


                (insercoes[i]).tamanhoRegistro = removidoAtual.tamanhoRegistro;

                fseek(dados, byteOffset_atual, SEEK_SET);
                escreve_registro(dados, insercoes[i]);
                for(int i = 0; i < tamanho_lixo; i++) //Preenche o registro com '$'
                    fwrite(&lixo, 1, 1, dados);

                if(cab.topo == byteOffset_atual){ //Testa se o registro que foi sobrescrito era o primeiro da lista. Se for, 'topo' do cabeçalho deve ser atualizado.
                    cab.topo = removidoAtual.prox;
                }
                else{ //Atualiza o campo 'prox' do registro imediatamente anterior ao registro sobrescrito na lista de removidos.
                    removidoAnterior.prox = removidoAtual.prox;

                    fseek(dados, byteOffset_anterior, SEEK_SET);
                    escreve_registro(dados, removidoAnterior);

                    libera_registro(removidoAnterior);
                }

                cab.nroRegRem--;

                libera_registro(removidoAtual);

                break;
            }

            if(byteOffset_atual != cab.topo) //Testa se 'byteOffset_atual' não é o byte offset do primeiro registro da lista. Se não for, 'removidoAnterior' contém memória alocada que precisa ser liberada.
                libera_registro(removidoAnterior);

            //"Avança" 'removidoAnterior' para 'removidoAtual', preparando para ler o próximo registro do arquivo.
            removidoAnterior = removidoAtual;

            byteOffset_anterior = byteOffset_atual;
            byteOffset_atual = removidoAtual.prox;

        }

        //Inserção no final do arquivo.
        if(byteOffset_atual == -1){
            byteOffset_atual = cab.proxByteOffset;
            
            (insercoes_indice[i]).byteOffset = byteOffset_atual;

            fseek(dados, byteOffset_atual, SEEK_SET);

            escreve_registro(dados, insercoes[i]);

            cab.proxByteOffset = byteOffset_atual + (insercoes[i]).tamanhoRegistro;
        }

        libera_registro(insercoes[i]);
    }

    //Atualiza o arquivo de índice, inserindo os novos registros.
    insere_registros_indice(arq_indice, indice, cab.nroRegArq, insercoes_indice, nroInsercoes);

    cab.nroRegArq += nroInsercoes;

    (cab.status)[0] = '1';
    rewind(dados);
    escreve_cabecalho(dados, cab);

    status_indice[0] = '1';
    rewind(arq_indice);
    fwrite(status_indice, 1, 1, arq_indice);

    free(indice);
    free(insercoes);
    free(insercoes_indice);

    fclose(arq_indice);

    return 1;
}