/* harness_sesion10_c.c - auditoria automatica de la Sesion 10
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion10_c.c artesano_sesion10_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion10.h"

static int probadas = 0;
static int fallidas = 0;

static void verificar_int(const char *caso, long esperado, long obtenido) {
    int ok = (esperado == obtenido);
    probadas++;
    if (!ok) fallidas++;
    printf("[%2d] %-34s esperado: %-12ld obtenido: %-12ld %s\n",
           probadas, caso, esperado, obtenido, ok ? "OK" : "FALLA");
}

int main(void) {
    printf("=== AUDITORIA SESION 10 - Mini-evaluador de expresiones (Hito 1) ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { int e=0; long r=evaluar_expresion("2+3*4",&e); (void)e; verificar_int("\"2+3*4\"", (long)(14), (long)(r)); }
    { int e=0; long r=evaluar_expresion("(2+3)*4",&e); (void)e; verificar_int("\"(2+3)*4\"", (long)(20), (long)(r)); }
    { int e=0; long r=evaluar_expresion("10/0",&e); (void)r; verificar_int("\"10/0\" -> error 1", (long)(1), (long)(e)); }
    { int e=0; long r=evaluar_expresion("2+",&e); (void)r; verificar_int("\"2+\" -> error 2", (long)(2), (long)(e)); }
    { int e=0; long r=evaluar_expresion("  7 ",&e); (void)e; verificar_int("\"  7 \"", (long)(7), (long)(r)); }
    { int e=0; long r=evaluar_expresion("5-10",&e); (void)e; verificar_int("\"5-10\"", (long)(-5), (long)(r)); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion10.md" "\n");
    return 0;
}
