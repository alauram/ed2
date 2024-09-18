#include <stdio.h>
#include <stdlib.h>
#include <math.h>


/*estrutura do vertice*/
typedef struct {
    int x, y;
} VERTICE, *PONT;

/*estrutura da aresta*/
typedef struct {
    int origem, destino;
    double distancia;
} ARESTA;

/*estrutura do grafo*/
typedef struct {
    PONT *vertices;
    int numVertices;
    ARESTA *arestas;
    int numArestas;
} GRAFO;

/*comparacao para ordenar o vetor de arestas posteriormente*/
int comparar(ARESTA *a, ARESTA *b) {
    if (a->distancia < b->distancia) return -1; //caso seja menor, retorna -1, nao faz troca
    if (a->distancia > b->distancia) return 1; //caso seja maior, retorna 1 e sera realizada a troca
    return 0; //caso sejam iguais retorna 0 e não fará troca
}

/*funcao para encontrar o pai, para saber se dois nós pertencem a mesma árvore*/
int encontrar(int *pai, int i) {
    if (pai[i] == i) { //caso o elemento atual do vetor de pais seja o pai, retorna o indice
        return i;
    }
    pai[i] = encontrar(pai, pai[i]); //caso contrário procura recursivamente pelo pai
    return pai[i];
}

/*funcao para unir duas arvores distintas caso sejam diferentes*/
void unir(int *pai, int x, int y) {
    int raizX = encontrar(pai, x);
    int raizY = encontrar(pai, y);
    if (raizX != raizY) {
        pai[raizY] = raizX; // une as árvores
    }
}

/*algoritmo de kruskal para arvore geradora minima*/
double kruskal(GRAFO *grafo) {
    int *pai = (int *)malloc(grafo->numVertices * sizeof(int)); //aloca um vetor para armazenar o pai de cada vértice
    double total = 0.0; //inicializa o valor total da árvore geradora mínima

    for (int i = 0; i < grafo->numVertices; i++) {
        pai[i] = i; //inicializa todos os pais como eles mesmos
    }
    
    //ordena as arestas com bubble sort
    for (int i = 0; i < grafo->numArestas - 1; i++){
        for (int j = 0; j < grafo->numArestas - 1; j++){
            if (comparar(&grafo->arestas[j], &grafo->arestas[j + 1]) > 0){
                ARESTA temp = grafo->arestas[j];
                grafo->arestas[j] = grafo->arestas[j + 1];
                grafo->arestas[j + 1] = temp;
            }
        }
    }
    //itera sobre o vetor de arestas ordenado
    for (int i = 0; i < grafo->numArestas; i++)
    {
        int raizX = encontrar(pai, grafo->arestas[i].origem); //encontra o pai do vertice de origem
        int raizY = encontrar(pai, grafo->arestas[i].destino); //encontra o pai do vertice de destino
        if (raizX != raizY) { //se os pais forem diferentes (nao pertencem a mesma arvore) faz a uniao
            unir(pai, raizX, raizY);
            total += grafo->arestas[i].distancia; //soma o valor da aresta ao total
        }
    }

    free(pai); //libera memoria do vetor de pais
    
    return total; //retorna o valor total da arvore geradora minima
}

/*funcao para criar o grafo*/
void criaGrafo(GRAFO *grafo, int numVertices) {
    grafo->numVertices = numVertices;
    grafo->vertices = (PONT *)malloc(numVertices * sizeof(PONT)); //aloca memoria para o vetor de vertices
    grafo->arestas = (ARESTA *)malloc(numVertices * ((numVertices - 1) / 2) * sizeof(ARESTA)); //aloca memória pro vetor de arestas (vertices * vertices)
    grafo->numArestas = 0; //inicializa o numero de arestas com 0

    //aloca memória para cada vertice pois vertices é um vetor de ponteiros
    for (int i = 0; i < numVertices; i++) {
        grafo->vertices[i] = (PONT)malloc(sizeof(VERTICE));
    }
}

/*funcao para calcular a distancia entre todos os vertices para realizar o kruskal*/
void calcularDistancia(GRAFO *grafo) {
    for (int i = 0; i < grafo->numVertices; i++) {
        for (int j = i + 1; j < grafo->numVertices; j++) {
            double dx = grafo->vertices[i]->x - grafo->vertices[j]->x; //xb - xa
            double dy = grafo->vertices[i]->y - grafo->vertices[j]->y; //yb - ya
            double distancia = sqrt(pow(dx, 2) + pow(dy, 2)); //raiz de (xb - xa)² + (yb - ya)²
            
            grafo->arestas[grafo->numArestas].origem = i; //define o vertice de origem da aresta
            grafo->arestas[grafo->numArestas].destino = j; //define o vertice de destino da aresta
            grafo->arestas[grafo->numArestas].distancia = distancia; //define a distancia como a distancia calculada
            grafo->numArestas++; //aumenta o numero de arestas
        }
    }
}


int main() {
    int C, x, y;
    printf("insira o numero de casos de teste\n");
    scanf("%d", &C);

    for(int i = 0; i < C; i++) {
        int n;
        printf("insira a quantidade de pessoas (ate 500)\n");
        scanf("%d", &n);
        if (n > 500 || n <= 0){
            printf("invalido\n");
            return 1;
        }
        GRAFO grafo;
        criaGrafo(&grafo, n);
        printf("insira x e y de cada pessoa em cm\n");
        for (int i = 0; i < n; i++) {
            printf("pessoa %d: \n", i + 1);
            scanf("%d %d", &x, &y);
            if (x >= 0 && x <= 10000 && y >= 0 && y <= 10000){
                grafo.vertices[i]->x = x;
                grafo.vertices[i]->y = y;
            }
            else {
                printf("valores invalidos, insira novamente\n");
                i--;
            }

        }

        calcularDistancia(&grafo);
        double resultado = kruskal(&grafo);
        
        printf("distancia minima em m: %.2f\n", resultado / 100.0);

        for (int i = 0; i < grafo.numVertices; i++)
        {
            free(grafo.vertices[i]);
        }
        free(grafo.vertices);
        free(grafo.arestas);
    }

    return 0;
}
