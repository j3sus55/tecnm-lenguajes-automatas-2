/* harness_sesion18_c.c - auditoria automatica de la Sesion 18
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion18_c.c artesano_sesion18_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion18.h"

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
    printf("=== AUDITORIA SESION 18 - Tamano de arreglo y limites ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { long r = bytes_arreglo("int", 10); verificar_int("int[10]", (long)(40), (long)(r)); }
    { long r = bytes_arreglo("double", 3); verificar_int("double[3]", (long)(24), (long)(r)); }
    { long r = bytes_arreglo("int", -4); verificar_int("int[-4]", (long)(-1), (long)(r)); }
    { int r = indice_en_rango(0, 5); verificar_int("indice 0 de 5", (long)(1), (long)(r)); }
    { int r = indice_en_rango(5, 5); verificar_int("indice 5 de 5", (long)(0), (long)(r)); }
    { int r = indice_en_rango(-1, 5); verificar_int("indice negativo", (long)(0), (long)(r)); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion18.md" "\n");
    return 0;
}
