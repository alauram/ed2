#include <stdio.h>
#include <stdlib.h>

#define MAX 4
#define MIN 2
#define true 1 
#define false 0  
typedef int TIPOCHAVE;
typedef int bool;

typedef struct aux {
    TIPOCHAVE item[MAX + 1];  // vetor que armazena as chaves, com espaço extra para insercoes
    int contador;  // numero de chaves atualmente no nó
    struct aux *filhos[MAX + 2];  // ponteiros para os filhos do nó, com espaço extra
} NO, *PONT;  // define a estrutura do nó e o tipo ponteiro para o no

/*inicializa a arvore, setando o ponteiro da raiz como NULL*/
void inicializar(PONT *arvore) {
    *arvore = NULL;  // a arvore começa vazia
}

/*funçao para inserir uma chave em uma pagina (no)*/
void inserirpagina(PONT aux, TIPOCHAVE ch, PONT direita) {
    int i = aux->contador;  // começa pela ultima posiçao ocupada

    // desloca as chaves e filhos para a direita para abrir espaço para a nova chave
    while (i > 0 && ch < aux->item[i - 1]) {
        aux->item[i] = aux->item[i - 1];  // move a chave para a frente
        aux->filhos[i + 1] = aux->filhos[i];  // move o ponteiro do filho para a frente
        i--;  // decrementa i
    }

    // insere a nova chave e o filho à direita
    aux->item[i] = ch;
    aux->filhos[i + 1] = direita;  // aponta o novo filho à direita
    aux->contador++;  // incrementa o numero de chaves no nó
}

/*funçao auxiliar para inserir uma chave em um nó da arvore*/
bool inseriraux(TIPOCHAVE ch, PONT aux, TIPOCHAVE *ch1, PONT *aux1) {
    int i = 0;
    PONT temp;

    // se o no for nulo, significa que a chave deve ser inserida
    if (aux == NULL) {
        *ch1 = ch;  // a nova chave é armazenada em ch1
        *aux1 = NULL;  // não há filhos
        return true;  // retorno indicando que precisa inserir
    }

    // encontra a posiçao correta para a chave dentro do nó
    while (i < aux->contador && ch > aux->item[i]) {
        i++;  // avança enquanto nao encontra a posiçao correta
    }

    // se a chave ja existir no nó, avisa e retorna false
    if (i < aux->contador && ch == aux->item[i]) {
        printf("ja existe\n");
        return false;
    }

    // recursivamente chama a funçao para descer a arvore
    if (!(inseriraux(ch, aux->filhos[i], ch1, aux1))) {
        return false;
    }

    // se o nó nao estiver cheio, insere a chave
    if (aux->contador < MAX) {
        inserirpagina(aux, *ch1, *aux1);  // insere a chave no nó
        return false;  // nao precisa dividir
    }

    // se o no estiver cheio, realiza uma divisao
    temp = (PONT)malloc(sizeof(NO));  // aloca memoria para um nóvo no
    temp->contador = 0;  // novo no começa com zero chaves
    temp->filhos[0] = NULL;  // inicializa o primeiro filho como nulo

    // verifica se a insercao deve ser feita na parte esquerda ou direita
    if (i <= MIN) {
        inserirpagina(temp, aux->item[MAX - 1], aux->filhos[MAX]);  // insere a chave na nova pagina
        aux->contador--;  // reduz o contador do nó original
        inserirpagina(aux, *ch1, *aux1);  // insere a chave na pagina atual
    } else {
        inserirpagina(temp, *ch1, *aux1);  // insere no nó novo
    }

    // move metade das chaves para o novo no
    for (int j = MIN + 1; j < MAX; j++) {
        inserirpagina(temp, aux->item[j], aux->filhos[j + 1]);  // move as chaves
    }

    aux->contador = MIN;  // ajusta o numero de chaves no nó atual
    temp->filhos[0] = aux->filhos[MIN + 1];  // ajusta o filho do nóvo no
    *ch1 = aux->item[MIN];  // define a chave que vai subir
    *aux1 = temp;  // o novo no que foi criado

    return true;  // precisa dividir
}

/*funçao principal para inserir uma chave na arvore*/
void inserir(TIPOCHAVE ch, PONT *aux) {
    TIPOCHAVE ch1;
    PONT aux1, temp;

    // se a chave precisar ser inserida, cria uma nova raiz
    if (inseriraux(ch, *aux, &ch1, &aux1)) {
        temp = (PONT)malloc(sizeof(NO));  // aloca memoria para o novo no
        temp->contador = 1;  // o novo no vai ter uma chave
        temp->item[0] = ch1;  // coloca a chave no nóvo no
        temp->filhos[1] = aux1;  // define o filho à direita
        temp->filhos[0] = *aux;  // o filho à esquerda e o no original
        *aux = temp;  // atualiza a arvore com a nova raiz
    }
}

/*funçao para mover uma chave de um nó da esquerda para o nó da direita*/
void moverdireita(PONT pai, int pos, PONT esquerda, PONT direita) {
    for (int i = direita->contador; i > 0; i--) {
        direita->item[i] = direita->item[i - 1];  // move as chaves na direita
        direita->filhos[i + 1] = direita->filhos[i];  // move os filhos na direita
    }
    direita->filhos[1] = direita->filhos[0];  // ajusta o ponteiro do primeiro filho
    direita->item[0] = pai->item[pos];  // move a chave do pai para a direita
    direita->filhos[0] = esquerda->filhos[esquerda->contador];  // ajusta os filhos da esquerda
    pai->item[pos] = esquerda->item[esquerda->contador - 1];  // move a chave da esquerda para o pai
    esquerda->contador--;  // decrementa o contador da esquerda
    direita->contador++;  // incrementa o contador da direita
}

/*funçao para mover uma chave de um nó da direita para o no da esquerda*/
void moveresquerda(PONT pai, int pos, PONT direita, PONT esquerda) {
    esquerda->item[esquerda->contador] = pai->item[pos];  // move a chave do pai para a esquerda
    esquerda->filhos[esquerda->contador + 1] = direita->filhos[0];  // ajusta o ponteiro do filho
    pai->item[pos] = direita->item[0];  // move a chave da direita para o pai

    for (int i = 0; i < direita->contador - 1; i++) {
        direita->item[i] = direita->item[i + 1];  // desloca as chaves na direita
        direita->filhos[i] = direita->filhos[i + 1];  // desloca os filhos na direita
    }

    direita->filhos[direita->contador - 1] = direita->filhos[direita->contador];  // ajusta o ultimo filho
    direita->contador--;  // decrementa o contador da direita
    esquerda->contador++;  // incrementa o contador da esquerda
}


/*funçao para fundir dois nos*/
void fundir(PONT pai, int pos) {
    PONT esquerda = pai->filhos[pos];  // pega o no a esquerda
    PONT direita = pai->filhos[pos + 1];  // pega o no a direita
    esquerda->item[MIN] = pai->item[pos];  // move a chave do pai para o no da esquerda
    
    // move todas as chaves e filhos do nó da direita para o no da esquerda
    for (int i = 0; i < direita->contador; i++) {
        esquerda->item[i + MIN + 1] = direita->item[i];  // move a chave
        esquerda->filhos[i + MIN + 1] = direita->filhos[i];  // move o filho
    }
    
    // ajusta o ponteiro do ultimo filho
    esquerda->filhos[direita->contador + MIN + 1] = direita->filhos[direita->contador];
    esquerda->contador += direita->contador + 1;  // incrementa o contador de chaves no nó da esquerda

    // ajusta as chaves e filhos no pai apos a fusao
    for (int i = pos; i < pai->contador - 1; i++) {
        pai->item[i] = pai->item[i + 1];  // move as chaves no pai
        pai->filhos[i + 1] = pai->filhos[i + 2];  // ajusta os filhos no pai
    }
    pai->contador--;  // decrementa o numero de chaves no pai

    free(direita);  // libera a memoria ocupada pelo no da direita
}

/*funçao auxiliar para remover uma chave da arvore*/
void removeraux(PONT arvore, TIPOCHAVE chave, int ordem) {
    int i = 0;
    
    // encontra a posiçao onde a chave pode estar
    while (i < arvore->contador && chave > arvore->item[i]) {
        i++;  // avança ate encontrar a posiçao correta
    }

    // se a chave foi encontrada no nó atual
    if (i < arvore->contador && chave == arvore->item[i]) {
        // se o no atual for uma folha, remove a chave diretamente
        if (arvore->filhos[0] == NULL) {
            for (int j = i; j < arvore->contador - 1; j++) {
                arvore->item[j] = arvore->item[j + 1];  // desloca as chaves
            }
            arvore->contador--;  // decrementa o contador de chaves
        } else {
            // se nao for uma folha, precisamos buscar o predecessor ou sucessor
            PONT esquerda = arvore->filhos[i];  // pega o filho a esquerda
            PONT direita = arvore->filhos[i + 1];  // pega o filho a direita

            // verifica se pode remover substituindo pelo predecessor ou sucessor
            if (esquerda->contador >= ordem) {
                TIPOCHAVE predecessor = esquerda->item[esquerda->contador - 1];  // encontra o predecessor
                arvore->item[i] = predecessor;  // substitui pela chave do predecessor
                removeraux(esquerda, predecessor, ordem);  // remove o predecessor
            } else if (direita->contador >= ordem) {
                TIPOCHAVE sucessor = direita->item[0];  // encontra o sucessor
                arvore->item[i] = sucessor;  // substitui pela chave do sucessor
                removeraux(direita, sucessor, ordem);  // remove o sucessor
            } else {
                // se nenhum dos dois tiver chaves suficientes, funde os nos
                fundir(arvore, i);
                removeraux(esquerda, chave, ordem);  // continua a remocao no nó fundido
            }
        }
    } else {
        // se nao encontrou a chave e o nó atual for uma folha
        if (arvore->filhos[0] == NULL) {
            printf("chave nao encontrada na arvore.\n");
            return;
        }

        PONT proximo = arvore->filhos[i];  // desce para o filho correto

        // se o proximo nó estiver com o numero minimo de chaves, realiza ajuste
        if (proximo->contador == ordem - 1) {
            PONT esquerdairmao = (i > 0) ? arvore->filhos[i - 1] : NULL;  // irmao a esquerda
            PONT direitairmao = (i < arvore->contador) ? arvore->filhos[i + 1] : NULL;  // irmao a direita

            // tenta mover chaves dos irmaos ou fundir os nos
            if (esquerdairmao != NULL && esquerdairmao->contador >= ordem) {
                moverdireita(arvore, i - 1, esquerdairmao, proximo);  // move chave da esquerda para direita
            } else if (direitairmao != NULL && direitairmao->contador >= ordem) {
                moveresquerda(arvore, i, direitairmao, proximo);  // move chave da direita para esquerda
            } else {
                if (esquerdairmao != NULL) {
                    fundir(arvore, i - 1);  // funde com o irmao da esquerda
                    proximo = esquerdairmao;  // ajusta o ponteiro
                } else {
                    fundir(arvore, i);  // funde com o irmao da direita
                }
            }
        }

        // continua a remocao no proximo no
        removeraux(proximo, chave, ordem);
    }
}

/*funçao principal para remover uma chave da arvore*/
void excluir(TIPOCHAVE chave, PONT *arvore, int ordem) {
    // se a arvore estiver vazia
    if (*arvore == NULL) {
        printf("arvore vazia\n");
        return;
    }
    
    removeraux(*arvore, chave, ordem);  // chama a funçao auxiliar para remocao
    
    // se a raiz ficar sem chaves, ajusta a arvore
    if ((*arvore)->contador == 0) {
        PONT temp = *arvore;
        if ((*arvore)->filhos[0] != NULL) {
            *arvore = (*arvore)->filhos[0];  // a raiz e substituida pelo filho
        } else {
            *arvore = NULL;  // se nao tiver filhos, a arvore fica vazia
        }
        free(temp);  // libera a memoria da raiz
    }
}

/*funçao para buscar uma chave na arvore*/
void busca(TIPOCHAVE ch, PONT arvore) {
    if (arvore == NULL) {
        printf("%d nao encontrado\n", ch);  // se a arvore for vazia ou nao encontrar a chave
        return;
    }

    int i = 0;

    // percorre o no atual procurando a chave
    while (i < arvore->contador && ch > arvore->item[i]) {
        i++;  // avança para a proxima chave
    }

    // se encontrar a chave no nó atual
    if (i < arvore->contador && ch == arvore->item[i]) {
        printf("%d encontrado\n", ch);
        return;
    }

    // se for uma folha e nao encontrou a chave
    if (arvore->filhos[0] == NULL) {
        printf("%d nao encontrado\n", ch);
        return;
    }

    // continua a busca no filho correspondente
    busca(ch, arvore->filhos[i]);
}

/*funçao auxiliar para imprimir a arvore em ordem de niveis*/
void imprimei(PONT p, int nivel) {
    if (p == NULL) {
        return;  // se o no for nulo, retorna
    }
    printf("nivel %d: ", nivel);  // exibe o nivel atual
    
    // exibe todas as chaves do nó atual
    for (int i = 0; i < p->contador; i++) {
        printf("%d ", p->item[i]);
    }
    
    printf("\n");
    nivel++;  // avança para o proximo nivel

    // recursivamente imprime os filhos
    for (int i = 0; i <= p->contador; i++) {
        imprimei(p->filhos[i], nivel);
    }
}

/*funçao principal para imprimir a arvore*/
void imprime(PONT p) { 
    int n = 0;  // nivel inicial
    imprimei(p, n); 
}
int main() {
    PONT arvore;
    int opcao;
    TIPOCHAVE ch;
    inicializar(&arvore);

    while (1) {
        printf("1 - inserir\n");
        printf("2 - excluir\n");
        printf("3 - exibir\n");
        printf("4 - buscar\n");
        printf("5 - sair\n");
        printf("escolha uma opcao: ");
        scanf("%d", &opcao);

        switch (opcao) {
            case 1:
                printf("digite a chave para inserir: ");
                scanf("%d", &ch);
                inserir(ch, &arvore);
                imprime(arvore);
                break;

            case 2:
                printf("digite a chave para excluir: ");
                scanf("%d", &ch);
                excluir(ch, &arvore, MIN + 1);
                imprime(arvore);
                break;

            case 3:
                imprime(arvore);
                printf("\n");
                break;

            case 4:
                printf("digite a chave para buscar: ");
                scanf("%d", &ch);
                busca(ch, arvore);
                break;
            case 5: 
                return 1;
            default:
                printf("invalido\n");
        }
    }
    return 0;
}
