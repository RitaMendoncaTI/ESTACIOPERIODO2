// war_novato.c
// Nível Novato: cadastro de 5 territórios usando vetor de structs.
// Compilar: gcc war_novato.c -o war_novato

#include <stdio.h>
#include <string.h>

typedef struct {
    char nome[30];
    char cor[10];
    int tropas;
} Territorio;

int main() {
    Territorio mapa[5];
    int i;

    printf("=== WAR - Nível Novato (cadastro de 5 territórios) ===\n");

    // Entrada dos dados dos 5 territórios
    for (i = 0; i < 5; i++) {
        printf("\nTerritório %d\n", i + 1);
        printf("Nome: ");
        // lê string sem espaços (suficiente ao pedido). Se quiser aceitar espaços use fgets+trim.
        scanf("%29s", mapa[i].nome);

        printf("Cor do exército (ex: vermelho, azul): ");
        scanf("%9s", mapa[i].cor);

        printf("Número de tropas: ");
        scanf("%d", &mapa[i].tropas);
    }

    // Exibição dos dados cadastrado
    printf("\n=== Territórios cadastrados ===\n");
    for (i = 0; i < 5; i++) {
        printf("Território %d: Nome: %s | Cor: %s | Tropas: %d\n",
               i + 1, mapa[i].nome, mapa[i].cor, mapa[i].tropas);
    }

    return 0;
}
