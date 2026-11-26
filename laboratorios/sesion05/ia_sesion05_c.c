#include "api_sesion05.h"
#include <stdio.h>
#include <string.h>

static void append_token(char *buf, int cap, const char *token) {
    size_t current_len = strlen(buf);
    size_t token_len = strlen(token);
    size_t needed = current_len + token_len + (current_len > 0 ? 1 : 0);
    
    if (needed >= (size_t)cap) return;
    
    if (current_len > 0) {
        buf[current_len] = ' ';
        buf[current_len + 1] = '\0';
    }
    strncat(buf, token, cap - strlen(buf) - 1);
}

static void postorden_rec(ASTNode *n, char *buf, int cap) {
    if (n == NULL) return;
    postorden_rec(n->izq, buf, cap);
    postorden_rec(n->der, buf, cap);
    
    char token[32];
    if (n->tipo == NODO_NUM) {
        snprintf(token, sizeof(token), "%d", n->valor);
    } else {
        token[0] = n->op;
        token[1] = '\0';
    }
    append_token(buf, cap, token);
}

void postorden(ASTNode *n, char *buf, int cap) {
    if (buf != NULL && cap > 0) buf[0] = '\0';
    if (n == NULL || buf == NULL || cap <= 0) return;
    postorden_rec(n, buf, cap);
}
