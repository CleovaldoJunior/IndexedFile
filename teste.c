#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "index.h"

int main(int argc, char * argv[]) {
    int opcao, table_size;

    printf("DIGITE O TAMANHO DA TABELA: ");
    scanf("%d", &table_size);

    tab* tabela;

    tabela = inicializar_tabela("livros", "indices", table_size);


    inicializarIndices(tabela, table_size);
    recuperar_indices(tabela, table_size);

        while(1) {
        printf("\nDIGITE 1 PARA CADASTRAR 1 LIVRO;\n");
        printf("DIGITE 2 PARA IMPRIMIR A TABELA;\n");
        printf("DIGITE 3 PARA PROCURAR 1 LIVRO;\n");
        printf("DIGITE 4 PARA REMOVER 1 LIVRO;\n");
        printf("DIGITE 5 PARA EDITAR 1 LIVRO;\n");
        printf("DIGITE 99 PARA SAIR;\n");
        printf("\n");
		scanf("%d", &opcao);
		printf("\n");

        int valor;
        int codigo;
        liv* livro = NULL;

		switch(opcao) {
				case 1:
                    adicionarLivroArquivo(tabela, table_size);
                    printf("\n");
                    break;
				case 2:
                    imprimirTabela(tabela, table_size);
                    printf("\n");
                    break;
		        case 3:
		            printf("DIGITE O CODIGO: ");
		            scanf("%d", &codigo);
		            livro = procurarLivro(tabela, codigo, table_size);
		            if(livro != NULL){
		                imprimirLivro(livro);
		            }
		            break;
                case 4:
                    printf("DIGITE O CODIGO: ");
		            scanf("%d", &codigo);
		            remover_livro_referencia(tabela, codigo, table_size);
		            break;
                case 5:
                    printf("DIGITE O CODIGO: ");
                    scanf("%d", &codigo);
                    editar_livro(tabela, codigo, table_size);
                    break;
				case 99:
				    fclose(tabela->livros);
				    fclose(tabela->indices);
                    exit(0);
		}
	}
}
