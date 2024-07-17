#include <stdio.h>
#include <stdlib.h>
#define true 1
#define false 0
typedef int bool;

/*criacao da estrutura das tarefas*/
typedef struct {
    int id;
    int urgencia;
    int tempoConclusao;
} TAREFAS;

/*criacao da estrutura do heap*/
typedef struct {
    TAREFAS *dados;
    int tamanho;
} HEAP;

/*inicializar heap com tamanho 0 e ponteiro para o array nulo*/
void inicializar(HEAP *heap){
    heap->tamanho = 0;
    heap->dados = NULL;
}

/*encontrar pai do elemento de indice i*/
int pai(int i){
    return (i/2);
}

/*encontrar filho esquerdo do elemento de indice i*/
int esq(int i){
    return (i*2);
}

/*encontrar filho direito do elemento de indice i*/
int dir(int i){
    return (i*2+1);
}

/*funcao para manter a propriedade do heap de maximo*/
void subirMax(HEAP *heap, int i){
    int j = pai(i);
    if(j >= 1){
        if(heap->dados[i].urgencia > heap->dados[j].urgencia){//faz a troca se for verdadeiro
            TAREFAS temp = heap->dados[i];
            heap->dados[i] = heap->dados[j];
            heap->dados[j] = temp;
            subirMax(heap, j);//chamada recursiva
        }
    }
}

/*funcao para manter a propriedade do heap de minimo*/
void subirMin(HEAP *heap, int i){
    int j = pai(i);
    if(j >= 1){
        if(heap->dados[i].tempoConclusao < heap->dados[j].tempoConclusao){
            TAREFAS temp = heap->dados[i];
            heap->dados[i] = heap->dados[j];
            heap->dados[j] = temp;
            subirMin(heap, j);
        }
    }
}

/*funcao para descer no heap de maximo*/
void descerMax(HEAP *heap, int i){
    int e = esq(i);
    int d = dir(i);
    int maior = i;

    if(e <= heap->tamanho && heap->dados[e].urgencia > heap->dados[i].urgencia){
        maior = e;
    }
    if(d <= heap->tamanho && heap->dados[d].urgencia > heap->dados[maior].urgencia){
        maior = d;
    }

    if(maior != i){ //faz a troca se verdadeiro
        TAREFAS temp = heap->dados[i];
        heap->dados[i] = heap->dados[maior];
        heap->dados[maior] = temp;
        descerMax(heap, maior); //chamada recursiva
    }
}

/*funcao para descer no heap de minimo*/
void descerMin(HEAP *heap, int i){
    int e = esq(i);
    int d = dir(i);
    int menor = i;

    if(e <= heap->tamanho && heap->dados[e].tempoConclusao < heap->dados[i].tempoConclusao){
        menor = e;
    }
    if(d <= heap->tamanho && heap->dados[d].tempoConclusao < heap->dados[menor].tempoConclusao){
        menor = d;
    }

    if(menor != i){
        TAREFAS temp = heap->dados[i];
        heap->dados[i] = heap->dados[menor];
        heap->dados[menor] = temp;
        descerMin(heap, menor);
    }
}

/*insercao no heap de maximo*/
bool inserirMax(HEAP *heap, TAREFAS novo){
    for (int i = 1; i <= heap->tamanho; i++)
    {
        if(heap->dados[i].id == novo.id){
            return false;//se ja existir um elemento com o id, nao sera inserido, o retorno garantira que tambem nao seja inserida no heap minimo
        }
    }
    
    heap->dados = (TAREFAS *)realloc(heap->dados, sizeof(TAREFAS) * (heap->tamanho + 2)); //realoca o espaço de memoria disponivel
    heap->tamanho++; //aumenta o campo tamanho referente ao numero de itens
    heap->dados[heap->tamanho] = novo; //coloca o novo elemento no final do array

    subirMax(heap, heap->tamanho); //chama a funcao para posicionar o elemento corretamente, mantendo as propriedades

    return true;
}
/*insercao no heap de minimo*/

void inserirMin(HEAP *heap, TAREFAS novo){
    heap->dados = (TAREFAS *)realloc(heap->dados, sizeof(TAREFAS) * (heap->tamanho + 2));
    heap->tamanho++;
    heap->dados[heap->tamanho] = novo;
    
    subirMin(heap, heap->tamanho);
}
/*remover primeiro elemento do heap de maximo*/
void removerMax(HEAP *heap){

    heap->dados[1] = heap->dados[heap->tamanho]; //troca o primeiro elemento que sera removido pelo ultimo elemento do heap
    heap->tamanho--;//diminui o tamanho

    if(heap->tamanho > 0){
        descerMax(heap, 1); //desce no heap, para manter as propriedades
    }

    heap->dados = (TAREFAS*)realloc(heap->dados, sizeof(TAREFAS)*(heap->tamanho + 1)); //realoca para nao "sobrar" espaco na memoria

}

/*remover primeiro elemento do heap de minimo*/
void removerMin(HEAP *heap){
    
    heap->dados[1] = heap->dados[heap->tamanho];
    heap->tamanho--;

    if(heap->tamanho > 0){
        descerMin(heap, 1);
    }

    heap->dados = (TAREFAS*)realloc(heap->dados, sizeof(TAREFAS)*(heap->tamanho + 1));
    
}
/*remove maior elemento do heap maximo e procura o elemento no heap minimo para remove-lo*/
void removerUrgencia(HEAP *heapMax, HEAP *heapMin){
    if(heapMax->tamanho == 0){
        printf("vazio\n");
        return;
    }

    int id = heapMax->dados[1].id;
    removerMax(heapMax);
    for (int i = 1; i <= heapMin->tamanho; i++){ //encontra elemento de mesmo id
        if (heapMin->dados[i].id == id){
            heapMin->dados[i] = heapMin->dados[heapMin->tamanho]; //coloca o ultimo elemento do heap no lugar do elemento a ser apagado
            heapMin->tamanho--; //diminui o tamanho do heap
            if (heapMin->tamanho > 0){
                descerMin(heapMin, 1); //ajusta o heap para manter as propriedades
            }
            heapMin->dados = (TAREFAS *)realloc(heapMin->dados, sizeof(TAREFAS) * (heapMin->tamanho + 1)); //realoca a memoria
            break;
        }
    }
}

/*remove primeiro elemento do heap de minimo e o procura no heap de maximo para remove-lo*/
void removerTempo(HEAP *heapMax, HEAP *heapMin){
    if(heapMax->tamanho == 0){
        printf("vazio\n");
        return;
    }

    int id = heapMin->dados[1].id;
    removerMin(heapMin);
    for (int i = 1; i <= heapMax->tamanho; i++)
    {
        if(heapMax->dados[i].id == id){
            heapMax->dados[i] = heapMax->dados[heapMax->tamanho];
            heapMax->tamanho--;
            if(heapMax->tamanho > 0){
            descerMax(heapMax, 1);
            }
            heapMax->dados = (TAREFAS*)realloc(heapMax->dados, sizeof(TAREFAS)*(heapMax->tamanho + 1));
            break;
        }
    }
}
/*atualiza campo de urgencia de um elemento de id passado pelo usuario*/
void atualizarUrgencia(HEAP *heap, int id, int novo){
    if (heap->tamanho != 0){
        for (int i = 1; i <= heap->tamanho; i++){
            if (heap->dados[i].id == id){
                heap->dados[i].urgencia = novo;
                subirMax(heap, i); //reorganiza o heap
                descerMax(heap, i);
                printf("urgencia atualizada\n");
                return;
            }
        }
    }
    printf("id nao presente no heap\n"); //caso o heap esteja vazio ou o id nao for encontrado
}

/*imprime o primeiro elemento do heap*/
void consultaMaiorMenor(HEAP *heap) {  
    if(heap->tamanho != 0){
    printf("{id: %d, urgencia: %d, tempo conclusao: %d} \n", heap->dados[1].id, heap->dados[1].urgencia, heap->dados[1].tempoConclusao);
    }
    else{
        printf("heap vazio\n");
    }
}
/*insere nos dois heaps de uma vez, garantindo que os ids nao se repitam*/
void inserirHeaps(HEAP *heapMax, HEAP *heapMin, TAREFAS tarefa){
    if (inserirMax(heapMax, tarefa)){ // se conseguir inserir no heap de maximo, ira inserir no heap de minimo
        inserirMin(heapMin, tarefa);
    }
    else{
        printf("id %d ja existe\n", tarefa.id); // caso nao consiga, devido a existencia de um elemento de mesmo id
    }
}

/*insere multiplas tarefas*/
void inserirTarefas(HEAP *heapMax, HEAP *heapMin, TAREFAS *tarefas, int n) {
    for (int i = 0; i < n; i++) {
        inserirHeaps(heapMax, heapMin, tarefas[i]);
    }
}

/*imprime ambos os heaps, exibindo todas as informacoes*/
void imprimir(HEAP *heapMax, HEAP *heapMin) {
    printf("\nheap de maximo: ");
    for (int i = 1; i <= heapMax->tamanho; i++) {
        printf("{id: %d, urgencia: %d, tempo conclusao: %d} ", heapMax->dados[i].id, heapMax->dados[i].urgencia, heapMax->dados[i].tempoConclusao);
    }
    printf("\n");

    printf("\nheap de minimo: ");
    for (int i = 1; i <= heapMin->tamanho; i++) {
        printf("{id: %d, urgencia: %d, tempo de conclusao: %d} ", heapMin->dados[i].id, heapMin->dados[i].urgencia, heapMin->dados[i].tempoConclusao);
    }
    printf("\n");
}
int main() {
    HEAP heapMax;
    HEAP heapMin;
    inicializar(&heapMax);
    inicializar(&heapMin);

    int opcao;


    while (1) {
        printf("\n1 - criar multiplas tarefas\n");
        printf("2 - inserir tarefa\n");
        printf("3 - remover tarefa mais urgente\n");
        printf("4 - remover tarefa menor tempo de conclusao\n");
        printf("5 - atualizar urgencia\n");
        printf("6 - consultar tarefa mais urgente\n");
        printf("7 - consultar tarefa de menor tempo de conclusao\n");
        printf("8 - imprimir heaps\n");
        printf("9 - sair\n");
        printf("\ndigite a opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1: {
                int tarefas;
                printf("digite o numero de tarefas a serem inseridas\n");
                scanf("%d", &tarefas);
                TAREFAS novas[tarefas];
                printf("digite os dados das tarefas, id, urgencia e tempo de conclusao\n");
                for (int i = 0; i < tarefas; i++) {
                    printf("tarefa %d: ", i + 1);
                    scanf("%d %d %d", &novas[i].id, &novas[i].urgencia, &novas[i].tempoConclusao);
                }
                inserirTarefas(&heapMax, &heapMin, novas, tarefas);
                imprimir(&heapMax, &heapMin);
                break;
            }
            case 2: {
                TAREFAS nova;
                printf("digite os dados da tarefa, id, urgencia e tempo de conclusao\n");
                scanf("%d %d %d", &nova.id, &nova.urgencia, &nova.tempoConclusao);
                inserirHeaps(&heapMax, &heapMin, nova);
                break;
            }
            case 3: {
                removerUrgencia(&heapMax, &heapMin);
                printf("tarefa removida\n");
                break;
            }
            case 4: {
                removerTempo(&heapMax, &heapMin);
                printf("tarefa removida\n");
                break;
            }
            case 5: {
                int id, nova;
                printf("digite o id da tarefa e a nova urgencia\n");
                scanf("%d %d", &id, &nova);

                atualizarUrgencia(&heapMax, id, nova);
                break;
            }
            case 6: {
                consultaMaiorMenor(&heapMax);
                break;
            }
            case 7: {
                consultaMaiorMenor(&heapMin);
                break;
            }
            case 8: {
                imprimir(&heapMax, &heapMin);
                break;
            }
            case 9: {
                printf("encerrando programa\n");
                free(heapMax.dados);
                free(heapMin.dados);
                return 1;
            }
            default: {
                printf("opcao invalida\n");
                break;
            }
        }

    }


    return 0;
}