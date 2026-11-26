/* api_sesion08.h - contrato de la Sesion 08
 * Reglas de promoción y pérdida
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION08_H
#define API_SESION08_H

typedef enum { TIPO_ERROR = 0, TIPO_CHAR = 1, TIPO_INT = 2, TIPO_FLOAT = 3 } Tipo;

Tipo promover(Tipo a, Tipo b);
int hay_perdida(Tipo destino, Tipo origen);
const char *nombre_tipo(Tipo t);

#endif /* API_SESION08_H */
