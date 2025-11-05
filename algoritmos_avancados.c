#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TAMANHO_HASH 10

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

struct HashNode {
    char pistaChave[150];
    char suspeitoValor[100];
    struct HashNode* proximo;
};
typedef struct HashNode HashNode;

HashNode* tabelaHash[TAMANHO_HASH];


Sala* criarSala(char* nome, char* pista) {
    Sala* sala = (Sala*)malloc(sizeof(Sala));
    if (sala == NULL) {
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

void liberarMansao(Sala* sala) {
    if (sala == NULL) {
        return;
    }
    liberarMansao(sala->esquerda);
    liberarMansao(sala->direita);
    free(sala);
}

PistaNode* criarNodePista(char* textoPista) {
    PistaNode* node = (PistaNode*)malloc(sizeof(PistaNode));
    if (node == NULL) {
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

void liberarPistas(PistaNode* raiz) {
    if (raiz == NULL) {
        return;
    }
    liberarPistas(raiz->esquerda);
    liberarPistas(raiz->direita);
    free(raiz);
}

int hash(char* pista) {
    unsigned long hashVal = 0;
    for (int i = 0; pista[i] != '\0'; i++) {
        hashVal += pista[i];
    }
    return hashVal % TAMANHO_HASH;
}

void inicializarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        tabelaHash[i] = NULL;
    }
}

void inserirNaHash(char* pista, char* suspeito) {
    int indice = hash(pista);
    
    HashNode* novoNode = (HashNode*)malloc(sizeof(HashNode));
    if (novoNode == NULL) {
        exit(1);
    }
    
    strcpy(novoNode->pistaChave, pista);
    strcpy(novoNode->suspeitoValor, suspeito);
    
    novoNode->proximo = tabelaHash[indice];
    tabelaHash[indice] = novoNode;
}

char* encontrarSuspeito(char* pista) {
    int indice = hash(pista);
    
    HashNode* atual = tabelaHash[indice];
    while (atual != NULL) {
        if (strcmp(atual->pistaChave, pista) == 0) {
            return atual->suspeitoValor;
        }
        atual = atual->proximo;
    }
    
    return NULL;
}

void liberarHash() {
    for (int i = 0; i < TAMANHO_HASH; i++) {
        HashNode* atual = tabelaHash[i];
        while (atual != NULL) {
            HashNode* temp = atual;
            atual = atual->proximo;
            free(temp);
        }
        tabelaHash[i] = NULL;
    }
}

void preencherAssociacoesHash() {
    inserirNaHash("Carta de ameaça amassada.", "Mordomo Regis");
    inserirNaHash("Faca suspeita na gaveta.", "Cozinheira Berta");
    inserirNaHash("Pegada de bota tamanho 42 na terra.", "Jardineiro Tom");
    inserirNaHash("Lata de veneno aberta.", "Cozinheira Berta");
    inserirNaHash("Garrafa de vinho vazia, tipo raro.", "Mordomo Regis");
    inserirNaHash("Partitura rasgada.", "Sobrinha Clara");
    inserirNaHash("Um prato quebrado com um monograma 'C'.", "Sobrinha Clara");
    inserirNaHash("Um livro de códigos aberto na página 5.", "Jardineiro Tom");
}


void explorarSalas(Sala* inicio, PistaNode** pRaizPistas) {
    if (inicio == NULL) {
        return;
    }

    Sala* atual = inicio;
    char escolha;

    while (1) {
        printf("\nVoce esta em: %s\n", atual->nome);

        if (atual->pista[0] != '\0') {
            printf("Você encontrou uma pista: %s\n", atual->pista);
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
        printf(" (s) Sair e fazer acusação\n");
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
            printf("Você decide analisar as pistas...\n");
            break; 
        } else {
            printf("Opção invalida.\n");
        }
    }
}

int contarPistasParaSuspeito(PistaNode* pistasRaiz, char* suspeitoAcusado) {
    if (pistasRaiz == NULL) {
        return 0;
    }

    int contagem = 0;
    char* suspeitoDaPista = encontrarSuspeito(pistasRaiz->pista);

    if (suspeitoDaPista != NULL && strcmp(suspeitoDaPista, suspeitoAcusado) == 0) {
        contagem = 1;
    }

    contagem += contarPistasParaSuspeito(pistasRaiz->esquerda, suspeitoAcusado);
    contagem += contarPistasParaSuspeito(pistasRaiz->direita, suspeitoAcusado);
    
    return contagem;
}

void verificarSuspeitoFinal(PistaNode* pistasRaiz) {
    printf("\n========================================\n");
    printf("Pistas Coletadas:\n");
    printf("========================================\n");

    if (pistasRaiz == NULL) {
        printf("Nenhuma pista foi coletada.\n");
    } else {
        exibirPistas(pistasRaiz);
    }
    
    printf("\n========================================\n");
    printf("Hora da Acusação\n");
    printf("Suspeitos: Mordomo Regis, Cozinheira Berta, Jardineiro Tom, Sobrinha Clara\n");
    printf("Quem é o culpado? ");

    char acusado[100];
    scanf(" %[^\n]", acusado);

    int contagem = contarPistasParaSuspeito(pistasRaiz, acusado);

    printf("\n--- O Desfecho ---\n");
    if (contagem >= 2) {
        printf("Correto! Voce coletou %d pistas conclusivas contra %s.\n", contagem, acusado);
        printf("O caso esta resolvido! Você venceu!\n");
    } else {
        printf("Incorreto. Você so tinha %d pista(s) contra %s.\n", contagem, acusado);
        printf("As provas eram insuficientes. O verdadeiro culpado escapou...\n");
    }
}

int main() {
    printf("Iniciando Detetive Quest Nivel Mestre...\n");

    Sala* mansaoRaiz = criarSala("Hall de Entrada", NULL);
    PistaNode* pistasRaiz = NULL;
    
    inicializarHash();
    preencherAssociacoesHash();

    mansaoRaiz->esquerda = criarSala("Biblioteca", "Um livro de códigos aberto na página 5.");
    mansaoRaiz->direita = criarSala("Sala de Jantar", "Um prato quebrado com um monograma 'C'.");

    mansaoRaiz->esquerda->esquerda = criarSala("Escritorio Secreto", "Carta de ameaca amassada.");
    mansaoRaiz->esquerda->direita = criarSala("Sala de Música", "Partitura rasgada.");

    mansaoRaiz->direita->esquerda = criarSala("Cozinha", "Faca suspeita na gaveta.");
    mansaoRaiz->direita->direita = criarSala("Jardim de Inverno", "Pegada de bota tamanho 42 na terra.");
    
    mansaoRaiz->esquerda->direita->esquerda = criarSala("Adega", "Garrafa de vinho vazia, tipo raro.");
    mansaoRaiz->direita->esquerda->direita = criarSala("Despensa", "Lata de veneno aberta.");

    
    explorarSalas(mansaoRaiz, &pistasRaiz);

    verificarSuspeitoFinal(pistasRaiz);
    
    liberarMansao(mansaoRaiz);
    liberarPistas(pistasRaiz);
    liberarHash();
    
    printf("\nFim do jogo.\n");

    return 0;
}
