/* harness_sesion08_c.c - auditoria automatica de la Sesion 08
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion08_c.c artesano_sesion08_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion08.h"

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
    printf("=== AUDITORIA SESION 08 - Reglas de promocion y perdida ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    {  verificar_str("promover(CHAR,INT)", "int", nombre_tipo(promover(TIPO_CHAR, TIPO_INT))); }
    {  verificar_str("promover(INT,FLOAT)", "float", nombre_tipo(promover(TIPO_INT, TIPO_FLOAT))); }
    {  verificar_str("promover(FLOAT,FLOAT)", "float", nombre_tipo(promover(TIPO_FLOAT, TIPO_FLOAT))); }
    {  verificar_str("promover(ERROR,INT)", "error", nombre_tipo(promover(TIPO_ERROR, TIPO_INT))); }
    {  verificar_int("hay_perdida(CHAR,INT)", (long)(1), (long)(hay_perdida(TIPO_CHAR, TIPO_INT))); }
    {  verificar_int("hay_perdida(INT,CHAR)", (long)(0), (long)(hay_perdida(TIPO_INT, TIPO_CHAR))); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion08.md" "\n");
    return 0;
}
