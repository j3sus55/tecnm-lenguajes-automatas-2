/* harness_sesion16_c.c - auditoria automatica de la Sesion 16
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion16_c.c artesano_sesion16_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion16.h"

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
    printf("=== AUDITORIA SESION 16 - Registro de prototipos y aridad ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { Funcion c[] = {{"suma", 3}}; int r = verificar_llamada(c, 1, "suma", 3); verificar_int("suma con 3 argumentos", (long)(1), (long)(r)); }
    { Funcion c[] = {{"suma", 3}}; int r = verificar_llamada(c, 1, "suma", 2); verificar_int("suma con 2 argumentos", (long)(0), (long)(r)); }
    { Funcion c[] = {{"suma", 3}}; int r = verificar_llamada(c, 1, "suma", 4); verificar_int("suma con 4 argumentos", (long)(0), (long)(r)); }
    { Funcion c[] = {{"suma", 3}}; int r = verificar_llamada(c, 1, "resta", 2); verificar_int("funcion no registrada", (long)(-1), (long)(r)); }
    { Funcion c[] = {{"main", 0}}; int r = verificar_llamada(c, 1, "main", 0); verificar_int("main sin argumentos", (long)(1), (long)(r)); }
    { Funcion c[] = {{"suma", 3}}; int r = verificar_llamada(c, 1, NULL, 3); verificar_int("nombre NULL", (long)(-1), (long)(r)); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion16.md" "\n");
    return 0;
}
