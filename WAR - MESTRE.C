// war_mestre.c
// Nível Mestre: missões estratégicas, alocação dinâmica para missão, verificação simples.
// Compilar: gcc war_mestre.c -o war_mestre

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

typedef struct {
    char nome[30];
    char cor[10];
    char* missao; // armazenada dinamicamente
} Jogador;

// Prototypes
Territorio* alocarMapa(int n);
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMapa(Territorio* mapa);
void atribuirMissao(char* destino, char* missoes[], int totalMissoes);
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador);

int main() {
    srand((unsigned int)time(NULL));

    int n;
    printf("=== WAR - Nível Mestre ===\n");
    printf("Quantos territórios deseja cadastrar? ");
    if (scanf("%d", &n) != 1 || n <= 0) {
        printf("Entrada inválida.\n");
        return 1;
    }

    Territorio* mapa = alocarMapa(n);
    if (!mapa) {
        printf("Erro de alocação.\n");
        return 1;
    }

    cadastrarTerritorios(mapa, n);

    // Criar jogadores (ex: 2 jogadores)
    Jogador jogadores[2];
    int i;
    for (i = 0; i < 2; i++) {
        printf("\nJogador %d - Nome: ", i + 1);
        scanf("%29s", jogadores[i].nome);
        printf("Cor do jogador %d: ", i + 1);
        scanf("%9s", jogadores[i].cor);
        jogadores[i].missao = NULL; // será alocada dinamicamente
    }

    // Vetor de missões (mínimo 5)
    char* missoes[] = {
        "Conquistar 3 territorios",
        "Eliminar todas as tropas da cor vermelha",
        "Ter pelo menos 10 tropas no total",
        "Conquistar 1 territorio adversario",
        "Manter pelo menos 2 territorios"
    };
    int totalMissoes = sizeof(missoes) / sizeof(missoes[0]);

    // Alocar espaço para missão de cada jogador e atribuir
    for (i = 0; i < 2; i++) {
        // aloca 100 bytes para a missão do jogador (suficiente para as missões definidas)
        jogadores[i].missao = (char*) malloc(100 * sizeof(char));
        if (!jogadores[i].missao) {
            printf("Erro de alocação para missão.\n");
            // liberar previamente alocado e sair
            for (int j = 0; j < i; j++) free(jogadores[j].missao);
            free(mapa);
            return 1;
        }
        atribuirMissao(jogadores[i].missao, missoes, totalMissoes);
        printf("Missão do %s: %s\n", jogadores[i].nome, jogadores[i].missao);
    }

    int opc, ata, def, jogadorAtual = 0;
    do {
        printf("\nMenu:\n1 - Exibir mapa\n2 - Atacar\n3 - Verificar missões\n0 - Sair\nEscolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                exibirMapa(mapa, n);
                break;
            case 2:
                exibirMapa(mapa, n);
                printf("Escolha índice do atacante (1 a %d): ", n);
                scanf("%d", &ata);
                printf("Escolha índice do defensor (1 a %d): ", n);
                scanf("%d", &def);

                if (ata < 1 || ata > n || def < 1 || def > n || ata == def) {
                    printf("Índices inválidos.\n");
                } else {
                    Territorio* atacante = &mapa[ata - 1];
                    Territorio* defensor = &mapa[def - 1];

                    // validar que o atacante é do jogador atual (cor)
                    if (strcmp(atacante->cor, jogadores[jogadorAtual].cor) != 0) {
                        printf("O território atacante não pertence ao jogador atual (%s).\n", jogadores[jogadorAtual].nome);
                    } else if (strcmp(atacante->cor, defensor->cor) == 0) {
                        printf("Não é permitido atacar território da própria cor.\n");
                    } else {
                        atacar(atacante, defensor);
                        printf("Turno de %s finalizado.\n", jogadores[jogadorAtual].nome);
                        // alterna jogador
                        jogadorAtual = (jogadorAtual + 1) % 2;
                    }
                }
                break;
            case 3:
                // verifica missão de cada jogador
                for (i = 0; i < 2; i++) {
                    int ok = verificarMissao(jogadores[i].missao, mapa, n, jogadores[i].cor);
                    printf("Jogador %s - Missão: %s -> %s\n", jogadores[i].nome, jogadores[i].missao, ok ? "Cumprida" : "Não cumprida");
                    if (ok) {
                        printf("=== Jogador %s VENCEU! ===\n", jogadores[i].nome);
                    }
                }
                break;
            case 0:
                printf("Encerrando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }

    } while (opc != 0);

    // liberar missões e mapa
    for (i = 0; i < 2; i++) free(jogadores[i].missao);
    liberarMapa(mapa);

    return 0;
}

// Funções auxiliares

Territorio* alocarMapa(int n) {
    return (Territorio*) calloc(n, sizeof(Territorio));
}

void cadastrarTerritorios(Territorio* mapa, int n) {
    int i;
    for (i = 0; i < n; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: ");
        scanf("%29s", mapa[i].nome);
        printf("Cor do exército: ");
        scanf("%9s", mapa[i].cor);
        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }
}

void exibirMapa(Territorio* mapa, int n) {
    int i;
    printf("\n--- Mapa (%d territórios) ---\n", n);
    for (i = 0; i < n; i++) {
        printf("%d) %s | Cor: %s | Tropas: %d\n", i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Reaproveita lógica simples de combate do nível aventureiro
void atacar(Territorio* atacante, Territorio* defensor) {
    int rollA = (rand() % 6) + 1;
    int rollD = (rand() % 6) + 1;

    printf("Rolagem: Atacante %d x Defensor %d\n", rollA, rollD);

    if (rollA > rollD) {
        int transferencia = atacante->tropas / 2;
        if (transferencia < 1) transferencia = 1;
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';
        defensor->tropas = transferencia;
        atacante->tropas -= transferencia;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Atacante venceu! %d tropas transferidas ao defensor.\n", transferencia);
    } else {
        if (atacante->tropas > 0) {
            atacante->tropas -= 1;
            printf("Atacante perdeu e perde 1 tropa.\n");
        } else {
            printf("Atacante sem tropas.\n");
        }
    }
}

void liberarMapa(Territorio* mapa) {
    free(mapa);
}

// Sorteia uma missão e copia para 'destino' (destino já deve ter memória alocada)
// Conforme instruções, a missão do jogador é armazenada dinamicamente (caller alocou).
void atribuirMissao(char* destino, char* missoes[], int totalMissoes) {
    if (totalMissoes <= 0) return;
    int idx = rand() % totalMissoes;
    strncpy(destino, missoes[idx], 99);
    destino[99] = '\0';
}

// Verificação simples de missão baseada em palavras-chave:
// - "Conquistar 3 territorios": checar se jogador tem >=3 territórios
// - "Eliminar todas as tropas da cor X": checar se não existe território com cor "vermelha" (hardcoded as example)
// - "Ter pelo menos 10 tropas": soma tropas do jogador >= 10
// - "Conquistar 1 territorio adversario": checar se jogador controla >=1 território (trivial)
// - "Manter pelo menos 2 territorios": jogador tem >=2 territórios
int verificarMissao(char* missao, Territorio* mapa, int tamanho, const char* corJogador) {
    int i;
    if (strstr(missao, "Conquistar 3 territorios") != NULL) {
        int contador = 0;
        for (i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, corJogador) == 0) contador++;
        return contador >= 3;
    } else if (strstr(missao, "Eliminar todas as tropas da cor vermelha") != NULL) {
        for (i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, "vermelha") == 0) return 0;
        return 1; // nenhuma "vermelha" encontrada
    } else if (strstr(missao, "Ter pelo menos 10 tropas") != NULL) {
        int soma = 0;
        for (i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, corJogador) == 0) soma += mapa[i].tropas;
        return soma >= 10;
    } else if (strstr(missao, "Conquistar 1 territorio adversario") != NULL) {
        int contador = 0;
        for (i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, corJogador) == 0) contador++;
        return contador >= 1;
    } else if (strstr(missao, "Manter pelo menos 2 territorios") != NULL) {
        int contador = 0;
        for (i = 0; i < tamanho; i++) if (strcmp(mapa[i].cor, corJogador) == 0) contador++;
        return contador >= 2;
    } else {
        // regra default: não cumprida
        return 0;
    }
}
