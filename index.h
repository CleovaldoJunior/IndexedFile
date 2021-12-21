#ifndef INDEX_H
#define INDEX_H
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#define P 7

typedef struct livro{
    int codigo;
	char *titulo;
	char *autor;
	int isbn;
	char *editora;

} liv;

typedef struct nodo{
    int indice;
    struct nodo* prox;
}no;

typedef no * lista;

typedef struct Hash{
	int chave;
	lista indice;
}hash;

typedef struct tabela{
    FILE* livros;
    FILE* indices;
    char *nomeArq;
    char *nomeArqIndice;
    hash *ha;
}tab;

lista adicionarLista (int indice, lista l);
lista removerLista (int codigo, lista l);

void imprimirListaIndices(lista l);
void imprimirTabela(tab* tabela, int tam);
void imprimirLivro(liv* livro);

int Hash(int key, int tam);

liv * ler_dados();
int escrever_livro_arquivo(tab* tabela, liv* livro);
void adicionarLivroNaTabela(tab* tabela, int tam);
void remover_livro_referencia(tab* tabela, int codigo, int tamanho);
void adicionarLivroArquivo(tab *tabela, int tam);
tab* inicializar_tabela(char* nome, char* nomeIndices, int table_size);
void inicializarIndices(tab *tabela, int tam);

char* tirar_enter(char *string);
char* ler_campo(FILE* f);

void recuperar_indices(tab* tabela, int tamanho);
int referencia_livro(tab* tabela, int codigo, int tamanho);

void apagar_campo_indice(tab* tabela, int indice);
liv *procurarLivro(tab* tabela, int codigo, int tam);

void editar_livro(tab* tabela, int codigo, int tamanho);

#endif
