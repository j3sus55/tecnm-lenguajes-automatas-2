/* api_sesion11.h - contrato de la Sesion 11
 * Tabla hash con encadenamiento
 *
 * Esta cabecera es el contrato: el harness y tu implementacion
 * deben incluirla. Si cambias una firma, el harness dejara de compilar.
 */
#ifndef API_SESION11_H
#define API_SESION11_H

typedef struct Simbolo {
    char nombre[16];
    int tipo;
    int offset;
    struct Simbolo *sig;
} Simbolo;

unsigned int hash_djb2(const char *clave);
int insertar_simbolo(Simbolo *tabla[], unsigned int cubetas, const char *nombre, int tipo, int offset);

#endif /* API_SESION11_H */
