#include "expressao.h"
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <stdbool.h>

// Definir M_PI manualmente se necessário
#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

// Estrutura para armazenar expressão com precedência
typedef struct {
    char expr[256];
    int precedencia;
} Elemento;

// Funções auxiliares
static bool ehOperador(char c) {
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

static bool ehFuncao(const char *token) {
    return strcmp(token, "raiz") == 0 || strcmp(token, "sen") == 0 ||
           strcmp(token, "cos") == 0 || strcmp(token, "tg") == 0 || 
           strcmp(token, "log") == 0;
}

static int precedencia(char op) {
    switch(op) {
        case '+': case '-': return 1;
        case '*': case '/': case '%': return 2;
        case '^': return 3;
        default: return 0;
    }
}

static float aplicarOperador(char op, float a, float b) {
    switch(op) {
        case '+': return a + b;
        case '-': return a - b;
        case '*': return a * b;
        case '/': return a / b;
        case '%': return fmod(a, b);
        case '^': return pow(a, b);
        default: return 0;
    }
}

static float aplicarFuncao(const char *funcao, float a) {
    if (strcmp(funcao, "raiz") == 0) return sqrt(a);
    if (strcmp(funcao, "sen") == 0) return sin(a * M_PI / 180.0);
    if (strcmp(funcao, "cos") == 0) return cos(a * M_PI / 180.0);
    if (strcmp(funcao, "tg") == 0) return tan(a * M_PI / 180.0);
    if (strcmp(funcao, "log") == 0) return log10(a);
    return 0;
}

// Conversão infixa para posfixa
char *getFormaPosFixa(char *infixa) {
    static char saida[512] = "";
    saida[0] = '\0';
    
    char pilha[256];
    int topo = -1;
    
    char *token = strtok(infixa, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && token[1] != '\0') || 
            (token[0] == '.' && isdigit(token[1]))) {
            strcat(saida, token);
            strcat(saida, " ");
        } 
        else if (strcmp(token, "(") == 0) {
            pilha[++topo] = '(';
        } 
        else if (strcmp(token, ")") == 0) {
            while (topo >= 0 && pilha[topo] != '(') {
                char temp[2] = {pilha[topo--], '\0'};
                strcat(saida, temp);
                strcat(saida, " ");
            }
            if (topo >= 0 && pilha[topo] == '(') topo--;
        } 
        else if (ehOperador(token[0])) {
            while (topo >= 0 && precedencia(pilha[topo]) >= precedencia(token[0])) {
                char temp[2] = {pilha[topo--], '\0'};
                strcat(saida, temp);
                strcat(saida, " ");
            }
            pilha[++topo] = token[0];
        } 
        else if (ehFuncao(token)) {
            // Empilha um marcador especial para funções
            pilha[++topo] = 'f';
            strcat(saida, token);
            strcat(saida, " ");
        }
        token = strtok(NULL, " ");
    }
    
    while (topo >= 0) {
        char temp[2] = {pilha[topo--], '\0'};
        strcat(saida, temp);
        strcat(saida, " ");
    }
    
    // Remove espaço extra no final se houver
    if (saida[strlen(saida) - 1] == ' ') {
        saida[strlen(saida) - 1] = '\0';
    }
    return saida;
}

// Conversão posfixa para infixa (melhorada)
char *getFormaInFixa(char *posFixa) {
    static Elemento pilha[256];
    int topo = -1;
    
    char *token = strtok(posFixa, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && token[1] != '\0') || 
            (token[0] == '.' && isdigit(token[1]))) {
            // Elemento número
            Elemento e;
            strcpy(e.expr, token);
            e.precedencia = 10; // Precedência alta
            pilha[++topo] = e;
        } 
        else if (ehOperador(token[0])) {
            // Operador binário
            if (topo < 1) {
                // Erro: não há operandos suficientes
                return "Erro: Expressao invalida";
            }
            Elemento e2 = pilha[topo--];
            Elemento e1 = pilha[topo--];
            Elemento novo;
            int p = precedencia(token[0]);
            
            char expr1[256];
            char expr2[256];
            
            // Verifica se precisa de parênteses na subexpressão esquerda
            if (e1.precedencia < p) {
                sprintf(expr1, "(%s)", e1.expr);
            } else {
                strcpy(expr1, e1.expr);
            }
            
            // Verifica se precisa de parênteses na subexpressão direita
            if (e2.precedencia < p || 
                (e2.precedencia == p && (token[0] == '-' || token[0] == '/' || token[0] == '^'))) {
                sprintf(expr2, "(%s)", e2.expr);
            } else {
                strcpy(expr2, e2.expr);
            }
            
            sprintf(novo.expr, "%s %c %s", expr1, token[0], expr2);
            novo.precedencia = p;
            pilha[++topo] = novo;
        } 
        else if (ehFuncao(token)) {
            // Função unária
            if (topo < 0) {
                return "Erro: Expressao invalida";
            }
            Elemento e = pilha[topo--];
            Elemento novo;
            
            // Funções sempre têm alta precedência
            sprintf(novo.expr, "%s(%s)", token, e.expr);
            novo.precedencia = 10;
            pilha[++topo] = novo;
        }
        token = strtok(NULL, " ");
    }
    
    if (topo != 0) {
        return "Erro: Expressao invalida";
    }
    return pilha[topo].expr;
}

// Avaliação de expressão posfixa
float getValorPosFixa(char *posFixa) {
    float pilha[256];
    int topo = -1;
    
    char *token = strtok(posFixa, " ");
    while (token != NULL) {
        if (isdigit(token[0]) || (token[0] == '-' && token[1] != '\0') || 
            (token[0] == '.' && isdigit(token[1]))) {
            pilha[++topo] = atof(token);
        } 
        else if (ehOperador(token[0])) {
            if (topo < 1) {
                return NAN;
            }
            float b = pilha[topo--];
            float a = pilha[topo--];
            pilha[++topo] = aplicarOperador(token[0], a, b);
        } 
        else if (ehFuncao(token)) {
            if (topo < 0) {
                return NAN;
            }
            float a = pilha[topo--];
            pilha[++topo] = aplicarFuncao(token, a);
        }
        token = strtok(NULL, " ");
    }
    
    if (topo != 0) {
        return NAN;
    }
    return pilha[topo];
}

// Avaliação de expressão infixa
float getValorInFixa(char *infixa) {
    char *infixa_copia = strdup(infixa);
    char *posFixa = getFormaPosFixa(infixa_copia);
    free(infixa_copia);
    
    char *posFixa_copia = strdup(posFixa);
    float resultado = getValorPosFixa(posFixa_copia);
    free(posFixa_copia);
    
    return resultado;
}