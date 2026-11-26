#include "api_sesion11.h"

#include <stdlib.h>
#include <string.h>

unsigned int hash_djb2(const char *clave) {
    unsigned int hash = 5381;
    const unsigned char *byte = (const unsigned char *)clave;

    while (*byte != 0) {
        hash = hash * 33u + *byte;
        ++byte;
    }
    return hash;
}

int insertar_simbolo(Simbolo *tabla[], unsigned int cubetas,
                     const char *nombre, int tipo, int offset) {
    if (tabla == NULL || cubetas == 0 || nombre == NULL) {
        return -1;
    }

    size_t longitud = strlen(nombre);
    if (longitud >= sizeof tabla[0]->nombre) {
        return -1;
    }

    unsigned int indice = hash_djb2(nombre) % cubetas;
    for (unsigned int i = 0; i < cubetas; ++i) {
        for (const Simbolo *actual = tabla[i]; actual != NULL; actual = actual->sig) {
            if (strcmp(actual->nombre, nombre) == 0) return 0;
        }
    }

    Simbolo *nuevo = malloc(sizeof *nuevo);
    if (nuevo == NULL) {
        return -1;
    }

    memcpy(nuevo->nombre, nombre, longitud + 1);
    nuevo->tipo = tipo;
    nuevo->offset = offset;
    nuevo->sig = tabla[indice];
    tabla[indice] = nuevo;
    return 1;
}
