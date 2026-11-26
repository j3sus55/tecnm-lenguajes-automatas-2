/* api_sesion07.h - contrato de la Sesion 07
 * Evaluador recursivo con detección de errores
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION07_H
#define API_SESION07_H

typedef enum { NODO_NUM, NODO_BINOP } TipoNodo;
typedef struct ASTNode {
    TipoNodo tipo;
    int valor;
    char op;
    struct ASTNode *izq;
    struct ASTNode *der;
} ASTNode;

int evaluar(ASTNode *n, int *error);

#endif /* API_SESION07_H */
