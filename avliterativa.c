/*árvore avl iterativa, aceita apenas o número de nós pré-definidos*/

#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0
#define MAX 100
typedef int TIPOCHAVE;
typedef int bool;

/*criação da estrutura do nó da arvore avl*/
typedef struct aux {
    TIPOCHAVE ch;
    struct aux *direita;
    struct aux *esquerda;
    int altura;
}NO, *PONT;
/*função para criar o novo nó, aloca memória para o novo nó e define 
seus ponteiros para null, altura começa como 0*/
PONT criarNovoNo(TIPOCHAVE ch){
    PONT novoNo = (PONT)malloc(sizeof(NO));
    if (novoNo){
    novoNo->esquerda = NULL;
    novoNo->direita = NULL;
    novoNo->ch = ch;
    novoNo->altura = 0;
    }
    else {
        printf("erro\n");
    }
    return novoNo;
}
/*retorna o maior número entre dois inteiros, facilitando 
comparações entre as alturas de dois nós*/
int maiorAltura (int a, int b){
    return (a > b)? a : b;
}
/*retorna o campo altura da estrutura de um nó*/
int alturaNo(PONT no){
    if (no == NULL){
        return -1;
    }
    else {
        return no->altura;
    }
}
/*calcula o fator de balanceamento de um nó, substraindo 
a altura do filho da esquerda pelo filho da direita, 
utilizado para balancear a árvore*/
int fatorBalanceamento(PONT no){
    if (no){
        return (alturaNo(no->esquerda) - alturaNo(no->direita));
    }
    else {
        return 0;
    }
}
/*rotação a esquerda*/
PONT rotEsq(PONT no){
    PONT a, b;

    a = no->direita; //define ponteiro a 
    b = a->esquerda; // define ponteiro b

    a->esquerda = no; //rotaciona
    no->direita = b; //ajusta o novo filho do nó

    // atualiza alturas
    no->altura = maiorAltura(alturaNo(no->esquerda), alturaNo(no->direita)) + 1;
    a->altura = maiorAltura(alturaNo(a->esquerda), alturaNo(a->direita)) + 1;

    return a; //retorna a nova raiz
}
/*rotação a direita, mesma lógica da rotação a esquerda, apenas espelhada*/
PONT rotDir(PONT no){
    PONT a, b;
    a = no->esquerda;
    b = a->direita;

    a->direita = no;
    no->esquerda = b;

    no->altura = maiorAltura(alturaNo(no->esquerda), alturaNo(no->direita)) + 1;
    a->altura = maiorAltura(alturaNo(a->esquerda), alturaNo(a->direita)) + 1;

    return a;
}
/*encontra o valor mínimo na subárvore, necessário para encontrar o substituto no 
caso de remoção de nós com dois filhos*/
PONT minimo(PONT no){
    PONT aux = no;
    while(aux->esquerda != NULL){
        aux = aux->esquerda; // percorre até o nó mais a esquerda
    }
    return aux;//retorna o nó mínimo
}
/*balanceia todos os nós da árvore iterativamente, percorre em ordem*/
void balancearArvore(PONT *raiz) {
    if (*raiz == NULL) return;

    PONT pilha[MAX]; //pilha para percorrer os nós
    int topo = -1; //inicializa o topo, vazio

    PONT atual = *raiz; // começa pela raiz da árvore
    PONT anterior = NULL; // começa nulo, nó anteriormente visitado

    //percorre a árvore em ordem para balancear cada nó
    while (topo >= 0 || atual != NULL) {
        if (atual != NULL) {
            pilha[++topo] = atual; //empilha o nó atual
            atual = atual->esquerda; //vai para o filho esquerdo
        } else {
            PONT noTopo = pilha[topo]; //pega o nó do topo
            if (noTopo->direita && anterior != noTopo->direita) {
                atual = noTopo->direita; //vai para o filho direito se existir e não tiver sido visitado
            } else { 
                //se nao tiver filho direito pra empilhar
                PONT aux = pilha[topo--]; //desempilha o nó
                aux->altura = maiorAltura(alturaNo(aux->esquerda), alturaNo(aux->direita)) + 1;//atualiza a altura
                int fator = fatorBalanceamento(aux); //calcula o fator de balanceamento
                //balanceia os nós conforme cada caso
                if (fator > 1) {
                    if (fatorBalanceamento(aux->esquerda) >= 0) {
                        // rotação simples à direita LL
                        if (topo >= 0) {
                            PONT pai = pilha[topo];
                            if (pai->esquerda == aux) { // caso o nó seja o filho esquerdo
                                pai->esquerda = rotDir(aux); //rotaciona o nó e posiciona o ponteiro do pai
                            } else {
                                pai->direita = rotDir(aux);
                            }
                        } else {
                            *raiz = rotDir(aux); //se a aux é a raiz, atualiza a raiz para apontar para o novo nó raiz após a rotação à direita em aux.
                        }
                    } else {
                        //rotação dupla LR
                        aux->esquerda = rotEsq(aux->esquerda); //rotação esquerda do filho esquerdo
                        if (topo >= 0) {
                            PONT pai = pilha[topo];
                            if (pai->esquerda == aux) {
                                pai->esquerda = rotDir(aux);
                            } else {
                                pai->direita = rotDir(aux);
                            }
                        } else {
                            *raiz = rotDir(aux);
                        }
                    }
                } else if (fator < -1) {
                    //rotação simples a esquerda RR
                    if (fatorBalanceamento(aux->direita) <= 0) {
                        if (topo >= 0) {
                            PONT pai = pilha[topo];
                            if (pai->esquerda == aux) {
                                pai->esquerda = rotEsq(aux);
                            } else {
                                pai->direita = rotEsq(aux);
                            }
                        } else {
                            *raiz = rotEsq(aux);
                        }
                    } else {
                        //rotação dupla RL
                        aux->direita = rotDir(aux->direita);
                        if (topo >= 0) {
                            PONT pai = pilha[topo];
                            if (pai->esquerda == aux) {
                                pai->esquerda = rotEsq(aux);
                            } else {
                                pai->direita = rotEsq(aux);
                            }
                        } else {
                            *raiz = rotEsq(aux);
                        }
                    }
                }

                anterior = aux; //avança com o nó visitado
            }
        }
    }
}
/*insere um nó na árvore, chamando a função de balanceamento e retornando a nova raiz*/
PONT inserir(PONT raiz, TIPOCHAVE ch) {
    if (raiz == NULL) {
        return criarNovoNo(ch); //se a raiz for nula, cria um novo nó
    }

    PONT atual = raiz; //começa pela raiz

    while (atual != NULL) {
        if (atual->ch > ch) {
            if (atual->esquerda == NULL) {
                //se a chave é menor que a chave do nó atual, cria o nó
                atual->esquerda = criarNovoNo(ch);
                balancearArvore(&raiz); //balanceia a árvore após a inserção
                return raiz; //retorna nova raiz
            }
            atual = atual->esquerda; //caso contrário, avança para esquerda
        } else if (atual->ch < ch) {
            if (atual->direita == NULL) {
                //se a chave é menor que a chave do nó atual, cria o nó
                atual->direita = criarNovoNo(ch);
                balancearArvore(&raiz); //balanceia
                return raiz; //retorna nova raiz
            }
            atual = atual->direita; //caso contrário, avança pra direita
        } else {
            printf("repetido\n"); //caso encontre um nó igual
            return raiz;
        }
    }

    return raiz;
}
/*remove um elemento da árvore, balanceando e retornando nova raiz*/
PONT remover(PONT raiz, TIPOCHAVE ch){
    if (raiz == NULL){
        return raiz;
    }

    PONT atual = raiz;
    PONT pai = NULL;
    PONT aux = NULL;
    //procura o nó a ser removido
    while(atual != NULL && atual->ch != ch){
        pai = atual; //atualiza o pai a cada iteração
        if (atual->ch > ch){
            atual = atual->esquerda; //se o nó a ser encontrado for menor, avança pra esquerda
        }
        else {
            atual = atual->direita; //se o nó a ser encontrado for maior, avança para a direita
        }
    }

    if (atual == NULL){ //caso não encontre o nó
        printf("chave nao encontrada\n");
        return raiz;
    }
    //nó folha
    if (atual->esquerda == NULL && atual->direita == NULL){
        if (atual == raiz){
            free(raiz); //libera a raiz se for o único nó
            return NULL; //retorna nulo para raiz
        }
        //atualiza ponteiros do pai
        if (pai->esquerda == atual){
            pai->esquerda = NULL;
        }
        else {
            pai->direita = NULL;
        }
        free(atual); //libera o nó sem filhos
    }
    //nó com um filho
    else if(atual->esquerda == NULL || atual->direita == NULL){
        //salva o filho do nó a ser removido
        if (atual->esquerda == NULL){
            aux = atual->direita;
        }
        else {
            aux = atual->esquerda;
        }
        if (atual == raiz){
            free(raiz); //libera a raiz se for o único nó
            return aux; //retorna o nó filho como raiz
        }
        //posiciona o filho do nó excluído a direita ou esquerda do pai
        if (pai->esquerda == atual){
            pai->esquerda = aux;
        }
        else {
            pai->direita = aux;
        }
        free(atual); //libera o nó
    }
    //nó com dois filhos
    else {
        PONT substituto = minimo(atual->direita); //encontra o nó substituto
        TIPOCHAVE ch = substituto->ch;
        atual->ch = ch; //substitui o valor
        PONT paiAux = atual;
        PONT aux = atual->direita;
        
        //encontra o nó na subárvore para posicionar os auxiliares
        while (aux->esquerda != NULL) {
            paiAux = aux;
            aux = aux->esquerda;
        }

        if (paiAux != atual) {
            paiAux->esquerda = aux->direita; //caso o nó substituto tenha um filho, seu pai o recebe
        } else {
            atual->direita = aux->direita; //caso o pai seja o atual, ele recebe o filho
        }
            free(aux); //libera o nó substituto
    }

    balancearArvore(&raiz); //balanceia a árvore após a remoção

    return raiz; //retorna a raiz

}
/*busca um elemento na árvore*/
void buscar (PONT raiz, TIPOCHAVE ch){
    if (raiz != NULL){
        PONT aux = raiz;
        //percorre os nós conferindo se precisa ir para subárvore esquerda ou direita
        while(aux != NULL){ 
            if (aux->ch > ch){
                aux = aux->esquerda;
            }
            else if (aux->ch < ch){
                aux = aux->direita;
            }
            else {
                printf("encontrado\n");
                return;
            }
        }

        printf("nao encontrado\n");
    }
}
/*imprime a altura total da árvore*/
void alturaArvore (PONT raiz){
    if (raiz != NULL){
        PONT pilha[MAX];
        int topo = -1;
        int altura = -1;
        pilha[++topo] = raiz;
        //percorre a árvore usando uma pilha pra calcular a altura
        while(topo >= 0){
            PONT atual = pilha[topo--]; //desempilha o nó

            if(atual->altura > altura){
                altura = atual->altura; //se a altura desse nó for maior, atualiza a variável
            }
            if(atual->esquerda != NULL){
                pilha[++topo] = atual->esquerda; //empilha o nó a esquerda
            }
            if (atual->direita != NULL){
                pilha[++topo] = atual->direita;//empilha o nó a direita
            }
        }

        printf("a altura e %d\n", altura);//imprime a altura
    }
}
/*percorre a árvore em pré-ordem*/
void imprimirPreOrdem(PONT raiz){
    if (raiz != NULL){
        PONT pilha[MAX];
        int topo = -1;
        PONT atual = raiz;
        while (1){
            printf("%d ", atual->ch); //imprime o nó atual, iniciando na raiz

            if (atual->direita != NULL){
                pilha[++topo] = atual->direita; //empilha o filho direito
            }
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;//vai pra esquerda
            }
            else {
                if (topo == -1){
                break; // se a pilha estiver vazia, termina
                }
                atual = pilha[topo--];//desempilha o próximo nó caso nao tenha nó a esquerda
            }
        }
    }
}

/*percorre a árvore em ordem*/
void imprimirEmOrdem(PONT raiz) {
    if (raiz != NULL){
        PONT pilha[MAX];
        int topo = -1;

        PONT atual = raiz;
        while (1){
            while (atual != NULL){
                pilha[++topo] = atual; //empilha os nós
                atual = atual->esquerda;
            }

            if (topo == -1){
                break; //se a pilha estiver vazia, termina
            }
            atual = pilha[topo--]; //desempilha o nó atual
            printf("%d ", atual->ch); //empilha o nó atual
            atual = atual->direita; //vai para o filho direito
        }
    }
}

/*percorre a árvore pós-ordem*/
void imprimirPosOrdem(PONT raiz){
    if (raiz != NULL){

        PONT pilha1[MAX];
        PONT pilha2[MAX];
        int topo1 = -1;
        int topo2 = -1;

        PONT atual = raiz;
        pilha1[++topo1] = atual;

        //percorre a árvore usando duas pilhas
        while (topo1 != -1){
            atual = pilha1[topo1--]; //desempilha da pilha1
            pilha2[++topo2] = atual; //empilha na pilha 2

            if (atual->esquerda != NULL){
                pilha1[++topo1] = atual->esquerda; //empilha o filho esquerdo
            }
            if (atual->direita != NULL){
                pilha1[++topo1] = atual->direita; //empilha o filho direito
            }
        }

 
        while (topo2 != -1){
            printf("%d ", pilha2[topo2--]->ch); //imprime o nó empilhado na pilha 2
        }
    }
}

/*apaga árvore*/
void destruirArvore(PONT raiz){
    if (raiz != NULL){
        PONT pilha[MAX];
        int topo = -1;
        pilha[++topo] = raiz;
        PONT atual = raiz;
        while(atual != NULL){
            atual = pilha[topo--]; //desempilha o no atual
            if (atual->esquerda != NULL){
                pilha[++topo] = atual->esquerda; //empilha o filho esquerdo
            }
            if (atual->direita != NULL){
                pilha[++topo] = atual->direita; //empilha o filho direito
            }
            free(atual); //libera o nó 
        }

    }
}
int main(){
    int opcao, valor;
    PONT raiz = NULL;
    while(1){
        printf("1 - Inserir elemento\n");
        printf("2 - Remover elemento\n");
        printf("3 - Buscar elemento\n");
        printf("4 - Imprimir em-ordem, pre-ordem e pos-ordem, respectivamente\n");
        printf("5 - Calcular altura da arvore\n");
        printf("6 - Sair\n");

        printf("Insira a opcao\n");
        scanf("%d", &opcao);

        switch(opcao){
            case 1: 
            printf("digite o valor a ser inserido\n");
            scanf("%d", &valor);
            raiz = inserir(raiz, valor);
            break;
            case 2:
            printf("digite o valor a ser removido\n");
            scanf("%d", &valor);
            raiz = remover(raiz, valor);
            break;
            case 3:
            printf("insira o valor a ser buscado\n");
            scanf("%d", &valor);
            buscar(raiz, valor);
            break;
            case 4:
            imprimirEmOrdem(raiz);
            printf("\n");
            imprimirPreOrdem(raiz);
            printf("\n");
            imprimirPosOrdem(raiz);
            printf("\n");
            break;
            case 5:
            alturaArvore(raiz);
            break;
            case 6:
            destruirArvore(raiz);
            return 1;
            default:
            printf("invalido\n");
            break;
        }
    }

    return 0;
}