#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX 23 //tamanho maximo da hash

/*estrutura do livro*/
typedef struct{
    int isbn;
    char titulo[100], autor[100];
    int ano;
} Livro;

/*estrutura do no*/
typedef struct aux{
    struct aux *prox;
    Livro livro;
} NO, *PONT;

/*estrutura da lista*/
typedef struct{
    PONT inicio;
} Lista;

 /*inicializa a lista como nulo*/
void inicializarLista(Lista* l) {
    l->inicio = NULL;
}

/*insere um novo elemento na lista, na primeira posicao*/
void inserirLista(Lista *l, Livro livro){
    PONT novo = (PONT)malloc(sizeof(NO));
    novo->livro = livro;
    novo->prox = l->inicio;
    l->inicio = novo;
}

/*imprime a lista, percorrendo-a*/
void imprimirLista(Lista *l, int i){
    PONT aux = l->inicio;

    if(aux != NULL){
        printf("posicao %d: ", i);
        while(aux != NULL){
            printf("isbn: %d, titulo: %s, autor: %s, ano: %d\n", aux->livro.isbn, aux->livro.titulo, aux->livro.autor, aux->livro.ano);
            aux = aux->prox;
        }
    }
}

/*recebe um vetor char, transformando-o em um tipo int para calcular o resto*/
int divisao(char nome[]){
    int conversao = 0;
    int i = 0;
    while(nome[i] != '\0'){
        conversao += (int)nome[i];
        i++;
    }

    return (conversao % MAX);
}

/*busca na lista usando o isbn, para caso tenha nomes repetidos*/
PONT buscarLista(PONT inicio, int isbn) {
    PONT atual = inicio;
    while (atual != NULL) {
        if (atual->livro.isbn == isbn) {
            return atual;
        }
        atual = atual->prox;
    }
    return NULL;
}

/*remove um elemento da lista com base no isbn*/
void removerLista(PONT *inicio, int isbn) {
    PONT atual = *inicio, anterior = NULL; //guarda os ponteiros para o elemento atual e anterior, para fazer a substituicao se necessario
    while (atual != NULL && atual->livro.isbn != isbn) { //percorre a lista ate encontrar o elemento
        anterior = atual;
        atual = atual->prox;
    }
    if (atual == NULL) { //caso chegue no final da lista
        printf("livro nao encontrado\n");
        return;
    }
    if (anterior == NULL) { //caso seja o primeiro elemento da lista
        *inicio = atual->prox; //o inicio da lista sera o elemento depois do atual
    } else {
        anterior->prox = atual->prox; //senao o anterior passara a apontar para o elemento depois do atual
    }
    free(atual);
    printf("livro removido\n");
}

/*inicializa o vetor de listas*/
void inicializar(Lista hash[]){
    for(int i = 0; i < MAX; i++){
        inicializarLista(&hash[i]);
    }
}

/*calcula a posicao com base no nome do livro e chama a funcao de insercao na lista adequada*/
void inserir(Lista hash[], Livro livro){
    int indice = divisao(livro.titulo);
    inserirLista(&hash[indice], livro);
}

/*calcula a posicao que o livro na hash esta com base no nome, em seguida procura na lista, caso encontre, imprime as informacoes*/
void buscar(Lista hash[], int isbn, char titulo[]) {
    int indice = divisao(titulo);
    PONT resultado = buscarLista(hash[indice].inicio, isbn);
    if (resultado != NULL) {
        printf("livro encontrado: isbn: %d, titulo: %s, autor: %s, ano: %d\n", resultado->livro.isbn, resultado->livro.titulo, resultado->livro.autor, resultado->livro.ano);
    } else {
        printf("livro nao encontrado.\n");
    }
}

/*calcula a posicao com base no nome do livro e chama a funcao de remover na lista adequada*/
void remover(Lista hash[], int isbn, char titulo[]) {
    int indice = divisao(titulo);
    removerLista(&hash[indice].inicio, isbn);
}

/*chama a funcao de imprimir de cada lista caso ela nao seja nula*/
void imprimir(Lista hash[]) {
    for (int i = 0; i < MAX; i++) {
        if (hash[i].inicio == NULL) {
            printf("posicao %d: vazia\n", i);
        } else {
            imprimirLista(&hash[i], i);
        }
    }
}

/* imprime apenas uma posicao especifica da tabela hash */
void imprimirPosicao(Lista hash[], int i) {
    if (i < 0 || i >= MAX) {
        printf("posicao invalida\n");
        return;
    }
    
    if (hash[i].inicio == NULL) {
        printf("posicao %d: vazia\n", i);
    } else {
        imprimirLista(&hash[i], i);
    }
}

int main() {
    Lista hash[MAX];
    inicializar(hash);

    int opcao, isbn, posicao;
    char nome[100];

    while (1) {
        printf("\nMenu:\n");
        printf("1 - inserir livro\n");
        printf("2 - imprimir tabela hash\n");
        printf("3 - buscar livro\n");
        printf("4 - remover livro\n");
        printf("5 - imprimir posicao especifica\n");
        printf("6 - sair\n");
        printf("escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                Livro livro;
                printf("digite o ISBN do livro: ");
                scanf("%d", &livro.isbn);
                getchar();
                printf("digite o titulo do livro: ");
                gets(livro.titulo);
                printf("digite o autor do livro: ");
                gets(livro.autor);
                printf("digite o ano de publicacao: ");
                scanf("%d", &livro.ano);
                inserir(hash, livro);
                break;
            }
            case 2:
                imprimir(hash);
                break;
            case 3:
                printf("digite o isbn do livro a ser buscado: ");
                scanf("%d", &isbn);
                getchar();
                printf("digite o titulo do livro: ");
                gets(nome);
                buscar(hash, isbn, nome);
                break;
            case 4:
                printf("Digite o ISBN do livro a ser removido: ");
                scanf("%d", &isbn);
                getchar();
                printf("Digite o titulo do livro: ");
                gets(nome);
                remover(hash, isbn, nome);
                break;
            case 5:
                printf("digite a posicao a ser impressa: ");
                scanf("%d", &posicao);
                imprimirPosicao(hash, posicao);
                break;
            case 6:
                printf("encerrando\n");
                return 1;
            default:
                printf("opcao invalida\n");
                break;
        }
    }

    return 0;
}