#include "api_sesion11.h"
#include <stdlib.h>
#include <string.h>

unsigned int hash_djb2(const char *clave) {
    unsigned int hash = 5381u;
    if (clave != NULL) {
        for (; *clave != '\0'; ++clave) {
            hash = (hash << 5) + hash + (unsigned char)*clave;
        }
    }
    return hash;
}

int insertar_simbolo(Simbolo *tabla[], unsigned int cubetas,
                     const char *nombre, int tipo, int offset) {
    if (tabla == NULL || cubetas == 0 || nombre == NULL) return -1;

    Simbolo datos = {0};
    size_t longitud = strlen(nombre);
    if (longitud >= sizeof datos.nombre) return -1;

    unsigned int indice = hash_djb2(nombre) % cubetas;
    Simbolo **destino = &tabla[indice];
    for (unsigned int i = 0; i < cubetas; ++i) {
        Simbolo **enlace = &tabla[i];
        while (*enlace != NULL) {
            if (strcmp((*enlace)->nombre, nombre) == 0) return 0;
            enlace = &(*enlace)->sig;
        }
        if (i == indice) destino = enlace;
    }

    Simbolo *nuevo = malloc(sizeof *nuevo);
    if (nuevo == NULL) return -1;
    memcpy(datos.nombre, nombre, longitud + 1);
    datos.tipo = tipo;
    datos.offset = offset;
    *nuevo = datos;
    *destino = nuevo;
    return 1;
}
