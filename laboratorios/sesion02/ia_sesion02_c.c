#include "api_sesion02.h"
#include <stdlib.h>
#include <string.h>

NodoToken *agregar_token(NodoToken *cabeza, const char *lexema) {
    if (lexema == NULL) return cabeza;

    NodoToken *nuevo = calloc(1, sizeof *nuevo);
    if (nuevo == NULL) return cabeza;

    size_t longitud = strlen(lexema);
    if (longitud >= sizeof nuevo->lexema) longitud = sizeof nuevo->lexema - 1;
    memcpy(nuevo->lexema, lexema, longitud);
    nuevo->siguiente = NULL;

    if (cabeza == NULL) return nuevo;

    NodoToken *ultimo = cabeza;
    while (ultimo->siguiente != NULL) ultimo = ultimo->siguiente;
    ultimo->siguiente = nuevo;
    return cabeza;
}

int contar_tokens(NodoToken *cabeza) {
    int total = 0;
    while (cabeza != NULL) {
        ++total;
        cabeza = cabeza->siguiente;
    }
    return total;
}

void liberar_lista(NodoToken *cabeza) {
    while (cabeza != NULL) {
        NodoToken *actual = cabeza;
        cabeza = cabeza->siguiente;
        free(actual);
    }
}
