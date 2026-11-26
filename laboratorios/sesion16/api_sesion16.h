/* api_sesion16.h - contrato de la Sesion 16
 * Registro de prototipos y aridad
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION16_H
#define API_SESION16_H

typedef struct Funcion {
    const char *nombre;
    int aridad;
} Funcion;

int verificar_llamada(const Funcion *catalogo, int total, const char *nombre, int nargs);

#endif /* API_SESION16_H */
