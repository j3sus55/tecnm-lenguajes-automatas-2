/* harness_sesion02_c.c - auditoria automatica de la Sesion 02
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion02_c.c artesano_sesion02_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion02.h"

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
    printf("=== AUDITORIA SESION 02 - Lista enlazada de tokens en heap ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { NodoToken *c = agregar_token(NULL, "if"); verificar_str("agregar_token(NULL,\"if\")", "if", (c != NULL ? c->lexema : "(NULL)")); }
    { NodoToken *c = NULL; c = agregar_token(c, "if"); c = agregar_token(c, "x"); c = agregar_token(c, ";"); verificar_int("contar_tokens(3 tokens)", (long)(3), (long)(contar_tokens(c))); }
    { NodoToken *c = NULL; c = agregar_token(c, "uno"); c = agregar_token(c, "dos"); verificar_str("orden: segundo token", "dos", (c && c->siguiente ? c->siguiente->lexema : "(NULL)")); }
    { NodoToken *c = NULL; c = agregar_token(c, "a"); c = agregar_token(c, "b"); verificar_int("ultimo->siguiente == NULL", (long)(1), (long)((c && c->siguiente ? (c->siguiente->siguiente == NULL) : 0))); }
    {  verificar_int("contar_tokens(NULL)", (long)(0), (long)(contar_tokens(NULL))); }
    { NodoToken *c = NULL; c = agregar_token(c, "a"); liberar_lista(c); liberar_lista(NULL); verificar_str("liberar_lista(NULL)", "sin crash", "sin crash"); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion02.md" "\n");
    return 0;
}
