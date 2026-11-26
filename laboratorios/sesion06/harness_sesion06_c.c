/* harness_sesion06_c.c - auditoria automatica de la Sesion 06
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion06_c.c artesano_sesion06_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion06.h"

static int probadas = 0;
static int fallidas = 0;

static void verificar_str(const char *caso, const char *esperado,
                          const char *obtenido) {
    int ok = (obtenido != NULL) && (strcmp(esperado, obtenido) == 0);
    probadas++;
    if (!ok) fallidas++;
    printf("[%2d] %-34s esperado: %-12s obtenido: %-12s %s\n",
           probadas, caso, esperado, obtenido ? obtenido : "(NULL)",
           ok ? "OK" : "FALLA");
}

static void verificar_int(const char *caso, long esperado, long obtenido) {
    int ok = (esperado == obtenido);
    probadas++;
    if (!ok) fallidas++;
    printf("[%2d] %-34s esperado: %-12ld obtenido: %-12ld %s\n",
           probadas, caso, esperado, obtenido, ok ? "OK" : "FALLA");
}

int main(void) {
    printf("=== AUDITORIA SESION 06 - Tabla de compatibilidad de tipos ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    {  verificar_int("compatible(INT,INT)", (long)(1), (long)(compatible(TIPO_INT, TIPO_INT))); }
    {  verificar_int("compatible(FLOAT,INT)", (long)(1), (long)(compatible(TIPO_FLOAT, TIPO_INT))); }
    {  verificar_int("compatible(INT,FLOAT)", (long)(0), (long)(compatible(TIPO_INT, TIPO_FLOAT))); }
    {  verificar_int("compatible(INT,ERROR)", (long)(0), (long)(compatible(TIPO_INT, TIPO_ERROR))); }
    {  verificar_str("nombre_tipo(INT)", "int", nombre_tipo(TIPO_INT)); }
    {  verificar_str("nombre_tipo(ERROR)", "error", nombre_tipo(TIPO_ERROR)); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion06.md" "\n");
    return 0;
}
