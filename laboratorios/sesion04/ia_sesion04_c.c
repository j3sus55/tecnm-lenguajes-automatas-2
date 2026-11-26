#include "api_sesion04.h"
#include <stdlib.h>

ASTNode *crear_numero(int v)
{
    ASTNode *n = malloc(sizeof(ASTNode));

    if (n == NULL) {
        return NULL;
    }

    n->tipo = NODO_NUM;
    n->valor = v;
    n->op = '\0';
    n->izq = NULL;
    n->der = NULL;

    return n;
}

ASTNode *crear_binario(char op, ASTNode *izq, ASTNode *der)
{
    ASTNode *n;

    if (op != '+' && op != '-' && op != '*') {
        return NULL;
    }

    if (izq == NULL || der == NULL) {
        return NULL;
    }

    n = malloc(sizeof(ASTNode));

    if (n == NULL) {
        return NULL;
    }

    n->tipo = NODO_BINOP;
    n->valor = 0;
    n->op = op;
    n->izq = izq;
    n->der = der;

    return n;
}

int evaluar(ASTNode *n)
{
    int izquierda;
    int derecha;

    if (n == NULL) {
        return -1;
    }

    if (n->tipo == NODO_NUM) {
        return n->valor;
    }

    if (n->tipo != NODO_BINOP) {
        return -1;
    }

    izquierda = evaluar(n->izq);
    derecha = evaluar(n->der);

    if (izquierda == -1 || derecha == -1) {
        return -1;
    }

    switch (n->op) {
        case '+':
            return izquierda + derecha;

        case '-':
            return izquierda - derecha;

        case '*':
            return izquierda * derecha;

        default:
            return -1;
    }
}
