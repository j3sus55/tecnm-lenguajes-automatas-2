/* api_sesion03.h - contrato de la Sesion 03
 * Archivos: escribir y contar líneas
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION03_H
#define API_SESION03_H

int escribir_texto(const char *ruta, const char *texto);
int contar_lineas(const char *ruta);

#endif /* API_SESION03_H */
