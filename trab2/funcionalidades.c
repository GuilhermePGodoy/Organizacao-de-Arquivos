#include "arvoreb.h"
#include "registros.h"
#include "funcionalidades.h"

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

    int deucerto;  //É diferente de 0 se algum registro do arquivo atende à busca realizada, e 0 caso contrário. 
};

//Funcionalidade 7: criação de uma árvore B que indexa o arquivo de dados fornecido.
int funcao7(FILE *dados, FILE *arvB){

    CABECALHO_ARV_B cabecalho; //Struct para armazenar os campos do cabeçalho da árvore B.
    CABECALHO_DADOS cab = le_cabecalho_dados(dados); //Struct para armazenar os campos do registro de cabeçalho do arquivo de dados.
    REGISTRO_DADOS reg; //Struct para armazenar os campos de um registro do arquivo de dados.

    //Inicialização do cabeçalho do índice.
    cabecalho.status[0] = '1'; //Status 0 pois haverá escrita no arquivo de índice.
    cabecalho.noRaiz = -1;
    cabecalho.proxRRN = 0;
    cabecalho.nroChaves = 0;
    escreve_cabecalho_arvB(arvB, cabecalho);

	//Verifica se o arquivo de dados está consistente.
	if((cab.status)[0] == '0'){
		return 0;
	}

	long int byteOffset = 25; //Variável que armazena o byteOffset do registro que está sendo tratado no momento.

    for(int i = 0; i < cab.nroRegArq + cab.nroRegRem; i++){ //Loop para percorrer o arquivo de dados inteiro.
		reg = le_registro_dados(dados); //Lê o próximo registro do arquivo.

		//Se o registro não estiver marcado como removido, insere-o no índice.
		if((reg.removido)[0] == '0'){
            insere_arvoreB(arvB, reg.id, byteOffset);
        }

        byteOffset += reg.tamanhoRegistro; //Atualiza 'byteOffset' para o byte offset do próximo registro do arquivo.

        libera_registro_dados(reg); //Libera a memória alocada para o registro.

	}

    return 1;
}


int funcao8(FILE *dados, FILE *arvB, int n){

    if(dados == NULL || arvB == NULL)
        return 0;

    CABECALHO_ARV_B cabArvB = le_cabecalho_arvB(arvB);
    CABECALHO_DADOS cabDados = le_cabecalho_dados(dados);

    //Verifica a consistência dos dados dos arquivos de dados e de índice.
    if(cabArvB.status[0] == '0' || cabDados.status[0] == '0'){
        return 0;
    }

    int nbusca; //numero da busca atual
    struct como_busca buscas[n]; //Vetor das structs com as informações das n buscas a serem realizadas.
    char nomeCampo[50]; //Vetor auxiliar para ler o nome do campo que deve ser usado na busca.
    REGISTRO_DADOS reg; //Struct que receberá os registros do arquivo.

    long int byteOffset; //Variável que armazenará o byte offset do registro lido no momento.

    //Inicialização dos campos das structs do vetor 'buscas'.
    for(int i = 0; i < n; i++){
        (buscas[i]).busca_id = 0;
    }

    for(int i = 0; i < n; i++){ //Leitura da entrada com as especificações das buscas.

        scanf(" %d", &nbusca);

        scanf(" %s", nomeCampo);

        scanf(" %d", &((buscas[i]).id));       
        
        printf("BUSCA %d\n\n", i + 1);

        byteOffset = busca_id_arvB(arvB, (buscas[i]).id);

        if(byteOffset == -1){
        	printf("Registro inexistente.\n\n");
        }
        else{
        	fseek(dados, byteOffset, SEEK_SET);
        	reg = le_registro_dados(dados);

        	//Imprime os dados do registro que passou na busca.
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

            libera_registro_dados(reg);
        }
    }

    return 1;
}


int funcao9(FILE *dados, FILE *arvB, int n)
{
    if (dados == NULL || arvB == NULL)
        return 0;

    CABECALHO_ARV_B cabArvB = le_cabecalho_arvB(arvB);
    CABECALHO_DADOS cabDados = le_cabecalho_dados(dados);

    if (cabArvB.status[0] == '0' || cabDados.status[0] == '0')
    {
        return 0;
    }

    struct como_busca buscas[n]; // Vetor das structs com as informações das n buscas a serem realizadas.
    char nomeCampo[50];          // Vetor auxiliar para ler o nome do campo que deve ser usado na busca.
    REGISTRO_DADOS reg;          // Struct que receberá os registros do arquivo.

    long int byteOffset; // Variável que armazenará o byte offset do registro lido no momento.

    // Inicialização dos campos das structs do vetor 'buscas'.
    for (int i = 0; i < n; i++)
    {
        (buscas[i]).busca_id = 0;
        (buscas[i]).busca_idade = 0;
        (buscas[i]).busca_nome = 0;
        (buscas[i]).busca_nacionalidade = 0;
        (buscas[i]).busca_clube = 0;
    }

    for (int i = 0; i < n; i++)
    { // Leitura da entrada com as especificações das buscas.
    	(buscas[i]).deucerto = 0;
        scanf(" %d", &((buscas[i]).m));

        for (int j = 0; j < (buscas[i]).m; j++)
        {

            scanf(" %s", nomeCampo);

            if (strcmp(nomeCampo, "id") == 0)
            {
                (buscas[i]).busca_id = 1;

                scanf(" %d", &((buscas[i]).id));

            }

            else if (strcmp(nomeCampo, "idade") == 0)
            {
                (buscas[i]).busca_idade = 1;

                scanf(" %d", &((buscas[i]).idade));
            }

            else if (strcmp(nomeCampo, "nomeJogador") == 0)
            {
                (buscas[i]).busca_nome = 1;

                scan_quote_string((buscas[i]).nome);
            }

            else if (strcmp(nomeCampo, "nacionalidade") == 0)
            {
                (buscas[i]).busca_nacionalidade = 1;

                scan_quote_string((buscas[i]).nacionalidade);
            }

            else if (strcmp(nomeCampo, "nomeClube") == 0)
            {
                (buscas[i]).busca_clube = 1;

                scan_quote_string((buscas[i]).clube);
            }
        }
    }

    int cont = 0;  // Variável que armazena o número de campos do registro atual que passam na busca (ou seja, são iguais aos buscados).
    int flag = 0;  // Flag utilizada para indicar que a busca deve parar. Utilizada para buscas por id, visto que este campo é único para cada jogador.

    for (int i = 0; i < n; i++)
    {                           // Loop para as n buscas.
        fseek(dados, 25, SEEK_SET); // Pula o registro de cabeçalho.
        flag = 0;

        printf("Busca %d\n\n", i + 1);

            if ((buscas[i]).busca_id == 1){ // Verifica se deve fazer a busca pelo campo 'id'.
            	//Se sim, utiliza o índice para buscar.
        		byteOffset = busca_id_arvB(arvB, (buscas[i]).id);

        		if(byteOffset == -1){ //Se byteOffset == -1, o id não foi encontrado.
        			printf("Registro inexistente.\n\n");

        			continue; //O id não foi encontrado. Portanto, esta busca acabou e pode-se seguir para a próxima.
        		}

        		//Se o id foi encontrado, acessa o registro correspondente e o lê.
        		fseek(dados, byteOffset, SEEK_SET); 

        		reg = le_registro_dados(dados);

                // Imprime os dados do registro que passou na busca.
                if (reg.tamNomeJogador != 0)
                    printf("Nome do Jogador: %s\n", reg.nomeJogador);
                else
                    printf("Nome do Jogador: SEM DADO\n");

                if (reg.tamNacionalidade != 0)
                    printf("Nacionalidade do Jogador: %s\n", reg.Nacionalidade);
                else
                    printf("Nacionalidade do Jogador: SEM DADO\n");

                if (reg.tamNomeClube != 0)
                    printf("Clube do Jogador: %s\n\n", reg.nomeClube);
                else
                    printf("Clube do Jogador: SEM DADO\n\n");

                libera_registro_dados(reg);
                continue; //Segue para a próxima busca.
            }

     	//Caso de busca sem considerar id.
        for(int j = 0; j < cabDados.nroRegArq + cabDados.nroRegRem; j++){ //Loop para percorrer o arquivo binário inteiro.
            cont = 0;

            reg = le_registro_dados(dados);

            if ((reg.removido)[0] == '1')
            { // Se o registro estiver marcado como removido, pula-se este registro.
            	libera_registro_dados(reg);
                continue;
            }

            if ((buscas[i]).busca_idade == 1)
            {                                       // Verifica se a busca por 'idade' deve ser feita.
                if (reg.idade == (buscas[i]).idade) // Se sim, realiza a comparação.
                    cont++;                         // Se passar na busca, incrementa 'cont'
            }

            if ((buscas[i]).busca_nome == 1)
            {                                                       // Verifica se a busca por 'nomeJogador' deve ser feita.
                if (strcmp(reg.nomeJogador, (buscas[i]).nome) == 0) // Se sim, realiza a comparação.
                    cont++;                                         // Se passar na busca, incrementa 'cont'
            }

            if ((buscas[i]).busca_nacionalidade == 1)
            {                                                                  // Verifica se a busca por 'Nacionalidade' deve ser feita.
                if (strcmp(reg.Nacionalidade, (buscas[i]).nacionalidade) == 0) // Se sim, realiza a comparação.
                    cont++;                                                    // Se passar na busca, incrementa 'cont'
            }
            if ((buscas[i]).busca_clube == 1)
            {                                                      // Verifica se a busca por 'nomeClube' deve ser feita.
                if (strcmp(reg.nomeClube, (buscas[i]).clube) == 0) // Se sim, realiza a comparação.
                    cont++;                                        // Se passar na busca, incrementa 'cont'
            }

            if (cont == (buscas[i]).m)
            {                           // Se o registro atende a todas as buscas por campo feitas, então cont == m.
                (buscas[i]).deucerto++; // Um registro atende às especificações da busca. Então, 'deucerto' é incrementado.

                // Imprime os dados do registro que passou na busca.
                if (reg.tamNomeJogador != 0)
                    printf("Nome do Jogador: %s\n", reg.nomeJogador);
                else
                    printf("Nome do Jogador: SEM DADO\n");

                if (reg.tamNacionalidade != 0)
                    printf("Nacionalidade do Jogador: %s\n", reg.Nacionalidade);
                else
                    printf("Nacionalidade do Jogador: SEM DADO\n");

                if (reg.tamNomeClube != 0)
                    printf("Clube do Jogador: %s\n\n", reg.nomeClube);
                else
                    printf("Clube do Jogador: SEM DADO\n\n");
            }

            libera_registro_dados(reg);

            if (flag == 1) // Se 'flag' == 1, encerra a busca.
                break;
        }

        if (buscas[i].deucerto == 0) // Se 'deucerto' == 0, nenhum registro atende às especificações da busca.
            printf("Registro inexistente.\n\n");
    }

    return 1;
}

// Função que lê a entrada e retorna um vetor com as structs que contêm os registros a serem inseridos.
REGISTRO_DADOS *le_entrada_funcao10(int n)
{
    REGISTRO_DADOS *registros = malloc(n * sizeof(REGISTRO_DADOS)); // Vetor de registros a serem inseridos.

    if (registros == NULL)
        return NULL;

    char valorCampo[100];
    for (int i = 0; i < n; i++)
    {

        // Inicialização padrão de alguns campos.
        ((registros[i]).removido)[0] = '0';
        (registros[i]).tamanhoRegistro = 33;
        (registros[i]).prox = -1;

        // Leitura dos valores dos campos, com tratamento de campos nulos.
        scanf(" %s", valorCampo);
        if (strcmp(valorCampo, "NULO") == 0)
            (registros[i]).id = -1;
        else
            (registros[i]).id = atoi(valorCampo);

        scanf(" %s", valorCampo);
        if (strcmp(valorCampo, "NULO") == 0)
            (registros[i]).idade = -1;
        else
            (registros[i]).idade = atoi(valorCampo);

        scan_quote_string(valorCampo); // leitura sem aspas das entradas.
        (registros[i]).tamNomeJogador = strlen(valorCampo);
        (registros[i]).nomeJogador = (char *)malloc((registros[i]).tamNomeJogador * sizeof(char) + 1);

        if ((registros[i]).nomeJogador == NULL)
            return NULL;

        strcpy((registros[i]).nomeJogador, valorCampo);

        scan_quote_string(valorCampo);
        (registros[i]).tamNacionalidade = strlen(valorCampo);
        (registros[i]).Nacionalidade = (char *)malloc((registros[i]).tamNacionalidade * sizeof(char) + 1);

        if ((registros[i]).Nacionalidade == NULL)
            return NULL;

        strcpy((registros[i]).Nacionalidade, valorCampo);

        scan_quote_string(valorCampo);
        (registros[i]).tamNomeClube = strlen(valorCampo);
        (registros[i]).nomeClube = (char *)malloc((registros[i]).tamNomeClube * sizeof(char) + 1);

        if ((registros[i]).nomeClube == NULL)
            return NULL;

        strcpy((registros[i]).nomeClube, valorCampo);

        (registros[i]).tamanhoRegistro += (registros[i]).tamNomeJogador + (registros[i]).tamNacionalidade + (registros[i]).tamNomeClube;
    }

    return registros;
}

//Funcionalidade 10: inserção de novos registros com reaproveitamento de espaço, utilizando a estratégia Best Fit.
int funcao10(FILE *dados, FILE *arvB, int nroInsercoes){

	if(dados == NULL || arvB == NULL)
		return 0;

    rewind(dados);
	CABECALHO_DADOS cabDados = le_cabecalho_dados(dados);
	CABECALHO_ARV_B cabArvB = le_cabecalho_arvB(arvB);

    //Verificação da consistência dos dados dos arquivos.
	if((cabDados.status)[0] == '0' || (cabArvB.status)[0] == '0')
		return 0;

    (cabDados.status)[0] = '0';

    //Os arquivos de dados e de índice sofrerão alterações. Logo, 'status' recebe 0 durante a execução da funcionalidade.
    rewind(dados);
    escreve_cabecalho_dados(dados, cabDados);

    //Leitura da entrada.
    REGISTRO_DADOS *insercoes = le_entrada_funcao10(nroInsercoes);

    if(insercoes == NULL)
        return 0;

    long int byteOffset_atual, byteOffset_anterior;
    long int estaNaArvore;
    REGISTRO_DADOS removidoAtual, removidoAnterior;
    int i = 0;
    char lixo = '$';
    int tamanho_lixo; //Variável que contém o número de bytes no fim do registro inserido que deve receber o caractere '$'.

    byteOffset_atual = cabDados.topo;

    for(i = 0; i < nroInsercoes; i++){ //Loop para as n inserções.
        byteOffset_atual = cabDados.topo;

        //Realiza uma busca pelo id do jogador a ser inserido, para garantir que não haverá id's repetidos no arquivo de dados.
        estaNaArvore = busca_id_arvB(arvB, (insercoes[i]).id);

        if(estaNaArvore != -1) //Se 'busca_id_arvB' for diferente de -1, significa que o id já existe na árvore.
        	continue;

        while(byteOffset_atual != -1){ //Loop para percorrer a lista de removidos, procurando o lugar apropriado para inserir o novo registro.

            fseek(dados, byteOffset_atual, SEEK_SET);
            removidoAtual = le_registro_dados(dados);

            if((insercoes[i]).tamanhoRegistro <= removidoAtual.tamanhoRegistro){ //Testa se o novo registro cabe no registro atual da lista de removidos.
                //Se couber, escreve-o sobre o registro removido.

            	insere_arvoreB(arvB, (insercoes[i]).id, byteOffset_atual);

                tamanho_lixo = removidoAtual.tamanhoRegistro - (insercoes[i]).tamanhoRegistro; //Calcula o número de '$' a ser acrescentado ao final do registro inserido.

                (insercoes[i]).tamanhoRegistro = removidoAtual.tamanhoRegistro;

                fseek(dados, byteOffset_atual, SEEK_SET);
                escreve_registro_dados(dados, insercoes[i]);
                for(int i = 0; i < tamanho_lixo; i++) //Preenche o registro com '$'
                    fwrite(&lixo, 1, 1, dados);

                if(cabDados.topo == byteOffset_atual){ //Testa se o registro que foi sobrescrito era o primeiro da lista. Se for, 'topo' do cabeçalho deve ser atualizado.
                    cabDados.topo = removidoAtual.prox;
                }
                else{ //Atualiza o campo 'prox' do registro imediatamente anterior ao registro sobrescrito na lista de removidos.
                    removidoAnterior.prox = removidoAtual.prox;

                    fseek(dados, byteOffset_anterior, SEEK_SET);
                    escreve_registro_dados(dados, removidoAnterior);

                    libera_registro_dados(removidoAnterior);
                }

                cabDados.nroRegRem--;

                libera_registro_dados(removidoAtual);

                break;
            }

            if(byteOffset_atual != cabDados.topo) //Testa se 'byteOffset_atual' não é o byte offset do primeiro registro da lista. Se não for, 'removidoAnterior' contém memória alocada que precisa ser liberada.
                libera_registro_dados(removidoAnterior);

            //"Avança" 'removidoAnterior' para 'removidoAtual', preparando para ler o próximo registro do arquivo.
            removidoAnterior = removidoAtual;

            byteOffset_anterior = byteOffset_atual;
            byteOffset_atual = removidoAtual.prox;

        }

        //Inserção no final do arquivo.
        if(byteOffset_atual == -1){
            byteOffset_atual = cabDados.proxByteOffset;

			insere_arvoreB(arvB, (insercoes[i]).id, byteOffset_atual);            

            fseek(dados, byteOffset_atual, SEEK_SET);

            escreve_registro_dados(dados, insercoes[i]);

            cabDados.proxByteOffset = byteOffset_atual + (insercoes[i]).tamanhoRegistro;
        }

        libera_registro_dados(insercoes[i]);
    }

    cabDados.nroRegArq += nroInsercoes;

    (cabDados.status)[0] = '1';
    rewind(dados);
    escreve_cabecalho_dados(dados, cabDados);

    free(insercoes);

    return 1;
}