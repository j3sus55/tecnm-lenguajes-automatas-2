#include "api_sesion07.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int valor;
    int error;
} Resultado;

static Resultado evaluar_interno(const ASTNode *n) {
    if (n == NULL) return (Resultado){0, 2};
    if (n->tipo == NODO_NUM) return (Resultado){n->valor, 0};
    if (n->tipo != NODO_BINOP) return (Resultado){0, 2};

    Resultado izq = evaluar_interno(n->izq);
    if (izq.error) return izq;
    Resultado der = evaluar_interno(n->der);
    if (der.error) return der;

    int64_t a = izq.valor;
    int64_t b = der.valor;
    int64_t valor;
    switch (n->op) {
        case '+': valor = a + b; break;
        case '-': valor = a - b; break;
        case '*': valor = a * b; break;
        case '/':
            if (b == 0) return (Resultado){0, 1};
            valor = a / b;
            break;
        case '%':
            if (b == 0) return (Resultado){0, 1};
            valor = a % b;
            break;
        default: return (Resultado){0, 2};
    }
    if (valor < INT_MIN || valor > INT_MAX) return (Resultado){0, 2};
    return (Resultado){(int)valor, 0};
}

int evaluar(ASTNode *n, int *error) {
    Resultado r = evaluar_interno(n);
    if (error != NULL) *error = r.error;
    return r.valor;
}
