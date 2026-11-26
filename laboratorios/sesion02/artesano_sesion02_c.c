#include "api_sesion02.h"
#include <stdlib.h>

NodoToken *agregar_token(NodoToken *cabeza, const char *lexema) {
    if (lexema == NULL) return cabeza;
    NodoToken *node = malloc(sizeof *node);
    if (node == NULL) return cabeza;
    size_t index = 0;
    while (index < sizeof node->lexema - 1 && lexema[index] != '\0') {
        node->lexema[index] = lexema[index];
        ++index;
    }
    node->lexema[index] = '\0';
    node->siguiente = NULL;
    NodoToken **link = &cabeza;
    while (*link != NULL) link = &(*link)->siguiente;
    *link = node;
    return cabeza;
}

int contar_tokens(NodoToken *cabeza) {
    int count = 0;
    for (; cabeza != NULL; cabeza = cabeza->siguiente) ++count;
    return count;
}

void liberar_lista(NodoToken *cabeza) {
    while (cabeza != NULL) {
        NodoToken *next = cabeza->siguiente;
        free(cabeza);
        cabeza = next;
    }
}
