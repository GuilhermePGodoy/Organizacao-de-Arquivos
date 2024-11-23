#include "arvoreb.h"

//Funções auxiliares para imprimir as páginas presentes na árvore B.

//Imprime todas as páginas da árvore.
void imprime_arvB_rec(FILE *arvB, int RRN){
	if(RRN == -1)
		return;

	fseek(arvB, (RRN + 1)*TAM_PAGINA, SEEK_SET);

	PAGINA pag = le_pagina_arvB(arvB, RRN);

	imprimePagina(pag);

	for(int i = 0; i < ORDEM; i++)
		imprime_arvB_rec(arvB, pag.ponteiros[i]);
}

void imprime_arvB(FILE *arvB){
	if(arvB == NULL)
		return;

	CABECALHO_ARV_B cab;

	cab = le_cabecalho_arvB(arvB);

	imprimeCab(cab);

	if(cab.noRaiz == -1){
		printf("Árvore vazia.\n");
		return;
	}

	imprime_arvB_rec(arvB, cab.noRaiz);
}

//Imprime o cabeçalho.
void imprimeCab(CABECALHO_ARV_B cab){
	printf("----CABEÇALHO----\n");

	printf("Status: %c\n", cab.status[0]);
	printf("noRaiz: %d\n", cab.noRaiz);
	printf("proxRRN: %d\n", cab.proxRRN);
	printf("nroChaves: %d\n--------\n", cab.nroChaves);
}

//Imprime uma página.
void imprimePagina(PAGINA pag){
	printf("----PAGINA----\n");

	printf("alturaNo: %d\n", pag.alturaNo);
	printf("nroChaves: %d\n", pag.nroChaves);

	printf("\nCHAVES:\n");
	for(int i = 0; i < ORDEM - 1; i++){
		printf("id: %d, byteOffset: %ld\n", (pag.chaves[i]).id, (pag.chaves[i]).byteOffset);
	}

	printf("\nPONTEIROS:\n");
	for(int i = 0; i < ORDEM; i++){
		printf("%d\n", pag.ponteiros[i]);
	}

	printf("--------\n\n");
}


//Retorna uma página com os campos inicializados com valores padrão.
PAGINA inicializaPagina(){
	//AlturaNo e nroChaves = 0
	//Chaves.id e chaves.byteOffset = -1
	//Ponteiros = -1

	PAGINA pag;

	pag.alturaNo = 0;
	pag.nroChaves = 0;
	for(int i = 0; i < ORDEM - 1; i++){
		(pag.chaves[i]).id = (pag.chaves[i]).byteOffset = -1;
	}

	for(int i = 0; i < ORDEM; i++){
		pag.ponteiros[i] = -1;
	}

	return pag;
}

//Lê os campos do cabeçalho da árvore-B e retorna uma struct com os valores.
CABECALHO_ARV_B le_cabecalho_arvB(FILE *arvB){
	CABECALHO_ARV_B cabArvB;
	char lixo[60]; 
	(cabArvB.status)[0] = '0';

	if(arvB == NULL)
		return cabArvB;

	//Garante que o cursor esteja no início do arquivo para realizar a leitura.
	rewind(arvB);

	//Leitura dos campos.
	fread(cabArvB.status, 1, 1, arvB);
	fread(&(cabArvB.noRaiz), 4, 1, arvB);
	fread(&(cabArvB.proxRRN), 4, 1, arvB);
	fread(&(cabArvB.nroChaves), 4, 1, arvB);

	fread(lixo, 47, 1, arvB); //Lê os bytes não usados do cabeçalho.

	return cabArvB;
}

//Escreve os dados do cabeçalho no arquivo.
void escreve_cabecalho_arvB(FILE *arvB, CABECALHO_ARV_B cab){
	if(arvB == NULL)
		return;

	//Declaração e inicialização do vetor usado para preencher os bytes não usados do cabeçalho.
	char lixo[47];
	for(int i = 0; i < 47; i++)
		lixo[i] = '$';

	//Garante que o cursor esteja no início do arquivo.
	rewind(arvB);

	//Escrita dos campos.
	fwrite(cab.status, 1, 1, arvB);
	fwrite(&(cab.noRaiz), 4, 1, arvB);
	fwrite(&(cab.proxRRN), 4, 1, arvB);
	fwrite(&(cab.nroChaves), 4, 1, arvB);
	fwrite(lixo, 47, 1, arvB);
}

//Escreve os dados de uma página no arquivo, no RRN passado como parâmetro.
void escreve_pagina(FILE *arvB, PAGINA pag, int RRN){
	if(arvB == NULL)
		return;

	//Posiciona o cursor na posição correspondente ao RRN passado.
	fseek(arvB, (RRN + 1)*TAM_PAGINA, SEEK_SET);

	//Escrita dos campos.
	fwrite(&(pag.alturaNo), 4, 1, arvB);
	fwrite(&(pag.nroChaves), 4, 1, arvB);

	for(int i = 0; i < ORDEM - 1; i++){
		fwrite(&(((pag.chaves)[i]).id), 4, 1, arvB);
		fwrite(&(((pag.chaves)[i]).byteOffset), 8, 1, arvB);
	}

	for(int i = 0; i < ORDEM; i++){
		fwrite(&((pag.ponteiros)[i]), 4, 1, arvB);
	}
}


//Lê os campos de uma página localizada no RRN passado como parâmetro.
PAGINA le_pagina_arvB(FILE *arvB, int RRN){
	PAGINA pag; //Struct para armazenar os campos lidos.

	//Posiciona o cursor na posição correta para a leitura.
	fseek(arvB, (RRN + 1)*TAM_PAGINA, SEEK_SET);

	//Leitura dos campos.
	fread(&(pag.alturaNo), 4, 1, arvB);
	fread(&(pag.nroChaves), 4, 1, arvB);

	for(int i = 0; i < ORDEM - 1; i++){
		fread(&((pag.chaves)[i]).id, 4, 1, arvB);
		fread(&((pag.chaves)[i]).byteOffset, 8, 1, arvB);
	}

	for(int i = 0; i < ORDEM; i++)
		fread(&((pag.ponteiros)[i]), 4, 1, arvB);

	return pag;
}

//Protótipos das funções.
void insercao(FILE *arvB, int RRN, CHAVE_ARV_B chave, int *FILHO_DIREITO_PROMOCAO, CHAVE_ARV_B *CHAVE_PROMOVIDA, int *proxRRN, int *nroChaves, int *PROMOCAO);
void split(CHAVE_ARV_B chave_inserida, int filho_direito_promocao, PAGINA *pag, CHAVE_ARV_B *CHAVE_PROMOVIDA, int *FILHO_DIREITO_PROMOCAO, PAGINA *novaPag, int proxRRN);

//Função que inverte o status do arquivo da árvore B.
void modificaStatus_arvB(FILE *arvB){
	if(arvB == NULL)
		return;

	char status;

	rewind(arvB);
	fread(&status, 1, 1, arvB);

	//Verifica se o status atual é 0 ou 1, e atribui o oposto a 'status'.
	if(status == '1')
		status = '0';
	else
		status = '1';

	//Posiciona o cursor para escrita.
	rewind(arvB);
	//Escreve o novo status no arquivo de índice.
	fwrite(&status, 1, 1, arvB);
}

/*
Funções que realizam a inserção de uma chave na árvore B.
Dividida em 3 partes:
	função 'insere_arvoreB': equivalente ao procedimento driver. Trata splits na raíz da árvore.
	função 'inserção': realiza a inserção da chave na árvore. É recursiva e chamada por 'insere_arvoreB'.
	função 'split': trata splits em um nó.
*/
//Trata a inserção de uma chave (id + byteOffset) na árvore B.
void insere_arvoreB(FILE *arvB, int id, long int byteOffset){
	if(arvB == NULL)
		return;

	//Declaração de variáveis de retorno das funções 'insercao' e 'split'. Serão passadas por referência para essas funções.
	int PROMOCAO = 0; //1 se houve promoção de chave do nó inferior, 0 caso contrário.
	int SUCESSO = 0; //1 se a inserção foi realizada, 0 caso contrário (a chave já está presente na árvore).
	CHAVE_ARV_B CHAVE_PROMOVIDA; //Chave promovida do nó inferior.
	CHAVE_PROMOVIDA.id = CHAVE_PROMOVIDA.byteOffset = -1; //Inicialização padrão dos campos (no momento, nenhuma chave foi promovida).
	int FILHO_DIREITO_PROMOCAO = -1; //Filho direito da chave promovida.


	//Leitura do cabeçalho.
	CABECALHO_ARV_B cab = le_cabecalho_arvB(arvB);

	//Verifica a consistência do arquivo de índice. Se status == 0, aborta a inserção.
	if(cab.status[0] == '0')
		return;

	//Haverá escrita no arquivo, logo status deve receber 0.
	modificaStatus_arvB(arvB);

	//Declaração e inicialização de structs para armazenar a raíz atual e uma possível nova raíz, em caso de split da raíz atual.
	PAGINA raiz, novaRaiz;
	raiz = novaRaiz = inicializaPagina();

	//Struct para armazenar a nova chave a ser inserida na árvore.
	CHAVE_ARV_B chave_inserida;
	chave_inserida.id = id;
	chave_inserida.byteOffset = byteOffset;

	//Verifica se a árvore está vazia.
	if(cab.noRaiz == -1){
		//Se estiver, insere a chave na raíz.
		(raiz.chaves)[0] = chave_inserida;
		(raiz.nroChaves)++;

		//Atualização dos campos do cabeçalho após a inserção e a criação da nova raíz.
		cab.noRaiz = 0;
		cab.proxRRN = 1;
		(cab.nroChaves)++;
		(cab.status)[0] = '1';

		//Escrita do cabeçalho atualizado e da raíz no arquivo.
		escreve_cabecalho_arvB(arvB, cab);
		escreve_pagina(arvB, raiz, 0);

		//Fim da inserção.
		return;
	}

	//Caso a árvore não esteja vazia, lê a raiz.
	raiz = le_pagina_arvB(arvB, cab.noRaiz);

	//Chama a função 'inserção', passando as variáveis de retorno por referência. Seus valores serão tratado após a volta desta função.
	insercao(arvB, cab.noRaiz, chave_inserida, &FILHO_DIREITO_PROMOCAO, &CHAVE_PROMOVIDA, &(cab.proxRRN), &SUCESSO, &PROMOCAO);

	if(SUCESSO) //Se a inserção foi bem sucedida, atualiza o número de chaves presentes na árvore.
		(cab.nroChaves)++;

	if(PROMOCAO){ //Verifica se PROMOCAO == 1, indicando que houve um split da raíz.
		(novaRaiz.chaves)[0] = CHAVE_PROMOVIDA; //Acomoda a chave promovida na nova raíz.
		(novaRaiz.ponteiros)[0] = cab.noRaiz; //Atualiza filho esquerdo da chave promovida.
		(novaRaiz.ponteiros)[1] = FILHO_DIREITO_PROMOCAO; //Atualiza filho direito da chave promovida.
		novaRaiz.alturaNo = raiz.alturaNo + 1; //A nova raíz está um nível acima da raíz antiga.
		novaRaiz.nroChaves++; //Atualiza número de chaves da nova raíz.

		//Escreve a nova raíz no arquivo no próximo RRN livre (as páginas resultantes do split da raíz antiga são escrita no arquivo pela função 'insercao').
		escreve_pagina(arvB, novaRaiz, cab.proxRRN);

		//Atualização e escrita do cabeçalho no arquivo.
		cab.noRaiz = cab.proxRRN; //A nova raíz foi escrita no RRN livre.
		cab.proxRRN++; //O RRN livre anterior foi utilizado para escrita da nova raíz. Logo, proxRRN deve ser incrementado.
		(cab.status)[0] = '1'; //As escritas no arquivo terminaram. Portanto, status recebe 1.
		escreve_cabecalho_arvB(arvB, cab); //Reescrita do cabeçalho.

		return;
	}

	//Se não houve split da raíz, apenas altera status para 1 e reescreve o cabeçalho.
	(cab.status)[0] = '1';
	escreve_cabecalho_arvB(arvB, cab);
}

void insercao(FILE *arvB, int RRN, CHAVE_ARV_B chave, int *FILHO_DIREITO_PROMOCAO, CHAVE_ARV_B *CHAVE_PROMOVIDA, int *proxRRN, int *SUCESSO, int *PROMOCAO){
	//Começa por uma busca pelo lugar adequado para inserir a nova chave.
	if(RRN == -1){ //Se chegou em um nó nulo, retorna, atualizando os parâmetros para que a chave seja inserida no nó acima, na volta da recursão.
		*PROMOCAO = 1; //Indica que o nó contido em CHAVE_PROMOVIDA deve ser inserida na volta da recursão.

		//Atualiza os valores de CHAVE_PROMOVIDA e FILHO_DIREITO_PROMOÇÃO, que serão inseridos no nó pai do atual.
		*CHAVE_PROMOVIDA = chave;
		*FILHO_DIREITO_PROMOCAO = -1;

		return;
	}

	//Lê a página no RRN atual.
	PAGINA pag = le_pagina_arvB(arvB, RRN);

	//Procura a posição onde o id a ser inserido está ou deveria estar.
	int pos;
	for(pos = 0; pos < pag.nroChaves && chave.id > ((pag.chaves)[pos]).id; pos++);

	//Se a chave já está presente na árvore, retorna, pois não pode haver inserção de id repetido.
	if(chave.id == ((pag.chaves)[pos]).id){
		*PROMOCAO = 0; //Indica que não houve promoção.
		*SUCESSO = 0; //Indica que não houve inserção.

		return;
	}

	//Desce um nível na árvore por meio da chamada recursiva à função 'insercao'.
	insercao(arvB, (pag.ponteiros)[pos], chave, FILHO_DIREITO_PROMOCAO, CHAVE_PROMOVIDA, proxRRN, SUCESSO, PROMOCAO);

	if(!(*PROMOCAO)) //Verifica se é necessário tratar a inserção, na página atual, de uma chave promovida do nível inferior.
		return;

	//Tratamento da promoção de uma chave.
	if(pag.nroChaves < ORDEM - 1){ //Verifica se há espaço no nó atual para a chave promovida.
		//Se houver, encontra a posição onde deve ser inserida, respeitando a ordenação das chaves.
		int pos;
		for(pos = 0; pos < pag.nroChaves && (*CHAVE_PROMOVIDA).id > ((pag.chaves)[pos]).id; pos ++);

		//Desloca as chaves maiores que a chave promovida para a direita, liberando espaço para a inserção.
		for(int i = ORDEM - 2; i > pos; i--){
			(pag.chaves)[i] = (pag.chaves)[i-1];
		}

		//Insere a chave promovida no lugar adequado.
		(pag.chaves)[pos] = *CHAVE_PROMOVIDA;

		//Desloca os ponteiros para liberar espaço para o filho direito da chave promovida.
		for(int i = ORDEM - 1; i > pos + 1; i--){
			(pag.ponteiros)[i] = (pag.ponteiros)[i-1];
		}

		//Insere o filho direito da chave promovida na posição adequada.
		(pag.ponteiros)[pos+1] = *FILHO_DIREITO_PROMOCAO;

		//Incrementa o número de chaves da página atual.
		pag.nroChaves++;

		//Reescreve a página com as atualizações no arquivo.
		escreve_pagina(arvB, pag, RRN);

		*SUCESSO = 1; //Indica que uma chave foi inserida na árvore.
		*PROMOCAO = 0; //Como havia espaço para inserção na página, não há um novo split e promoção. Logo, PROMOCAO recebe 0.

		return;
	}

	//Se não há espaço para inserção na página, é necessário fazer o split.
	PAGINA novaPag = inicializaPagina(); //Inicializa uma nova página.

	//Chama a rotina de split da página atual.
	split(*CHAVE_PROMOVIDA, *FILHO_DIREITO_PROMOCAO, &pag, CHAVE_PROMOVIDA, FILHO_DIREITO_PROMOCAO, &novaPag, *proxRRN);

	//Escreve as alterações feitas pela rotina 'split' no arquivo.
	escreve_pagina(arvB, pag, RRN); //Reescreve a página já existente.
	escreve_pagina(arvB, novaPag, *proxRRN); ////Escreve a nova página no próximo RRN livre.

	(*proxRRN)++; //Incrementa proxRRN.

	*PROMOCAO = 1; //Indica que houve promoção de chave para o nível superior.
	*SUCESSO = 1; //Indica que houve inserção de chave na árvore.
}

//Página auxiliar que tem espaço para armazenar uma chave e um ponteiro a mais, para tratar do split.
typedef struct pag_aux{
	int alturaNo;
	int nroChaves;
	CHAVE_ARV_B chaves[ORDEM];
	int ponteiros[ORDEM + 1];
}PAG_AUX;

void split(CHAVE_ARV_B chave_inserida, int filho_direito_promocao, PAGINA *pag, CHAVE_ARV_B *CHAVE_PROMOVIDA, int *FILHO_DIREITO_PROMOCAO, PAGINA *novaPag, int proxRRN){
	PAG_AUX paginaAux; //Página auxiliar.
	paginaAux.alturaNo = pag->alturaNo; //Salva a altura da página que sofrerá o split.
	paginaAux.nroChaves = 0;

	//Inserção ordenada das chaves da página que sofrerá o split e da chave a ser inserida, assim como dos ponteiros, na página auxiliar.
	int i, j;
	for(i = 0; i < ORDEM - 1 && chave_inserida.id > ((pag->chaves)[i]).id; i++){
		(paginaAux.chaves)[i] = (pag->chaves)[i];
		(paginaAux.ponteiros)[i] = (pag->ponteiros)[i];
		(paginaAux.nroChaves)++;
	}
	(paginaAux.ponteiros)[i] = (pag->ponteiros)[i];

	(paginaAux.chaves)[i] = chave_inserida;
	(paginaAux.ponteiros)[i+1] = filho_direito_promocao;
	(paginaAux.nroChaves)++;
	i++;

	for(; i < ORDEM; i++){
		(paginaAux.chaves)[i] = (pag->chaves)[i-1];
		(paginaAux.ponteiros)[i+1] = (pag->ponteiros)[i];
		(paginaAux.nroChaves)++;
	}

	//Calcula o índice da chave a ser promovida.
	int meio = (ORDEM - 1)/2;
	//Atualiza os parâmetros de retorno.
	*CHAVE_PROMOVIDA = (paginaAux.chaves)[meio];
	*FILHO_DIREITO_PROMOCAO = proxRRN;

	*pag = inicializaPagina();
	*novaPag = inicializaPagina();

	//Copiando chaves e ponteiros que precedem a chave promovida para 'pag'.
	for(i = 0; i < meio; i++){
		(pag->chaves)[i] = (paginaAux.chaves)[i];
		(pag->ponteiros)[i] = (paginaAux.ponteiros)[i];
		(pag->nroChaves)++;
	}
	(pag->ponteiros)[i] = (paginaAux.ponteiros)[i];
	i++;

	//Copiando chaves e ponteiros que sucedem a chave promovida para 'novaPag'.
	for(j = 0; i < ORDEM; i++, j++){
		(novaPag->chaves)[j] = (paginaAux.chaves)[i];
		(novaPag->ponteiros)[j] = (paginaAux.ponteiros)[i];
		(novaPag->nroChaves)++;
	}
	(novaPag->ponteiros)[j] = (paginaAux.ponteiros)[i];

	//A página já existente e a nova página estão no mesmo nível, que foi salvo em 'paginaAux.alturaNo'.
	pag->alturaNo = novaPag->alturaNo = paginaAux.alturaNo;
}


//Funções para busca de um id na árvore B.
//Retorna o byteOffset do id caso este seja encontrado, e -1 caso contrário.
long int busca_id_arvB_rec(FILE *arvB, int id, int RRN){
	if(RRN == -1) //Se chegou a um nó nulo, o id não está presente na árvore B.
		return -1;

	//Lê a página do RRN atual.
	PAGINA pag = le_pagina_arvB(arvB, RRN);

	//Procura a posição onde o id buscado está ou deveria estar.
	int pos;
	for(pos = 0; pos < pag.nroChaves && id > ((pag.chaves)[pos]).id; pos++);

	//Se o id foi encontrado, retorna seu byteOffset no arquivo de dados.
	if(id == ((pag.chaves)[pos]).id)
		return ((pag.chaves)[pos]).byteOffset;

	//Se ainda não foi encontrado, desce para o nível inferior da árvore com a recursão.
	return busca_id_arvB_rec(arvB, id, (pag.ponteiros)[pos]);
}

long int busca_id_arvB(FILE *arvB, int id){
	if(arvB == NULL)
		return -1;

	//Lê o cabeçalho para acessar o RRN da raíz.
	CABECALHO_ARV_B cabArvB = le_cabecalho_arvB(arvB);

	//Passa o RRN da raíz para a busca recursiva, para iniciar a busca.
	return busca_id_arvB_rec(arvB, id, cabArvB.noRaiz);
}