#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/*numero maximo de elementos da hash*/
#define MAX 32

/*definicao da estrututura do livro*/
typedef struct {
    int isbn;
    char titulo[100], autor[100];
    int ano;
} Livro;

/*inicializa o isbn dos elementos da hash como -1, usado de referencia para um elemento vazio*/
void inicializar(Livro hash[]){
    for(int i = 0; i < MAX; i++){
        hash[i].isbn = -1;
    }
}

/*calcula a dobra fazendo xor dos numeros binarios, dividindo o numero em 2 partes de 5 bits e operando*/
int dobra(int id){
    int parte1 = id >> 5;
    int parte2 = id & 31;

    return (parte1 ^ parte2);
}

/*calcula o resto da divisao*/
int divisao(int id){
    return id % MAX;
}

/*insere um elemento na hash, calculando a primeira posicao com divisao e, em caso de colisao, faz duplo hash com a dobra*/
void insercao(Livro hash[], Livro livro) {
    int h1 = divisao(livro.isbn);
    int h2 = dobra(livro.isbn);

    int i = h1; //primeira tentativa e o resto da divisao

    while(hash[i].isbn != -1){ //caso esteja ocupado, tenta a proxima posicao, calculada pelo duplo hash
        i = (h1 + i * h2) % MAX; //duplo hash
        if(i == h1){ //caso o duplo hash volte para a primeira tentativa, a tabela esta cheia
            printf("tabela cheia\n");
            return;
        }
    }
    hash[i] = livro; //livro e inserido na posicao se seu isbn for diferente de -1
    printf("inserido na posicao %d\n", i);
}

/*busca um elemento usando a funcao hash, se nao encontra-lo, calcula duplo hash*/
int buscar(Livro hash[], int isbn){
    int h1 = divisao(isbn);
    int h2 = divisao(isbn);

    int i = h1;

    while(hash[i].isbn != -1){
        if (hash[i].isbn == isbn){
            return i; //retorna o indice do elemento encontrado
        }
        i = (h1 + i * h2) % MAX;
    }

    return -1; //caso o calculo do hash caia em um elemento vazio, retorna -1
}

/*remove elemento da hash usando funcao buscar*/
void remover(Livro hash[], int isbn){
    int i = buscar(hash, isbn);
    if(i == -1){
        printf("nao encontrado\n");
    }
    else {
        hash[i].isbn = -1; //marca como vazio
        printf("removido da posicao %d\n", i);
    }
}

/* imprime o livro em uma determinada posicao da tabela hash */
void imprimirPosicao(Livro hash[], int i) {
    if (i < 0 || i >= MAX) {
        printf("posicao invalida\n");
        return;
    }
    
    if (hash[i].isbn != -1) {
        printf("posicao %d: isbn: %d, titulo: %s, autor: %s, ano: %d\n", 
            i, hash[i].isbn, hash[i].titulo, hash[i].autor, hash[i].ano);
    } else {
        printf("posicao %d: vazia\n", i);
    }
}

/*imprime a tabela percorrendo toda a lista, caso esteja marcada como vazia, indica que esta vazio*/
void imprimirTabela(Livro hash[]) {
    for (int i = 0; i < MAX; i++) {
        if (hash[i].isbn != -1) {
            printf("posicao %d: isbn: %d, titulo: %s, autor: %s, ano: %d\n", 
                i, hash[i].isbn, hash[i].titulo, hash[i].autor, hash[i].ano);
        } else {
            printf("posicao %d: vazia\n", i);
        }
    }
}

int main() {
    Livro hash[MAX];
    inicializar(hash);

    int opcao, n, posicao;
    while(1) {
        printf("\nMenu:\n");
        printf("1 - inserir livro\n");
        printf("2 - imprimir tabela hash\n");
        printf("3 - buscar\n");
        printf("4 - remover\n");
        printf("5 - imprimir livro na posicao\n");
        printf("6 - sair\n");
        printf("escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Livro livro;
                printf("digite o ISBN do livro: ");
                scanf("%d", &livro.isbn);
                printf("digite o titulo do livro: ");
                scanf("%s", livro.titulo);
                getchar();
                printf("digite o autor do livro: ");
                scanf("%s", livro.autor);
                getchar();
                printf("digite o ano de publicacao: ");
                scanf("%d", &livro.ano);
                insercao(hash, livro);
                break;
            }
            case 2:
                imprimirTabela(hash);
                break;
            case 3:
                printf("digite o isbn do livro a ser buscado\n");
                scanf("%d", &n);
                int i = buscar(hash, n);
                if(i != -1){
                    printf("encontrado na posicao %d\n", i);
                }
                else {
                    printf("nao encontrado\n");
                }
                break;
            case 4:
                printf("digite o isbn do livro a ser removido\n");
                scanf("%d", &n);
                remover(hash, n);
                break;
            case 5:
                printf("digite a posicao do livro a ser impresso\n");
                scanf("%d", &posicao);
                imprimirPosicao(hash, posicao);
                break;
            case 6:
                printf("saindo\n");
                return 0;
            default:
                printf("opcao invalida\n");
                break;
        }
    }

    return 0;
}