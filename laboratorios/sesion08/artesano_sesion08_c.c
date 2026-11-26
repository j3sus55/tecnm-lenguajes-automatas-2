#include "api_sesion08.h"

Tipo promover(Tipo a, Tipo b) {
    if (a == TIPO_ERROR || b == TIPO_ERROR) {
        return TIPO_ERROR;
    }
    if (a == TIPO_FLOAT || b == TIPO_FLOAT) {
        return TIPO_FLOAT;
    }
    if (a == TIPO_INT || b == TIPO_INT) {
        return TIPO_INT;
    }
    return TIPO_CHAR;
}

int hay_perdida(Tipo destino, Tipo origen) {
    switch (origen) {
        case TIPO_FLOAT:
            return destino == TIPO_INT || destino == TIPO_CHAR;
        case TIPO_INT:
            return destino == TIPO_CHAR;
        default:
            return 0;
    }
}

const char *nombre_tipo(Tipo t) {
    switch (t) {
        case TIPO_CHAR: return "char";
        case TIPO_INT: return "int";
        case TIPO_FLOAT: return "float";
        default: return "error";
    }
}
