#include "registros.h"
#include "funcoes_fornecidas.h"

//Funcionalidade 1: escreve os registros do arquivo de entrada 'incsv' no arquivo binário de saída 'outbin'.
void funcao1(FILE *incsv, FILE *outbin)
{
    CABECALHO cab;
    REGISTRO reg;
    char controle; //Variável auxiliar para monitorar o fim do arquivo.


    //Inicialização de campos do registro de cabeçalho.
    (cab.status)[0] = '0';
    cab.topo = -1;
    cab.proxByteOffset = 25;
    cab.nroRegArq = 0;
    cab.nroRegRem = 0;

    fseek(outbin, 25, SEEK_CUR); //Reserva espaço para o registro de cabeçalho.

    fseek(incsv, 45, SEEK_CUR); //Pula a primeira linha.


    while((controle = getc(incsv)) != EOF){ //Tenta ler o próximo caractere com 'getc.'. Se não houver, 'getc' retorna  EOF, indicando que o arquivo acabou.
        fseek(incsv, -1, SEEK_CUR); //Se o arquivo não acabou, volta à posição lida por 'getc()'.

        reg = le_registro_csv(incsv);

        cab.nroRegArq++; //A cada registro lido do .csv, incrementa o contador de registros.

        //Próximo byte offset livre é logo após o último registro gravado.
        cab.proxByteOffset += reg.tamanhoRegistro;

        escreve_registro(outbin, reg); //Escreve o registro no arquivo .bin.
    }


    rewind(outbin); //Volta ao início do arquivo para escrever o registro de cabeçalho.

    //Escreve no arquivo .bin o registro de cabeçalho campo a campo.
    (cab.status)[0] = '1';
    fwrite(cab.status, 1, 1, outbin);
    fwrite(&(cab.topo), 8, 1, outbin);
    fwrite(&(cab.proxByteOffset), 8, 1, outbin);
    fwrite(&(cab.nroRegArq), 4, 1, outbin);
    fwrite(&(cab.nroRegRem), 4, 1, outbin);
}

//Funcionalidade 2: recupera os dados de todos os registros de um arquivo binário.
void funcao2(FILE *f){
    
    int nroRegArq;
    fseek(f, 17, SEEK_CUR); //Pula para o campo 'nroRegArq' do registro de cabeçalho.
    fread(&nroRegArq, 4, 1, f);
    if(nroRegArq == 0){ //Verifica se não há nenhum registro no arquivo.
        printf("Registro inexistente.\n\n");
        return;
    }

    fseek(f, 4, SEEK_CUR); //Pula o campo 'nroRegRem' do registro de cabeçalho.

    REGISTRO reg;
    char controle; //Variável auxiliar para monitorar o fim do arquivo.

    while((controle = getc(f)) != EOF){  //Loop para percorrer o arquivo todo. Se estiver no fim, getc retorna EOF.
        fseek(f, -1, SEEK_CUR); //Se não estiver no fim, volta à posição lida por 'getc()'.

        reg = le_registro_bin(f); //Lê um registro do arquivo.

        if((reg.removido)[0] != '1'){ //Se o registro não estiver marcado como removido, imprime seus dados.
            printf("Nome do Jogador: ");
            if(reg.tamNomeJogador == 0) //Se o campo é vazio, imprime "SEM DADO".
                printf("SEM DADO\n");
            else{
                printf("%s\n", reg.nomeJogador);
            }
           free(reg.nomeJogador);

            printf("Nacionalidade do Jogador: ");
            if(reg.tamNacionalidade == 0)
                printf("SEM DADO\n");
            else{
                printf("%s\n", reg.Nacionalidade);
            }
            free(reg.Nacionalidade);

            printf("Clube do Jogador: ");
            if(reg.tamNomeClube == 0)
                printf("SEM DADO\n\n");
            else{
                printf("%s\n\n", reg.nomeClube);
            }
            free(reg.nomeClube);
        }
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

    int deucerto;  //É diferente de 0 se algum registro do arquivo atende à busca realizada, e 0 caso contrário. 
};

//Funcionalidade 3: realiza n buscas, cada uma por m campos, em um arquivo binário.
void funcao3(FILE *f, int n){
    struct como_busca buscas[n]; //Vetor das structs com as informações das n buscas a serem realizadas.
    char nomeCampo[20]; //Vetor auxiliar para ler o nome do campo que deve ser usado na busca.
    REGISTRO reg; //Struct que receberá os registros do arquivo.


    for(int i = 0; i < n; i++){ //Leitura da entrada com as especificações das buscas.
        buscas[i].deucerto = 0; 

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

    for(int i = 0; i < n; i++){ //Loop para as n buscas.
        fseek(f, 25, SEEK_SET); //Pula o registro de cabeçalho.
        flag = 0;

        printf("Busca %d\n\n", i + 1);

        while((controle = getc(f)) != EOF){ //Loop para percorrer o arquivo binário inteiro.
            fseek(f, -1, SEEK_CUR); //Se não está no fim do arquivo, volta à posição lida por 'getc()'.

            cont = 0;


            reg = le_registro_bin(f); 


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
                (buscas[i]).deucerto++; //Um registro atende às especificações da busca. Então, 'deucerto' é incrementado.

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
            }

            free(reg.nomeJogador);
            free(reg.Nacionalidade);
            free(reg.nomeClube);

            if(flag == 1)  //Se 'flag' == 1, encerra a busca.
                break;
        }

        if(buscas[i].deucerto == 0) //Se 'deucerto' == 0, nenhum registro atende às especificações da busca.
            printf("Registro inexistente.\n\n");
    }

    return;
}