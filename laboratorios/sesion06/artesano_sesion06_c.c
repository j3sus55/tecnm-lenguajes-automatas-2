#include "api_sesion06.h"

/* Filas: destino. Columnas: origen. Orden: ERROR, CHAR, INT, FLOAT. */
static const int tabla[TIPO_FLOAT + 1][TIPO_FLOAT + 1] = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 1}
};

int compatible(Tipo destino, Tipo origen) {
    if ((unsigned int)destino > TIPO_FLOAT ||
        (unsigned int)origen > TIPO_FLOAT) {
        return 0;
    }

    return tabla[destino][origen];
}

const char *nombre_tipo(Tipo t) {
    static const char *const nombres[] = {"error", "char", "int", "float"};

    if ((unsigned int)t > TIPO_FLOAT) {
        return "desconocido";
    }

    return nombres[t];
}
