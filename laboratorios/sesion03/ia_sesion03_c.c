#include "api_sesion03.h"
#include <stdio.h>

int escribir_texto(const char *ruta, const char *texto)
{
    FILE *archivo;

    if (ruta == NULL || texto == NULL) {
        return -1;
    }

    archivo = fopen(ruta, "w");

    if (archivo == NULL) {
        return -1;
    }

    if (fputs(texto, archivo) == EOF) {
        fclose(archivo);
        return -1;
    }

    if (fclose(archivo) == EOF) {
        return -1;
    }

    return 0;
}

int contar_lineas(const char *ruta)
{
    FILE *archivo;
    int caracter;
    int lineas = 0;
    int hay_contenido = 0;
    int ultimo_caracter = '\0';

    if (ruta == NULL) {
        return -1;
    }

    archivo = fopen(ruta, "r");

    if (archivo == NULL) {
        return -1;
    }

    while ((caracter = fgetc(archivo)) != EOF) {
        hay_contenido = 1;

        if (caracter == '\n') {
            lineas++;
        }

        ultimo_caracter = caracter;
    }

    if (ferror(archivo)) {
        fclose(archivo);
        return -1;
    }

    if (hay_contenido && ultimo_caracter != '\n') {
        lineas++;
    }

    if (fclose(archivo) == EOF) {
        return -1;
    }

    return lineas;
}
