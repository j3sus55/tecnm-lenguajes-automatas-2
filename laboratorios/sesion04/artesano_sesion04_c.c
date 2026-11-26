#include "api_sesion04.h"
#include <stdlib.h>

ASTNode *crear_numero(int v) {
    ASTNode *n = malloc(sizeof *n);
    if (n != NULL) *n = (ASTNode){NODO_NUM, v, 0, NULL, NULL};
    return n;
}

ASTNode *crear_binario(char op, ASTNode *izq, ASTNode *der) {
    ASTNode *n = malloc(sizeof *n);
    if (n != NULL) *n = (ASTNode){NODO_BINOP, 0, op, izq, der};
    return n;
}

int evaluar(ASTNode *n) {
    if (n == NULL) return -1;
    if (n->tipo == NODO_NUM) return n->valor;
    if (n->tipo != NODO_BINOP || n->izq == NULL || n->der == NULL) return -1;

    int izq = evaluar(n->izq);
    int der = evaluar(n->der);
    switch (n->op) {
        case '+': return izq + der;
        case '-': return izq - der;
        case '*': return izq * der;
        default: return -1;
    }
}
