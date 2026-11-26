/* api_sesion04.h - contrato de la Sesion 04
 * Nodo binario de AST
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION04_H
#define API_SESION04_H

typedef enum { NODO_NUM, NODO_BINOP } TipoNodo;
typedef struct ASTNode {
    TipoNodo tipo;
    int valor;
    char op;
    struct ASTNode *izq;
    struct ASTNode *der;
} ASTNode;

ASTNode *crear_numero(int v);
ASTNode *crear_binario(char op, ASTNode *izq, ASTNode *der);
int evaluar(ASTNode *n);

#endif /* API_SESION04_H */
