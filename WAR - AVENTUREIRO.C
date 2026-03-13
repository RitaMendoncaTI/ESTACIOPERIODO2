// war_aventureiro.c
// Nível Aventureiro: alocação dinâmica do mapa e simulação de ataque.
// Compilar: gcc war_aventureiro.c -o war_aventureiro

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

// Prototypes
Territorio* alocarMapa(int n);
void cadastrarTerritorios(Territorio* mapa, int n);
void exibirMapa(Territorio* mapa, int n);
void atacar(Territorio* atacante, Territorio* defensor);
void liberarMemoria(Territorio* mapa);

int main() {
    int n, opc, ata, def;

    srand((unsigned int)time(NULL));

    printf("=== WAR - Nível Aventureiro ===\n");
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

    do {
        printf("\nMenu:\n1 - Exibir mapa\n2 - Atacar\n0 - Sair\nEscolha: ");
        scanf("%d", &opc);

        switch (opc) {
            case 1:
                exibirMapa(mapa, n);
                break;
            case 2:
                exibirMapa(mapa, n);
                printf("Escolha o índice do território ATACANTE (1 a %d): ", n);
                scanf("%d", &ata);
                printf("Escolha o índice do território DEFENSOR (1 a %d): ", n);
                scanf("%d", &def);

                // validar índices
                if (ata < 1 || ata > n || def < 1 || def > n) {
                    printf("Índices inválidos.\n");
                } else if (ata == def) {
                    printf("Um território não pode atacar ele mesmo.\n");
                } else {
                    Territorio* atacante = &mapa[ata - 1];
                    Territorio* defensor = &mapa[def - 1];
                    // validar cor (não atacar território da própria cor)
                    if (strcmp(atacante->cor, defensor->cor) == 0) {
                        printf("Não é permitido atacar território da própria cor.\n");
                    } else {
                        atacar(atacante, defensor);
                        printf("Resultado do ataque:\n");
                        printf("Atacante: %s | Cor: %s | Tropas: %d\n",
                               atacante->nome, atacante->cor, atacante->tropas);
                        printf("Defensor: %s | Cor: %s | Tropas: %d\n",
                               defensor->nome, defensor->cor, defensor->tropas);
                    }
                }
                break;
            case 0:
                printf("Finalizando...\n");
                break;
            default:
                printf("Opção inválida.\n");
        }
    } while (opc != 0);

    liberarMemoria(mapa);
    return 0;
}

// Aloca memória para n territórios
Territorio* alocarMapa(int n) {
    Territorio* mapa = (Territorio*) calloc(n, sizeof(Territorio));
    return mapa;
}

// Cadastro de territórios usando ponteiros
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

// Exibe o mapa
void exibirMapa(Territorio* mapa, int n) {
    int i;
    printf("\n--- Mapa (%d territórios) ---\n", n);
    for (i = 0; i < n; i++) {
        printf("%d) %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }
}

// Simula ataque: usa rand() como rolagem de dado (1-6) para cada lado.
// Regras:
// - Se atacante vence (roll maior), defensor muda de cor para a cor do atacante
//   e o atacante transfere metade de suas tropas (truncando) para o defensor.
// - Se atacante perde (roll menor ou igual), atacante perde 1 tropa.
void atacar(Territorio* atacante, Territorio* defensor) {
    int rollA = (rand() % 6) + 1;
    int rollD = (rand() % 6) + 1;

    printf("Rolagem: Atacante %d x Defensor %d\n", rollA, rollD);

    if (rollA > rollD) {
        // atacante vence
        int transferencia = atacante->tropas / 2; // metade das tropas do atacante
        if (transferencia < 1) transferencia = 1; // garantir pelo menos 1 transferido quando possível
        // atualizar defensor
        strncpy(defensor->cor, atacante->cor, sizeof(defensor->cor) - 1);
        defensor->cor[sizeof(defensor->cor) - 1] = '\0';
        defensor->tropas = transferencia;
        // reduzir tropas do atacante
        atacante->tropas -= transferencia;
        if (atacante->tropas < 0) atacante->tropas = 0;
        printf("Atacante venceu! %d tropas transferidas ao defensor.\n", transferencia);
    } else {
        // atacante perde
        if (atacante->tropas > 0) {
            atacante->tropas -= 1;
            printf("Atacante perdeu a batalha e perde 1 tropa.\n");
        } else {
            printf("Atacante não tem tropas suficientes para perder.\n");
        }
    }
}

// Libera memória alocada
void liberarMemoria(Territorio* mapa) {
    free(mapa);
}
