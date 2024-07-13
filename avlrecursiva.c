#include <stdio.h>
#include <stdlib.h>

#define true 1
#define false 0

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
/*balanceia o nó da árvore*/
PONT balancear(PONT no) {
    if (no == NULL) { //se o nó for nulo, retorna null
        return no;
    }
    int fator = fatorBalanceamento(no); //calcula o fator de balanceamento do nó

    if (fator > 1) {
        if (fatorBalanceamento(no->esquerda) >= 0) 
            no = rotDir(no); //caso LL: rotação simples à direita
        else {
            no->esquerda = rotEsq(no->esquerda);//caso LR: rotação à esquerda no filho esquerdo
            no = rotDir(no); //depois, rotação à direita no próprio nó
        }
    } else if (fator < -1) {
        if (fatorBalanceamento(no->direita) <= 0)
            no = rotEsq(no);//caso RR: rotação simples à esquerda
        else {
            no->direita = rotDir(no->direita); //caso RL: rotação à direita no filho direito
            no = rotEsq(no);//depois, rotação à esquerda no próprio nó
        }
    }

    return no;//retorna o nó balanceado
}
/*insere um novo nó na posição adequada, maiores a direita, menores a esquerda,
 não aceita repetidos*/
PONT inserir(PONT raiz, TIPOCHAVE ch){
    if (raiz == NULL){
        return criarNovoNo(ch);//cria um novo nó caso a árvore esteja vazia
    }
    else {
        if (ch < raiz->ch){
            raiz->esquerda = inserir(raiz->esquerda, ch);//chama a função novamente para subárvore à esquerda

        }
        else if(ch > raiz->ch){
            raiz->direita = inserir(raiz->direita, ch);//chama a função novamente para subárvore à direita
        }
        else {
            printf("elemento repetido\n");
        }
    }

    raiz->altura = maiorAltura(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) + 1; //ajusta do nó raiz

    raiz = balancear(raiz);//balanceia o nó

    return raiz;//retorna a nova raiz da árvore ou subárvore
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
/*remove um nó da árvore*/
PONT remover(PONT raiz, TIPOCHAVE ch){
    if (raiz == NULL){
        printf("nao existe\n");//se o nó não existe, retorna NULL
        return NULL;
    }
    else{
        if (raiz->ch == ch){
            //nó encontrado
            if (raiz->esquerda == NULL && raiz->direita == NULL){//caso nó folha
                free(raiz); //apenas libera o nó
                return NULL;
            }
            else{
                if (raiz->esquerda == NULL){//nó com filho direito
                    PONT temp = raiz->direita; // váriável temporária para salvar o filho
                    free(raiz);
                    return temp; //retorna o filho do nó excluído
                }
                else if (raiz->direita == NULL){
                    PONT temp = raiz->esquerda; //nó com filho esquerdo
                    free(raiz);
                    return temp;
                }
                //nó com dois filhos
                PONT substituto = minimo(raiz->direita); //encontra o substituto
                raiz->ch = substituto->ch; //substitui o valor
                raiz->direita = remover(raiz->direita, substituto->ch); //chama a função pra remover o substituto
            }
        }
        else{
            if (ch < raiz->ch){
                raiz->esquerda = remover(raiz->esquerda, ch);//procura o nó na subárvore esquerda
            }
            else{
                raiz->direita = remover(raiz->direita, ch);//procura o nó na subárvore direita
            }
        }

        //atualiza a altura do nó
        raiz->altura = maiorAltura(alturaNo(raiz->esquerda), alturaNo(raiz->direita)) + 1;

        //balanceia o nó
        return balancear(raiz);
    }
}
/*calcula a altura total da árvore*/
int alturaArvore(PONT raiz){
    if (raiz == NULL) {
        return -1;
    } else {
        int alturaEsquerda = alturaArvore(raiz->esquerda);//altura da subárvore esquerda
        int alturaDireita = alturaArvore(raiz->direita);//altura da subárvore direita
        return maiorAltura(alturaEsquerda, alturaDireita) + 1;//retorna a altura total
    }
}
/*busca um elemento na arvore*/
void buscar(PONT raiz, TIPOCHAVE ch){
    if (raiz == NULL){
        printf("nao existe\n");//caso não seja encontrado
    }
    else if(raiz->ch > ch){
        buscar(raiz->esquerda, ch);//busca na subárvore esquerda
    }
    else if (raiz->ch < ch){
        buscar(raiz->direita, ch);//busca na subárvore direita
    }
    else {
        printf("encontrado\n");
    }
}
/*percorre a árvore em pré-ordem*/
void imprimirPreOrdem(PONT raiz){
    if(raiz != NULL){
        printf("%d ", raiz->ch);//visita o nó raiz
        imprimirPreOrdem(raiz->esquerda);//percorre a subárvore esquerda
        imprimirPreOrdem(raiz->direita);//percorre a subárvore direita
    }
}
/*percorre a árvore em ordem*/
void imprimirEmOrdem(PONT raiz) {
    if (raiz != NULL) {
        imprimirEmOrdem(raiz->esquerda);//percorre a subárvore esquerda
        printf("%d ", raiz->ch);//visita o nó raiz
        imprimirEmOrdem(raiz->direita);//percorre a subárvore direita
    }
}
/*percorre a árvore pós-ordem*/
void imprimirPosOrdem(PONT raiz){
    if(raiz != NULL){
        imprimirPosOrdem(raiz->esquerda);//percorre a subárvore esquerda
        imprimirPosOrdem(raiz->direita);//percorre a subárvore direita
        printf("%d ", raiz->ch);//visita o nó raiz
    }
}
/*função para apagar a árvore*/
void destruirArvore(PONT raiz){
    if(raiz != NULL){
    destruirArvore(raiz->esquerda);//chama a função para subárvore esquerda
    destruirArvore(raiz->direita);//chama a função para subárvore direita
    free(raiz);//libera a memória do nó atual
    }
}

int main(){
    int opcao, valor;
    PONT raiz = NULL;
    while(1){
        printf("1 - Inserir elemento\n");
        printf("2 - Remover elemento\n");
        printf("3 - Imprimir em-ordem, pre-ordem e pos-ordem, respectivamente\n");
        printf("4 - Procurar elemento\n");
        printf("5 - altura da arvore\n");
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
            imprimirEmOrdem(raiz);
            printf("\n");
            imprimirPreOrdem(raiz);
            printf("\n");
            imprimirPosOrdem(raiz);
            printf("\n");
            break;
            case 4:
            printf("insira o valor a ser encontrado\n");
            scanf("%d", &valor);
            buscar(raiz, valor);
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