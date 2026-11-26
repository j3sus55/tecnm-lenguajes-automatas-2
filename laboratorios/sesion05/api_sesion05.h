/* api_sesion05.h - contrato de la Sesion 05
 * Recorrido postorden y RPN
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION05_H
#define API_SESION05_H

typedef enum { NODO_NUM, NODO_BINOP } TipoNodo;
typedef struct ASTNode {
    TipoNodo tipo;
    int valor;
    char op;
    struct ASTNode *izq;
    struct ASTNode *der;
} ASTNode;

void postorden(ASTNode *n, char *buf, int cap);

#endif /* API_SESION05_H */
