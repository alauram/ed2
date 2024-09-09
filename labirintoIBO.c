/*codigo para solucionar um labirinto usando depth first search, funciona apenas inserindo um
vertice "Entrada", um "Saida" e um "*", imprime o caminho encontrado para facilitar a conferencia dos testes*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_NOME 8
#define BRANCO 0
#define CINZA 1

/*estrutura do vertice*/
typedef struct aux {
    struct aux **vet_vizinhos;  // ponteiro para um array de ponteiros para os vizinhos
    int num_vizinhos;           // numero de vizinhos conectados ao vertice
    int cor;                    // cor do vertice
    char id[MAX_NOME];          // nome do vértice
} VERTICE, *PONT;

/*estrutura do grafo*/
typedef struct {
    PONT *vertices; //ponteiro para um array de ponteiros para os vertices do grafo
    int numVertices; //numero de vertices do grafo
} GRAFO;

/*cria um vertice*/
PONT criarVertice(char *nome) {
    PONT novo = (PONT)malloc(sizeof(VERTICE)); //aloca memoria para o vertice
    strcpy(novo->id, nome); //atribui o nome ao vertice
    novo->vet_vizinhos = NULL; // inicialmente sem vizinhos
    novo->num_vizinhos = 0; 
    novo->cor = BRANCO; //pinta de branco
    return novo;
}
/*funcao para criar o grafo com o numero de vertices necessarios*/
GRAFO* criarGrafo(int numVertices) {
    GRAFO *grafo = (GRAFO*)malloc(sizeof(GRAFO));
    grafo->vertices = (PONT*)malloc(numVertices * sizeof(PONT)); //aloca memoria para os ponteiros dos vertices no array
    grafo->numVertices = 0; //como o grafo foi criado mas os vertices ainda nao, define como 0
    return grafo;
}

/*encontra o vertice com base no nome e retorna o ponteiro do array de ponteiros dos vertices*/
PONT encontrarVertice(GRAFO* grafo, char* nome) {
    for (int i = 0; i < grafo->numVertices; i++) {
        if (strcmp(grafo->vertices[i]->id, nome) == 0) {
            return grafo->vertices[i];
        }
    }
    return NULL;
}

/*adiciona um vertice para a lista de vizinhos de outro vertice*/
void adicionarVizinho(PONT vertice, PONT vizinho) {
    vertice->vet_vizinhos = (PONT*)realloc(vertice->vet_vizinhos, (vertice->num_vizinhos + 1) * sizeof(PONT)); //realoca memoria do array para adicionar o vizinho
    vertice->vet_vizinhos[vertice->num_vizinhos] = vizinho; //adiciona o vizinho ao array de ponteiros para vizinhos
    vertice->num_vizinhos++; //incrementa o numero de vizinhos do vertice
}

/*cria uma aresta entre dois vertices, se eles ja existirem, apenas liga, senao, cria o vertice antes*/
void criarAresta(GRAFO* grafo, char* inicio, char* destino) {
    PONT inicioNo = encontrarVertice(grafo, inicio); //encontra o vertice de inicio
    PONT destinoNo = encontrarVertice(grafo, destino); //encontra o vertice de destino

    if (!inicioNo) { //se o no de inicio nao existir no grafo
        inicioNo = criarVertice(inicio); //cria o vertice
        grafo->vertices[grafo->numVertices++] = inicioNo; //aumenta o numero de vertices do grafo e adiciona nessa posicao do array de ponteiros
    }

    if (!destinoNo) { //se o no de destino nao existir no grafo
        destinoNo = criarVertice(destino); //cria o vertice
        grafo->vertices[grafo->numVertices++] = destinoNo; //aumenta o numero de vertices do grafo e adiciona nessa posicao do array de ponteiros
    }

    adicionarVizinho(inicioNo, destinoNo); //adiciona o vertice de destino como vizinho do vertice de inicio
    adicionarVizinho(destinoNo, inicioNo); //adiciona o vertice de inicio como vizinho do vertice de destino
}

void DFS(GRAFO* grafo, PONT atual, char* destino, char **caminho, char **menorCaminho, int tamanho, int* menorTamanho) {
    atual->cor = CINZA; //marca o vertice atual como cinza (sendo visitado)

    strcpy(caminho[tamanho], atual->id); //adiciona o vertice atual ao caminho
    tamanho++;//aumenta o tamanho do caminho atual

    if (strcmp(atual->id, destino) == 0) { //se o vertice atual for o destino procurado
        if (tamanho < *menorTamanho) { //se o caminho encontrado for menor que o menor caminho encontrado anteriormente
            *menorTamanho = tamanho; //atualiza o menor tamanho
            for (int i = 0; i < tamanho; i++) {
                strcpy(menorCaminho[i], caminho[i]); //salva o caminho como menor caminho
            }
        }
    } else { //caso nao encontre o destino, explora os vizinhos do vertice atual
        for (int i = 0; i < atual->num_vizinhos; i++) { //percorre todos os vizinhos do no
            PONT vizinho = atual->vet_vizinhos[i]; //obtem o vizinho
            if (vizinho->cor == BRANCO) { //se o vizinho for branco, chama a funcao, ocorrendo a recursao
                DFS(grafo, vizinho, destino, caminho, menorCaminho, tamanho, menorTamanho);
            }
        }
    }

    atual->cor = BRANCO; //marca o vertice como branco, para ser visitado novamente se necessario (precisa fazer isso para caso haja outro caminho menor que passe pelo mesmo vertice)
}

int menorCaminho(GRAFO* grafo, char* inicio, char* destino) {
    PONT inicioNo = encontrarVertice(grafo, inicio); //encontra o vertice de inicio
    int menorTamanho = grafo->numVertices + 1; //inicia o menor caminho como vertices + 1 para comparaçoes
    
    /*aloca os arrays para armazenar os caminhos*/
    char **caminho = (char **)malloc(grafo->numVertices * sizeof(char *));
    char **menorCaminho = (char **)malloc(grafo->numVertices * sizeof(char *));
    for (int i = 0; i < grafo->numVertices; i++) {
        caminho[i] = (char *)malloc(MAX_NOME * sizeof(char));
        menorCaminho[i] = (char *)malloc(MAX_NOME * sizeof(char));
    }

    DFS(grafo, inicioNo, destino, caminho, menorCaminho, 0, &menorTamanho);

    if (menorTamanho <= grafo->numVertices) { //se o caminho for menor que o numero de vertices imprime o caminho
        printf("menor caminho entre %s e %s:\n", inicio, destino);
        for (int i = 0; i < menorTamanho; i++) {
            printf("%s ", menorCaminho[i]);
        }
        printf("\n");
    } else {
        printf("nao existe caminho entre %s e %s\n", inicio, destino);
    }
    /*libera a memoria*/
    for (int i = 0; i < grafo->numVertices; i++) {
        free(caminho[i]);
        free(menorCaminho[i]);
    }
    free(caminho);
    free(menorCaminho);

    return menorTamanho; //retorna o tamanho do menor caminho
}

int main() {
    int numVertices, numArestas;
    printf("digite o numero de vertices e arestas:\n");
    scanf("%d %d", &numVertices, &numArestas);
    if (numVertices >= 4 && numVertices <= 4000 && numArestas >= 4 && numArestas <= 5000){
    GRAFO* grafo = criarGrafo(numVertices);

    printf("Digite as arestas:\n");
    for (int i = 0; i < numArestas; i++) {
        char inicio[MAX_NOME], destino[MAX_NOME];
        scanf("%s %s", inicio, destino);
        criarAresta(grafo, inicio, destino);
    }

    int primeiroCaminho = menorCaminho(grafo, "Entrada", "*");
    int segundoCaminho = menorCaminho(grafo, "*", "Saida");

    printf("o menor caminho e %d\n", (primeiroCaminho + segundoCaminho) - 2); //subtrai 2 para nao contar a entrada e nao contar duas vezes o *

    for (int i = 0; i < grafo->numVertices; i++) {
        free(grafo->vertices[i]->vet_vizinhos);
        free(grafo->vertices[i]);
    }
    free(grafo->vertices);
    free(grafo);

    }

    else {
        printf("numero de vertices ou arestas invalido\n");
    }
    return 0;
}
