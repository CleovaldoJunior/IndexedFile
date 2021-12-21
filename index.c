#include <stdio.h>
#include <stdlib.h>
#include "index.h"
#include <string.h>

//** ADICIONA INDICE À LISTA DE INDICE DO HASH
lista adicionarLista(int indice, lista l){
    lista novo = (lista) malloc(sizeof(struct nodo));
    if(l == NULL){
        novo->indice = indice;
        novo->prox = NULL;
        l = novo;
        lista aux = l;
        return l;
    }else{
        if(l->indice == indice){
            return l;
        }else{
            l->prox = adicionarLista(indice, l->prox);
        }
    }
}

//** REMOVE INDICE DA LISTA DE INDICE DO HASH
lista removerLista(int indice, lista l){
    if(l->indice == indice){
        l = l->prox;
        return l;
    }else{
        while(l->prox != NULL){
            if(l->prox->indice == indice){
                l->prox = l->prox->prox;
                return l;
            }
        }

    }
}

//** IMPRIME INDICE DA LISTA DE INDICE DO HASH
void imprimirListaIndices(lista indices){
    lista aux = indices;
    if(aux == NULL){
        printf("VAZIO ");
    }else{
        while(aux != NULL){
            printf("[%d] ", aux->indice);
            aux = aux->prox;
        }
    }
    printf("\n");
}

//** IMPRIME UM LIVRO FORMATADO
void imprimirLivro(liv* livro){
    if(livro != NULL){
       printf("[Titulo: %s, Autor: %s, Editora: %s, ISBN: %d, Codigo: %d] ",livro->titulo, livro->autor, livro->editora, livro->isbn, livro->codigo);
    }
}

//** IMPRIME TODA A TABELA HASH DE INDICE
void imprimirTabela(tab *tabela, int tam){
    int i;
    for(i = 0; i < tam; i++){
        printf("CHAVE: %d  ", tabela->ha[i].chave);
        printf("LISTA:  ");
        imprimirListaIndices(tabela->ha[i].indice);
        printf("\n");
    }
}

//** FAZ O HASH
int Hash(int key, int tam){
    return (key*P)%tam;
}

//** CRIA UM LIVRO DE ACORDO COM A ENTRADA DO USUARIO E O RETORNA
liv * ler_dados() {
    liv * novo = (liv *) malloc(sizeof(liv));
    char * buffer = (char *) malloc(sizeof(char) * 256);

    printf ("Digite o isbn: ");
    scanf("%d", &novo->isbn);

    printf("Digite o Codigo: ");
    scanf("%d", &novo->codigo);

    getchar() ;
    printf ("Digite o titulo: ");
    fgets(buffer, 256, stdin);
    novo->titulo = strdup(tirar_enter(buffer));

    printf ("Digite o autor: ");
    fgets(buffer, 256, stdin);
    novo->autor = strdup(tirar_enter(buffer));

    printf ("Digite o editora: ");
    fgets(buffer, 256, stdin);
    novo->editora = strdup(tirar_enter(buffer));

    free(buffer);
    return novo;
}

//** ESCREVE O LIVRO NO ARQUIVO
int escrever_livro_arquivo(tab *tabela, liv *l) {
    int fim;
    fseek(tabela->livros, 0, SEEK_END);
    fim = ftell(tabela->livros);
    fprintf(tabela->livros, "%d|", l->codigo);
    fprintf(tabela->livros, "%d|", l->isbn);
    fprintf(tabela->livros, "%s|", l->titulo);
    fprintf(tabela->livros, "%s|", l->autor);
    fprintf(tabela->livros, "%s", l->editora);
    fprintf(tabela->livros, "\n");
    return fim;
}

//** ADICIONA A REFERENCIA DO LIVRO NA LISTA DO HASH E ESCREVE INDICE NO ARQUIVO
void adicionarLivroArquivo(tab *tabela, int tam){
    int aux, referencia;
    liv* livro = ler_dados();
    aux = Hash(livro->codigo, tam);
    referencia = escrever_livro_arquivo(tabela, livro);
    tabela->ha[aux].indice = adicionarLista(referencia, tabela->ha[aux].indice);
    fprintf(tabela->indices, "%d\n", referencia);
    fflush(tabela->livros);
    fflush(tabela->indices);
}


//** REMOVE A REFERENCIA INDICE DO ARQUIVO
void remover_livro_referencia(tab* tabela, int codigo, int tamanho){
    int referencia, hash, i;
    lista aux = NULL;

    hash = Hash(codigo, tamanho);

    fseek(tabela->indices, 1, SEEK_SET);
    referencia = referencia_livro(tabela, codigo, tamanho);

    tabela->ha[hash].indice = removerLista(referencia, tabela->ha[hash].indice);

    fopen(tabela->nomeArqIndice, "w+");
    fflush(tabela->indices);
    fopen(tabela->nomeArqIndice, "a+");

    for(i = 0; i < tamanho; i++){
        aux = tabela->ha[i].indice;
        while(aux != NULL){
            fprintf(tabela->indices, "%d\n", aux->indice);
            aux = aux->prox;
        }
    }
    fflush(tabela->indices);
}

//** INICIALIZA A TABELA QUE CONTEM O HASH E O ARQUIVO
tab* inicializar_tabela(char* nome, char* nomeIndices, int table_size){
    tab* tabela = (tab*) malloc(sizeof(tab));
    tabela->nomeArq = strdup(nome);
    tabela->livros = fopen(nome, "a+");
    tabela->nomeArqIndice = strdup(nomeIndices);
    tabela->indices = fopen(nomeIndices, "a+");
    tabela->ha = (hash*) malloc(sizeof(hash)*table_size);
    if(tabela->livros == NULL) {
        exit(1);
    }
    return tabela;
}

//** INICIALIZA OS INDICES DA TABELA HASH
void inicializarIndices(tab* tabela, int tam){
    int i;
    for(i = 0; i < tam; i++){
        tabela->ha[i].chave = i;
        tabela->ha[i].indice = NULL;
    }
}

//** TIRA ENTRER DE 1 CHAR[]
char* tirar_enter(char *string) {
	string[strlen(string) -1] = '\0';
	return string;
}

//** LER O ARQUIVO E PASSAR PARA STRING CADA PARAMETRO
char *ler_campo(FILE *f){
    char * buffer = malloc(sizeof(char) * 256);
    int i;
    for(i = 0; i < 256; i++){
        buffer[i] = fgetc(f);
        if(buffer[i] == '|' || buffer[i] == '\n') {
            buffer[i] = '\0';
            break;
        }
    }
    return buffer;
}

liv *procurarLivro(tab* tabela, int codigo, int tam){
    liv* livro = (liv*) malloc(sizeof(liv));
    int hash;
    hash = Hash(codigo, tam);
    lista aux = tabela->ha[hash].indice;
    while(aux != NULL){
        fseek(tabela->livros, aux->indice, SEEK_SET);
        if(codigo == atoi(ler_campo(tabela->livros))){
            fseek(tabela->livros, aux->indice, SEEK_SET);
            livro->codigo = atoi(ler_campo(tabela->livros));
            livro->isbn = atoi(ler_campo(tabela->livros));
            livro->titulo = strdup(ler_campo(tabela->livros));
            livro->autor = strdup(ler_campo(tabela->livros));
            livro->editora = strdup(ler_campo(tabela->livros));
            break;
        }
        aux = aux->prox;
    }
    return livro;
}

int referencia_livro(tab* tabela, int codigo, int tamanho){
    liv* livro = (liv*) malloc(sizeof(liv));
    int hash;
    hash = Hash(codigo, tamanho);
    lista aux = tabela->ha[hash].indice;
    while(aux != NULL){
        fseek(tabela->livros, aux->indice, SEEK_SET);
        if(codigo == atoi(ler_campo(tabela->livros))){
            fseek(tabela->livros, aux->indice, SEEK_SET);
            return aux->indice;
        }
        aux = aux->prox;
    }

}

void recuperar_indices(tab* tabela, int tamanho){
    int referencia, aux, hash, True;

    fseek(tabela->indices, 0, SEEK_END);

    if(ftell(tabela->indices) == 0){
        return ;
    }else{
        fseek(tabela->indices, 0, SEEK_SET);
        while(!feof(tabela->indices)){
            referencia = atoi(ler_campo(tabela->indices));
            fseek(tabela->livros, referencia, SEEK_SET);
            hash = Hash(atoi(ler_campo(tabela->livros)), tamanho);
            tabela->ha[hash].indice = adicionarLista(referencia, tabela->ha[hash].indice);
        }
    }
}


void editar_livro(tab* tabela, int codigo, int tamanho){
    liv * novo = procurarLivro(tabela, codigo, tamanho);
    char * buffer = (char *) malloc(sizeof(char) * 256);

    printf ("Digite o isbn: ");
    scanf("%d", &novo->isbn);
    getchar() ;
    printf ("Digite o titulo: ");
    fgets(buffer, 256, stdin);
    novo->titulo = strdup(tirar_enter(buffer));

    printf ("Digite o autor: ");
    fgets(buffer, 256, stdin);
    novo->autor = strdup(tirar_enter(buffer));

    printf ("Digite o editora: ");
    fgets(buffer, 256, stdin);
    novo->editora = strdup(tirar_enter(buffer));

    free(buffer);

    remover_livro_referencia(tabela, codigo, tamanho);
    printf("%d",novo->codigo);
    int referencia = escrever_livro_arquivo(tabela, novo);
    int aux = Hash(codigo, tamanho);
    tabela->ha[aux].indice = adicionarLista(referencia, tabela->ha[aux].indice);
    fprintf(tabela->indices, "%d\n", referencia);
    fflush(tabela->livros);
    fflush(tabela->indices);

}


















