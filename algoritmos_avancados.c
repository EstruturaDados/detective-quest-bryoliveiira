#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct Sala {
    char nome[100];
    struct Sala* esquerda;
    struct Sala* direita;
};

typedef struct Sala Sala;

Sala* criarSala(char* nome) {
    Sala* sala = (Sala*)malloc(sizeof(Sala));
    if (sala == NULL) {
        printf("Erro de memoria!\n");
        exit(1);
    }
    strcpy(sala->nome, nome);
    sala->esquerda = NULL;
    sala->direita = NULL;
    return sala;
}

void explorarSalas(Sala* inicio) {
    if (inicio == NULL) {
        printf("A mansao esta vazia.\n");
        return;
    }

    Sala* atual = inicio;
    char escolha;

    while (1) {
        printf("\nVoce esta em: %s\n", atual->nome);

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
            printf("Opcao invalida.\n");
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

int main() {
    printf("Iniciando Detective Quest...\n");

    Sala* raiz = criarSala("Hall de Entrada");
    raiz->esquerda = criarSala("Biblioteca");
    raiz->direita = criarSala("Sala de Jantar");

    raiz->esquerda->esquerda = criarSala("Escritorio Secreto");
    raiz->esquerda->direita = criarSala("Sala de Musica");

    raiz->direita->esquerda = criarSala("Cozinha");
    raiz->direita->direita = criarSala("Jardim de Inverno");
    
    raiz->esquerda->direita->esquerda = criarSala("Adega");
    raiz->direita->esquerda->direita = criarSala("Despensa");

    printf("Mapa criado. Comece a explorar.\n");
    explorarSalas(raiz);

    liberarMansao(raiz);
    printf("Fim do jogo.\n");

    return 0;
}
