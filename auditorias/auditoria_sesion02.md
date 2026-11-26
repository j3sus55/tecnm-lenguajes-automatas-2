# Auditoría de la sesión 02

Cada nodo almacena hasta 15 caracteres más el terminador. La inserción conserva el orden y la liberación guarda el enlace siguiente antes de llamar a `free`. El harness comprueba el contrato funcional. La ejecución no incluye un detector de fugas.

La implementación de esta revisión se preparó con asistencia de IA. La ejecución de abajo verifica ese código.

## Ejecución del harness oficial

Pruebas ejecutadas con MSVC en C11 el 26 de noviembre de 2026, con advertencias tratadas como errores. Se verificó el archivo indicado por el laboratorio. Queda pendiente documentar una comparación con una implementación independiente.

[Archivo Artesano](../laboratorios/sesion02/artesano_sesion02_c.c) · [Harness oficial](../laboratorios/sesion02/harness_sesion02_c.c)

### Archivo Artesano

```text
=== AUDITORIA SESION 02 - Lista enlazada de tokens en heap ===
Caso                               esperado:              obtenido:              resultado
[ 1] agregar_token(NULL,"if")           esperado: if           obtenido: if           OK
[ 2] contar_tokens(3 tokens)            esperado: 3            obtenido: 3            OK
[ 3] orden: segundo token               esperado: dos          obtenido: dos          OK
[ 4] ultimo->siguiente == NULL          esperado: 1            obtenido: 1            OK
[ 5] contar_tokens(NULL)                esperado: 0            obtenido: 0            OK
[ 6] liberar_lista(NULL)                esperado: sin crash    obtenido: sin crash    OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion02.md
```
