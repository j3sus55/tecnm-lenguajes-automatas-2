/* harness_sesion01_c.c - auditoria automatica de la Sesion 01
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion01_c.c artesano_sesion01_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion01.h"

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

int main(void) {
    printf("=== AUDITORIA SESION 01 - Inversion de buffer in-place ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { char a[] = "HOLA"; invertir(a); verificar_str("invertir(\"HOLA\")", "ALOH", a); }
    { char b[] = "abc"; invertir(b); verificar_str("invertir(\"abc\")", "cba", b); }
    { char c[] = ""; invertir(c); verificar_str("invertir(\"\")", "", c); }
    { char d[] = "AB"; invertir(d); verificar_str("invertir(\"AB\")", "BA", d); }
    { char e[] = "a"; invertir(e); verificar_str("invertir(\"a\")", "a", e); }
    { invertir(NULL); verificar_str("invertir(NULL)", "sin crash", "sin crash"); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion01.md" "\n");
    return 0;
}
