// main.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "expressao.h"

int main(void) {
    char tipo[8];
    char linha[512];
    while (1) {
        printf("Digite o tipo de expressão ([i]nfixa, [p]osfixa ou vazio para sair): ");
        if (!fgets(tipo, sizeof tipo, stdin) || tipo[0]=='\n') break;
        // remove \n e lower
        tipo[strcspn(tipo, "\n")] = '\0';
        for (char *p = tipo; *p; ++p) *p = tolower((unsigned char)*p);

        if (tipo[0] == 'i') {
            printf("Digite expressão infixa: ");
            if (!fgets(linha, sizeof linha, stdin) || linha[0]=='\n') break;
            linha[strcspn(linha, "\n")] = '\0';
            char *pos = getFormaPosFixa(linha);
            char *inf = getFormaInFixa(pos);
            float vPos = getValorPosFixa(pos);
            float vInf = getValorInFixa(inf);
            printf("Infixa convertida: %s\n", inf);
            printf("Posfixa convertida: %s\n", pos);
            printf("Valor (infixa): %.6f\n", vInf);
            printf("Valor (posfixa): %.6f\n", vPos);
            free(pos); free(inf);
        } else if (tipo[0] == 'p') {
            printf("Digite expressão posfixa: ");
            if (!fgets(linha, sizeof linha, stdin) || linha[0]=='\n') break;
            linha[strcspn(linha, "\n")] = '\0';
            char *inf = getFormaInFixa(linha);
            float vPos = getValorPosFixa(linha);
            float vInf = getValorInFixa(inf);
            char *posRecon = getFormaPosFixa(inf);
            printf("Infixa convertida: %s\n", inf);
            printf("Posfixa (reconstruída): %s\n", posRecon);
            printf("Valor (posfixa): %.6f\n", vPos);
            printf("Valor (infixa): %.6f\n", vInf);
            free(inf); free(posRecon);
        } else {
            printf("Tipo inválido: use 'i' ou 'p'.\n");
        }
        printf("\n");
    }
    return 0;
}
