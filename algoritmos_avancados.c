#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Sala {
    char nome[100];
    char pista[150];
    struct Sala* esquerda;
    struct Sala* direita;
};
typedef struct Sala Sala;

struct PistaNode {
    char pista[150];
    struct PistaNode* esquerda;
    struct PistaNode* direita;
};
typedef struct PistaNode PistaNode;


Sala* criarSala(char* nome, char* pista) {
    Sala* sala = (Sala*)malloc(sizeof(Sala));
    if (sala == NULL) {
        printf("Erro de memoria!\n");
        exit(1);
    }
    
    strcpy(sala->nome, nome);
    
    if (pista != NULL) {
        strcpy(sala->pista, pista);
    } else {
        sala->pista[0] = '\0';
    }
    
    sala->esquerda = NULL;
    sala->direita = NULL;
    return sala;
}

PistaNode* criarNodePista(char* textoPista) {
    PistaNode* node = (PistaNode*)malloc(sizeof(PistaNode));
    if (node == NULL) {
        printf("Erro de memoria!\n");
        exit(1);
    }
    strcpy(node->pista, textoPista);
    node->esquerda = NULL;
    node->direita = NULL;
    return node;
}

PistaNode* inserirPista(PistaNode* raiz, char* textoPista) {
    if (raiz == NULL) {
        return criarNodePista(textoPista);
    }

    int cmp = strcmp(textoPista, raiz->pista);

    if (cmp < 0) {
        raiz->esquerda = inserirPista(raiz->esquerda, textoPista);
    } else if (cmp > 0) {
        raiz->direita = inserirPista(raiz->direita, textoPista);
    }

    return raiz;
}

void exibirPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    
    exibirPistas(raiz->esquerda);
    printf("- %s\n", raiz->pista);
    exibirPistas(raiz->direita);
}

void explorarSalasComPistas(Sala* inicio, PistaNode** pRaizPistas) {
    if (inicio == NULL) {
        printf("A mansão esta vazia.\n");
        return;
    }

    Sala* atual = inicio;
    char escolha;

    while (1) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (atual->pista[0] != '\0') {
            printf("Voce encontrou uma pista: %s\n", atual->pista);
            *pRaizPistas = inserirPista(*pRaizPistas, atual->pista);
            atual->pista[0] = '\0';
        }

        if (atual->esquerda == NULL && atual->direita == NULL) {
            printf("Fim do caminho.\n");
        }

        printf("Escolha seu caminho:\n");
        if (atual->esquerda != NULL) {
            printf(" (e) Esquerda (%s)\n", atual->esquerda->nome);
        }
        if (atual->direita != NULL) {
            printf(" (d) Direita (%s)\n", atual->direita->nome);
        }
        printf(" (s) Sair\n");
        printf("Opcao: ");

        scanf(" %c", &escolha);

        if (escolha == 'e') {
            if (atual->esquerda != NULL) {
                atual = atual->esquerda;
            } else {
                printf("Caminho bloqueado.\n");
            }
        } else if (escolha == 'd') {
            if (atual->direita != NULL) {
                atual = atual->direita;
            } else {
                printf("Caminho bloqueado.\n");
            }
        } else if (escolha == 's') {
            printf("Saindo da mansao...\n");
            break; 
        } else {
            printf("Opção invalida.\n");
        }
    }
}

void liberarMansao(Sala* sala) {
    if (sala == NULL) {
        return;
    }
    liberarMansao(sala->esquerda);
    liberarMansao(sala->direita);
    free(sala);
}

void liberarPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}


int main() {
    printf("Iniciando Detective Quest Nivel 2...\n");

    Sala* mansaoRaiz = criarSala("Hall de Entrada", NULL);
    PistaNode* pistasRaiz = NULL;

    mansaoRaiz->esquerda = criarSala("Biblioteca", "Um livro de codigos aberto na pagina 5.");
    mansaoRaiz->direita = criarSala("Sala de Jantar", "Um prato quebrado com um monograma 'C'.");

    mansaoRaiz->esquerda->esquerda = criarSala("Escritorio Secreto", "Carta de ameaca amassada.");
    mansaoRaiz->esquerda->direita = criarSala("Sala de Musica", "Partitura rasgada.");

    mansaoRaiz->direita->esquerda = criarSala("Cozinha", "Faca suspeita na gaveta.");
    mansaoRaiz->direita->direita = criarSala("Jardim de Inverno", "Pegada de bota tamanho 42 na terra.");
    
    mansaoRaiz->esquerda->direita->esquerda = criarSala("Adega", "Garrafa de vinho vazia, tipo raro.");
    mansaoRaiz->direita->esquerda->direita = criarSala("Despensa", "Lata de veneno aberta.");

    printf("Mapa criado. Comece a explorar.\n");
    
    explorarSalasComPistas(mansaoRaiz, &pistasRaiz);

    printf("\n========================================\n");
    printf("Pistas Coletadas (em ordem alfabetica):\n");
    printf("========================================\n");

    if (pistasRaiz == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasRaiz);
    }
    
    liberarMansao(mansaoRaiz);
    liberarPistas(pistasRaiz);
    printf("\nFim do jogo.\n");

    return 0;
}
