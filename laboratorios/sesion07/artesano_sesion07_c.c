#include "api_sesion07.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

static int evaluar_nodo(const ASTNode *node, int *error) {
    if (node == NULL || (node->tipo != NODO_NUM && node->tipo != NODO_BINOP)) {
        *error = 2;
        return 0;
    }
    if (node->tipo == NODO_NUM) return node->valor;

    int64_t left = evaluar_nodo(node->izq, error);
    if (*error != 0) return 0;
    int64_t right = evaluar_nodo(node->der, error);
    if (*error != 0) return 0;
    if ((node->op == '/' || node->op == '%') && right == 0) {
        *error = 1;
        return 0;
    }

    int64_t result;
    switch (node->op) {
        case '+': result = left + right; break;
        case '-': result = left - right; break;
        case '*': result = left * right; break;
        case '/': result = left / right; break;
        case '%': result = left % right; break;
        default:
            *error = 2;
            return 0;
    }
    if (result < INT_MIN || result > INT_MAX) {
        *error = 2;
        return 0;
    }
    return (int)result;
}

int evaluar(ASTNode *n, int *error) {
    int status = 0;
    int result = evaluar_nodo(n, &status);
    if (error != NULL) *error = status;
    return result;
}
