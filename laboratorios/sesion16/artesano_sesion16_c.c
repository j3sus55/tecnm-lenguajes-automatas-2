#include "api_sesion16.h"
#include <string.h>

int verificar_llamada(const Funcion *catalogo, int total, const char *nombre, int nargs) {
    if (catalogo == NULL || nombre == NULL) return -1;
    for (int i = 0; i < total; ++i) {
        if (catalogo[i].nombre != NULL && strcmp(catalogo[i].nombre, nombre) == 0) {
            return nargs >= 0 && catalogo[i].aridad == nargs;
        }
    }
    return -1;
}
