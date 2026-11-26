/* harness_sesion03_c.c - auditoria automatica de la Sesion 03
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion03_c.c artesano_sesion03_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion03.h"

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
    printf("=== AUDITORIA SESION 03 - Archivos: escribir y contar lineas ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { int r = escribir_texto("datos.txt", "uno\ndos\ntres\n"); verificar_int("escribir_texto(...)==0", (long)(0), (long)(r)); }
    { escribir_texto("datos.txt", "uno\ndos\ntres\n"); verificar_int("contar_lineas(3 lineas)", (long)(3), (long)(contar_lineas("datos.txt"))); }
    { escribir_texto("datos2.txt", "a\nb"); verificar_int("contar_lineas(sin salto final)", (long)(2), (long)(contar_lineas("datos2.txt"))); }
    { escribir_texto("vacio.txt", ""); verificar_int("contar_lineas(archivo vacio)", (long)(0), (long)(contar_lineas("vacio.txt"))); }
    {  verificar_int("contar_lineas(inexistente)", (long)(-1), (long)(contar_lineas("no_existe.txt"))); }
    {  verificar_int("escribir_texto(ruta invalida)", (long)(-1), (long)(escribir_texto("/no/existe/x.txt", "a"))); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion03.md" "\n");
    return 0;
}
