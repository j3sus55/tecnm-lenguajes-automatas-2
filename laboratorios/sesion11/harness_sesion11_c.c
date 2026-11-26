/* harness_sesion11_c.c - auditoria automatica de la Sesion 11
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion11_c.c artesano_sesion11_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion11.h"

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
    printf("=== AUDITORIA SESION 11 - Tabla hash con encadenamiento ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { unsigned int r = hash_djb2(""); verificar_int("hash_djb2(\"\")", (long)(5381), (long)(r)); }
    { unsigned int r = hash_djb2("abc"); verificar_int("hash_djb2(\"abc\")", (long)(193485963), (long)(r)); }
    { Simbolo *t[8] = {0}; insertar_simbolo(t, 8, "x", 1, 0); int r = insertar_simbolo(t, 8, "x", 2, 8); verificar_int("insertar x dos veces", (long)(0), (long)(r)); }
    { Simbolo *t[1] = {0}; insertar_simbolo(t, 1, "alpha", 1, 0); insertar_simbolo(t, 1, "beta", 2, 4); int r = 0; for (Simbolo *p = t[0]; p != NULL; p = p->sig) r++; verificar_int("dos claves, una cubeta", (long)(2), (long)(r)); }
    { Simbolo *t[1] = {0}; insertar_simbolo(t, 1, "alpha", 1, 0); insertar_simbolo(t, 1, "beta", 2, 4); int r = -1; for (Simbolo *p = t[0]; p != NULL; p = p->sig) { if (strcmp(p->nombre, "beta") == 0) r = p->offset; } verificar_int("offset de beta en cadena", (long)(4), (long)(r)); }
    { int r = insertar_simbolo(NULL, 8, "x", 1, 0); verificar_int("insertar en tabla NULL", (long)(-1), (long)(r)); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion11.md" "\n");
    return 0;
}
