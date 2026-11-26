#include "api_sesion18.h"
#include <limits.h>
#include <string.h>

long bytes_arreglo(const char *tipo, int elementos) {
    static const struct { const char *nombre; long ancho; } tipos[] = {
        {"char", 1}, {"short", 2}, {"int", 4}, {"float", 4}, {"double", 8}, {"puntero", 8}
    };
    if (tipo == NULL || elementos < 0) return -1;

    for (size_t i = 0; i < sizeof tipos / sizeof tipos[0]; ++i) {
        if (strcmp(tipo, tipos[i].nombre) == 0) {
            long ancho = tipos[i].ancho;
            if (elementos > LONG_MAX / ancho) return -1;
            return ancho * elementos;
        }
    }
    return -1;
}

int indice_en_rango(int indice, int elementos) {
    return indice >= 0 && indice < elementos;
}
