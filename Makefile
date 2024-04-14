all:
	gcc -o trabalho funcoes_fornecidas.c trabalhoarquivo.c -std=c99 -Wall

run:
	./trabalho