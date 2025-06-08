#include "expressao.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void testarExpressao(int teste, char *posFixa, char *infixa, float valorEsperado) {
    // Teste A: Conversão infixa->posfixa
    char *infixa_dup = strdup(infixa);
    char *convertidaPos = getFormaPosFixa(infixa_dup);
    printf("Teste %d - A: Infixa->PosFixa\n", teste);
    printf("  Esperado: %s\n", posFixa);
    printf("  Obtido:   %s\n", convertidaPos);
    free(infixa_dup);
    
    // Teste B: Conversão posfixa->infixa
    char *posFixa_dup = strdup(posFixa);
    char *convertidaInf = getFormaInFixa(posFixa_dup);
    printf("Teste %d - B: PosFixa->Infixa\n", teste);
    printf("  Esperado: %s\n", infixa);
    printf("  Obtido:   %s\n", convertidaInf);
    free(posFixa_dup);
    
    // Teste C: Avaliação posfixa
    posFixa_dup = strdup(posFixa);
    float valorPosFixa = getValorPosFixa(posFixa_dup);
    printf("Teste %d - C: Valor PosFixa\n", teste);
    printf("  Esperado: %.2f\n", valorEsperado);
    printf("  Obtido:   %.2f\n", valorPosFixa);
    free(posFixa_dup);
    
    // Teste D: Avaliação infixa
    infixa_dup = strdup(infixa);
    float valorInFixa = getValorInFixa(infixa_dup);
    printf("Teste %d - D: Valor Infixa\n", teste);
    printf("  Esperado: %.2f\n", valorEsperado);
    printf("  Obtido:   %.2f\n\n", valorInFixa);
    free(infixa_dup);
}

void testeAutomatico() {
    printf("=== TESTES AUTOMÁTICOS ===\n");
    testarExpressao(1, "3 4 + 5 *", "(3 + 4) * 5", 35);
    testarExpressao(2, "7 2 * 4 +", "7 * 2 + 4", 18);
    testarExpressao(3, "8 5 2 4 + * +", "8 + (5 * (2 + 4))", 38);
    testarExpressao(4, "6 2 / 3 + 4 *", "(6 / 2 + 3) * 4", 24);
    testarExpressao(5, "9 5 2 8 * 4 + * +", "9 + (5 * (2 + 8 * 4))", 109);
    testarExpressao(6, "2 3 + log 5 /", "log(2 + 3) / 5", 0.14f);
    testarExpressao(7, "10 log 3 ^ 2 +", "(log10)^3 + 2", 3);
    testarExpressao(8, "45 60 + 30 cos *", "(45 + 60) * cos(30)", 90.93f);
    testarExpressao(9, "0.5 45 sen 2 ^ +", "sen(45) ^2 + 0.5", 1);
    printf("==========================\n\n");
}

void modoManual() {
    int opcao;
    char entrada[512];
    
    while(1) {
        printf("\n=== MODO MANUAL ===\n");
        printf("1. Converter infixa para posfixa\n");
        printf("2. Converter posfixa para infixa\n");
        printf("3. Calcular valor de expressão infixa\n");
        printf("4. Calcular valor de expressão posfixa\n");
        printf("5. Voltar\n");
        printf("Escolha: ");
        scanf("%d", &opcao);
        getchar(); // Limpar buffer
        
        if(opcao == 5) break;
        
        printf("Digite a expressao: ");
        fgets(entrada, sizeof(entrada), stdin);
        entrada[strcspn(entrada, "\n")] = '\0'; // Remover \n
        
        char *copia = strdup(entrada);
        
        switch(opcao) {
            case 1: {
                char *resultado = getFormaPosFixa(copia);
                printf("\nExpressao infixa: %s", entrada);
                printf("\nForma posfixa: %s\n", resultado);
                break;
            }
            case 2: {
                char *resultado = getFormaInFixa(copia);
                printf("\nExpressao posfixa: %s", entrada);
                printf("\nForma infixa: %s\n", resultado);
                break;
            }
            case 3: {
                float valor = getValorInFixa(copia);
                printf("\nExpressao infixa: %s", entrada);
                printf("\nValor: %.4f\n", valor);
                break;
            }
            case 4: {
                float valor = getValorPosFixa(copia);
                printf("\nExpressao posfixa: %s", entrada);
                printf("\nValor: %.4f\n", valor);
                break;
            }
            default:
                printf("Opcao invalida!\n");
        }
        
        free(copia);
    }
}

int main() {
    int opcao;
    
    while(1) {
        printf("\n=== AVALIADOR DE EXPRESSOES ===\n");
        printf("1. Executar testes automaticos\n");
        printf("2. Modo manual\n");
        printf("3. Sair\n");
        printf("Escolha: ");
        if (scanf("%d", &opcao) != 1) {
            // Limpar buffer em caso de erro
            while (getchar() != '\n');
            printf("Entrada invalida!\n");
            continue;
        }
        getchar(); // Limpar buffer
        
        switch(opcao) {
            case 1:
                testeAutomatico();
                break;
            case 2:
                modoManual();
                break;
            case 3:
                printf("Saindo...\n");
                return 0;
            default:
                printf("Opcao invalida!\n");
        }
    }
}