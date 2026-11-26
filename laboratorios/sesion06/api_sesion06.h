/* api_sesion06.h - contrato de la Sesion 06
 * Tabla de compatibilidad de tipos
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION06_H
#define API_SESION06_H

typedef enum { TIPO_ERROR = 0, TIPO_CHAR = 1, TIPO_INT = 2, TIPO_FLOAT = 3 } Tipo;

int compatible(Tipo destino, Tipo origen);
const char *nombre_tipo(Tipo t);

#endif /* API_SESION06_H */
