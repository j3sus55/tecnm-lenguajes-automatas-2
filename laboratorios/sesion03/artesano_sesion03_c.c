#include "api_sesion03.h"
#include <stdio.h>

int escribir_texto(const char *ruta, const char *texto) {
    if (ruta == NULL || texto == NULL) return -1;
    FILE *archivo = fopen(ruta, "w");
    if (archivo == NULL) return -1;

    int escritura = fputs(texto, archivo);
    int cierre = fclose(archivo);
    return escritura == EOF || cierre == EOF ? -1 : 0;
}

int contar_lineas(const char *ruta) {
    if (ruta == NULL) return -1;
    FILE *archivo = fopen(ruta, "r");
    if (archivo == NULL) return -1;

    int lineas = 0;
    int ultima_incompleta = 0;
    for (int c = fgetc(archivo); c != EOF; c = fgetc(archivo)) {
        ultima_incompleta = c != '\n';
        if (!ultima_incompleta) ++lineas;
    }

    int error = ferror(archivo);
    if (fclose(archivo) == EOF || error) return -1;
    return lineas + ultima_incompleta;
}
