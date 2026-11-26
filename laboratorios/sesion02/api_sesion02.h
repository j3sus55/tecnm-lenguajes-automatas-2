/* api_sesion02.h - contrato de la Sesion 02
 * Lista enlazada de tokens en heap
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION02_H
#define API_SESION02_H

typedef struct NodoToken {
    char lexema[16];
    struct NodoToken *siguiente;
} NodoToken;

NodoToken *agregar_token(NodoToken *cabeza, const char *lexema);
int contar_tokens(NodoToken *cabeza);
void liberar_lista(NodoToken *cabeza);

#endif /* API_SESION02_H */
