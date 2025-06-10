// expressao.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include <limits.h>
#include "expressao.h"

#define MAX_TOKENS 512
#define MAX_TOKEN_LEN 64

static char *stackS[MAX_TOKENS];
static int topS;
static void pushS(char *s) { stackS[++topS] = s; }
static char *popS(void) { return (topS >= 0) ? stackS[topS--] : NULL; }
static char *peekS(void) { return (topS >= 0) ? stackS[topS] : NULL; }

static float stackF[MAX_TOKENS];
static int topF;
static void pushF(float v) { stackF[++topF] = v; }
static float popF(void) { return (topF >= 0) ? stackF[topF--] : 0.0f; }

static int prec(const char *op) {
    if (strcmp(op, "^") == 0) return 4;
    if (!strcmp(op, "raiz") || !strcmp(op, "sen") || !strcmp(op, "cos") || !strcmp(op, "tg") || !strcmp(op, "log")) return 5;
    if (!strcmp(op, "*") || !strcmp(op, "/") || !strcmp(op, "%")) return 3;
    if (!strcmp(op, "+") || !strcmp(op, "-")) return 2;
    return 0;
}
static int isOperator(const char *s) {
    return (!strcmp(s, "+") || !strcmp(s, "-") || !strcmp(s, "*") || !strcmp(s, "/") || !strcmp(s, "%") || !strcmp(s, "^"));
}
static int isFunction(const char *s) {
    return (!strcmp(s, "raiz") || !strcmp(s, "sen") || !strcmp(s, "cos") || !strcmp(s, "tg") || !strcmp(s, "log"));
}

static int tokenize(const char *expr, char tokens[][MAX_TOKEN_LEN]) {
    int i = 0, k = 0;
    while (expr[i] && k < MAX_TOKENS) {
        if (isspace((unsigned char)expr[i])) { i++; continue; }
        if (expr[i] == '(' || expr[i] == ')') {
            tokens[k][0] = expr[i]; tokens[k][1] = '\0'; k++; i++; continue;
        }
        int j = 0;
        while (expr[i] && !isspace((unsigned char)expr[i]) && expr[i] != '(' && expr[i] != ')') {
            tokens[k][j++] = expr[i++];
        }
        tokens[k][j] = '\0';
        k++;
    }
    return k;
}

char *getFormaPosFixa(char *Str) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN], output[512];
    int n = tokenize(Str, tokens);
    output[0] = '\0'; topS = -1;
    for (int i = 0; i < n; i++) {
        char *tok = tokens[i];
        if (isdigit((unsigned char)tok[0]) || tok[0] == '.') {
            strcat(output, tok); strcat(output, " ");
        } else if (isFunction(tok)) {
            pushS(strdup(tok));
        } else if (isOperator(tok)) {
            while (topS >= 0 && (isOperator(peekS()) || isFunction(peekS())) &&
                   ((prec(peekS()) > prec(tok)) || (prec(peekS()) == prec(tok) && strcmp(tok, "^") != 0))) {
                char *op = popS(); strcat(output, op); strcat(output, " "); free(op);
            }
            pushS(strdup(tok));
        } else if (!strcmp(tok, "(")) {
            pushS(strdup(tok));
        } else if (!strcmp(tok, ")")) {
            while (topS >= 0 && strcmp(peekS(), "(") != 0) {
                char *op = popS(); strcat(output, op); strcat(output, " "); free(op);
            }
            free(popS()); // remove '('
            if (topS >= 0 && isFunction(peekS())) {
                char *fn = popS(); strcat(output, fn); strcat(output, " "); free(fn);
            }
        }
    }
    while (topS >= 0) { char *op = popS(); strcat(output, op); strcat(output, " "); free(op); }
    return strdup(output);
}

typedef struct Node { char *expr; int prec; } Node;

char *getFormaInFixa(char *Str) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN];
    int n = tokenize(Str, tokens), topN = -1;
    Node *stackN[MAX_TOKENS];
    for (int i = 0; i < n; i++) {
        char *tok = tokens[i];
        if (isdigit((unsigned char)tok[0]) || tok[0] == '.') {
            Node *node = malloc(sizeof(Node)); node->expr = strdup(tok); node->prec = INT_MAX;
            stackN[++topN] = node;
        } else if (isOperator(tok)) {
            Node *b = stackN[topN--], *a = stackN[topN--];
            int p = prec(tok);
            char *astr = a->expr, *bstr = b->expr;
            if (a->prec < p) { int la = strlen(astr)+3; char *t = malloc(la); snprintf(t, la, "(%s)", astr); astr = t; }
            if (b->prec < p) { int lb = strlen(bstr)+3; char *t = malloc(lb); snprintf(t, lb, "(%s)", bstr); bstr = t; }
            int lbuff = strlen(astr)+strlen(tok)+strlen(bstr)+4;
            char *buf = malloc(lbuff); snprintf(buf, lbuff, "%s %s %s", astr, tok, bstr);
            Node *node = malloc(sizeof(Node)); node->expr = buf; node->prec = p;
            free(a->expr); free(b->expr); free(a); free(b);
            if (astr != a->expr) free(astr); if (bstr != b->expr) free(bstr);
            stackN[++topN] = node;
        } else if (isFunction(tok)) {
            Node *a = stackN[topN--]; int p = prec(tok);
            char *astr = a->expr;
            if (a->prec < p) { int la = strlen(astr)+3; char *t = malloc(la); snprintf(t, la, "(%s)", astr); astr = t; }
            int lbuff = strlen(tok)+strlen(astr)+3;
            char *buf = malloc(lbuff); snprintf(buf, lbuff, "%s(%s)", tok, astr);
            Node *node = malloc(sizeof(Node)); node->expr = buf; node->prec = p;
            free(a->expr); free(a); if (astr != a->expr) free(astr);
            stackN[++topN] = node;
        }
    }
    char *res = strdup(stackN[topN]->expr);
    free(stackN[topN]->expr); free(stackN[topN]);
    // remove external parentheses
    int len = strlen(res); if (len>1 && res[0]=='(' && res[len-1]==')') {
        int bal=0, ext=1; for (int i=0;i<len;i++){ if(res[i]=='(')bal++; else if(res[i]==')')bal--; if(!bal && i<len-1){ext=0;break;} }
        if(ext){ char *s = malloc(len-1); strncpy(s, res+1, len-2); s[len-2]='\0'; free(res); res=s; }
    }
    return res;
}

float getValorPosFixa(char *StrPosFixa) {
    char tokens[MAX_TOKENS][MAX_TOKEN_LEN]; int n=tokenize(StrPosFixa, tokens);
    topF=-1;
    for(int i=0;i<n;i++){
        char *tok=tokens[i];
        if(isdigit((unsigned char)tok[0])||tok[0]=='.') pushF(atof(tok));
        else if(isOperator(tok)){
            float b=popF(), a=popF(), r;
            if(!strcmp(tok,"+")) r=a+b; else if(!strcmp(tok,"-")) r=a-b;
            else if(!strcmp(tok,"*")) r=a*b; else if(!strcmp(tok,"/")) r=a/b;
            else if(!strcmp(tok,"%")) r=fmodf(a,b); else r=powf(a,b);
            pushF(r);
        } else if(isFunction(tok)){
            float a=popF(), r;
            if(!strcmp(tok,"raiz")) r=sqrtf(a);
            else if(!strcmp(tok,"sen")) r=sinf(a*M_PI/180);
            else if(!strcmp(tok,"cos")) r=cosf(a*M_PI/180);
            else if(!strcmp(tok,"tg"))  r=tanf(a*M_PI/180);
            else r=log10f(a);
            pushF(r);
        }
    }
    return popF();
}

float getValorInFixa(char *StrInFixa) {
    char *p=getFormaPosFixa(StrInFixa); float v=getValorPosFixa(p);
    free(p); return v;
}

