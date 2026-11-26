/* harness_sesion04_c.c - auditoria automatica de la Sesion 04
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion04_c.c artesano_sesion04_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion04.h"

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
    printf("=== AUDITORIA SESION 04 - Nodo binario de AST ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { ASTNode *n = crear_numero(42); verificar_int("evaluar(nodo 42)", (long)(42), (long)(evaluar(n))); }
    { ASTNode *n = crear_binario('+', crear_numero(3), crear_binario('*', crear_numero(5), crear_numero(2))); verificar_int("evaluar(3+5*2)", (long)(13), (long)(evaluar(n))); }
    { ASTNode *n = crear_binario('*', crear_numero(5), crear_numero(2)); verificar_int("hijo izquierdo de (* 5 2)", (long)(5), (long)((n && n->izq ? n->izq->valor : -1))); }
    { ASTNode *n = crear_binario('*', crear_numero(5), crear_numero(2)); char b[2]; b[0] = n ? n->op : '?'; b[1] = '\0'; verificar_str("operador guardado", "*", b); }
    {  verificar_int("evaluar(NULL)", (long)(-1), (long)(evaluar(NULL))); }
    { ASTNode *n = crear_binario('-', crear_numero(10), crear_numero(4)); verificar_int("evaluar(10-4)", (long)(6), (long)(evaluar(n))); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion04.md" "\n");
    return 0;
}
