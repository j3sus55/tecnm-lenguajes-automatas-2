/* harness_sesion07_c.c - auditoria automatica de la Sesion 07
 *
 * Compilacion:
 *   gcc -std=c11 -Wall -Wextra -Werror harness_sesion07_c.c artesano_sesion07_c.c -o test_artesano
 *
 * El harness NO aprueba nada por si solo: cada linea se calcula
 * ejecutando tu implementacion y comparandola contra el valor esperado.
 * Si algun caso falla, el programa termina con codigo de salida 1.
 */
#include <stdio.h>
#include <string.h>
#include "api_sesion07.h"

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
    printf("=== AUDITORIA SESION 07 - Evaluador recursivo con deteccion de errores ===\n");
    printf("Caso                               esperado:              obtenido:              resultado\n");

    { ASTNode a={NODO_NUM,2,0,NULL,NULL}, b={NODO_NUM,3,0,NULL,NULL}, c={NODO_NUM,4,0,NULL,NULL}; ASTNode s={NODO_BINOP,0,'+',&a,&b}, m={NODO_BINOP,0,'*',&s,&c}; int e=0; int r=evaluar(&m,&e); (void)e; verificar_int("(2+3)*4", (long)(20), (long)(r)); }
    { ASTNode a={NODO_NUM,7,0,NULL,NULL}, b={NODO_NUM,0,0,NULL,NULL}; ASTNode d={NODO_BINOP,0,'/',&a,&b}; int e=0; int r=evaluar(&d,&e); (void)r; verificar_int("7/0 -> error 1", (long)(1), (long)(e)); }
    { ASTNode a={NODO_NUM,7,0,NULL,NULL}, b={NODO_NUM,0,0,NULL,NULL}; ASTNode d={NODO_BINOP,0,'%',&a,&b}; int e=0; int r=evaluar(&d,&e); (void)r; verificar_int("7%0 -> error 1", (long)(1), (long)(e)); }
    { int e=0; int r=evaluar(NULL,&e); (void)r; verificar_int("arbol NULL -> error 2", (long)(2), (long)(e)); }
    { ASTNode a={NODO_NUM,10,0,NULL,NULL}, b={NODO_NUM,2,0,NULL,NULL}, c={NODO_NUM,3,0,NULL,NULL}; ASTNode s={NODO_BINOP,0,'-',&a,&b}, r2={NODO_BINOP,0,'-',&s,&c}; int e=0; int r=evaluar(&r2,&e); (void)e; verificar_int("10-2-3", (long)(5), (long)(r)); }
    { ASTNode a={NODO_NUM,8,0,NULL,NULL}, b={NODO_NUM,2,0,NULL,NULL}, c={NODO_NUM,1,0,NULL,NULL}; ASTNode d={NODO_BINOP,0,'/',&a,&b}, s={NODO_BINOP,0,'+',&d,&c}; int e=0; int r=evaluar(&s,&e); (void)e; verificar_int("(8/2)+1", (long)(5), (long)(r)); }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion07.md" "\n");
    return 0;
}
