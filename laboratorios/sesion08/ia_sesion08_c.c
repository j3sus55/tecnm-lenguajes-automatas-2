#include "api_sesion08.h"

Tipo promover(Tipo a, Tipo b) {
    if (a == TIPO_ERROR || b == TIPO_ERROR) {
        return TIPO_ERROR;
    }
    return a > b ? a : b;
}

int hay_perdida(Tipo destino, Tipo origen) {
    return destino != TIPO_ERROR && origen != TIPO_ERROR && destino < origen;
}

const char *nombre_tipo(Tipo t) {
    static const char *const nombres[] = {
        [TIPO_ERROR] = "error",
        [TIPO_CHAR] = "char",
        [TIPO_INT] = "int",
        [TIPO_FLOAT] = "float"
    };

    if (t < TIPO_ERROR || t > TIPO_FLOAT) {
        return nombres[TIPO_ERROR];
    }
    return nombres[t];
}
