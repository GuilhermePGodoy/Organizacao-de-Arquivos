/* Funções para a manipulação (leitura e escrita) dos registros de dados em arquivos .csv e .bin */

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

struct registro le_registro_csv(*FILE f){
	
}