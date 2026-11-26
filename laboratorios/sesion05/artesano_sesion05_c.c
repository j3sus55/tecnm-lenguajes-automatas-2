#include "api_sesion05.h"
#include <stdio.h>
#include <string.h>

static int recorrer(const ASTNode *n, char *buf, size_t cap, size_t *usado) {
    if (n == NULL) return 1;
    if (!recorrer(n->izq, buf, cap, usado)) return 0;
    if (!recorrer(n->der, buf, cap, usado)) return 0;

    char token[32];
    int longitud;
    if (n->tipo == NODO_NUM) {
        longitud = snprintf(token, sizeof token, "%d", n->valor);
    } else {
        token[0] = n->op;
        longitud = 1;
    }
    if (longitud < 0 || (size_t)longitud >= sizeof token) return 0;
    size_t separador = *usado != 0;
    if (separador + (size_t)longitud >= cap - *usado) return 0;

    if (separador) buf[(*usado)++] = ' ';
    memcpy(buf + *usado, token, (size_t)longitud);
    *usado += (size_t)longitud;
    buf[*usado] = '\0';
    return 1;
}

void postorden(ASTNode *n, char *buf, int cap) {
    if (buf == NULL || cap <= 0) return;
    buf[0] = '\0';
    size_t usado = 0;
    (void)recorrer(n, buf, (size_t)cap, &usado);
}
