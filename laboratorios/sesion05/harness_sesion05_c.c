/* harness_sesion05_c.c - auditoria automatica de la Sesion 05
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion05_c.c artesano_sesion05_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion05.h"

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

static void verificar_str_largo(const char *caso, const char *esperado,
                                const char *obtenido) {
    int ok = (obtenido != NULL) && (strcmp(esperado, obtenido) == 0);
    probadas++;
    if (!ok) fallidas++;
    printf("[%2d] %s\n", probadas, caso);
    printf("     esperado: %s\n", esperado);
    printf("     obtenido: %s\n", obtenido ? obtenido : "(NULL)");
    printf("     -> %s\n", ok ? "OK" : "FALLA");
}

int main(void) {
    printf("=== AUDITORIA SESION 05 - Recorrido postorden y RPN ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { ASTNode a = {NODO_NUM, 42, 0, NULL, NULL}; char b[64]; postorden(&a, b, 64); verificar_str("hoja 42", "42", b); }
    { ASTNode a = {NODO_NUM, 3, 0, NULL, NULL}; ASTNode b = {NODO_NUM, 5, 0, NULL, NULL}; ASTNode c = {NODO_NUM, 2, 0, NULL, NULL}; ASTNode m = {NODO_BINOP, 0, '*', &b, &c}; ASTNode s = {NODO_BINOP, 0, '+', &a, &m}; char buf[64]; postorden(&s, buf, 64); verificar_str("3+(5*2)", "3 5 2 * +", buf); }
    { char b[16]; postorden(NULL, b, 16); verificar_str("arbol NULL", "", b); }
    { ASTNode a = {NODO_NUM, 1, 0, NULL, NULL}; ASTNode b = {NODO_NUM, 2, 0, NULL, NULL}; ASTNode c = {NODO_NUM, 3, 0, NULL, NULL}; ASTNode d = {NODO_NUM, 4, 0, NULL, NULL}; ASTNode s1 = {NODO_BINOP, 0, '+', &a, &b}; ASTNode s2 = {NODO_BINOP, 0, '+', &c, &d}; ASTNode m = {NODO_BINOP, 0, '*', &s1, &s2}; char buf[64]; postorden(&m, buf, 64); verificar_str_largo("(1+2)*(3+4)", "1 2 + 3 4 + *", buf); }
    { ASTNode a = {NODO_NUM, 8, 0, NULL, NULL}; ASTNode b = {NODO_NUM, 3, 0, NULL, NULL}; ASTNode r = {NODO_BINOP, 0, '-', &a, &b}; char buf[32]; postorden(&r, buf, 32); verificar_str("8-3", "8 3 -", buf); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion05.md" "\n");
    return 0;
}
