# Auditoría de la sesión 01

La función `invertir` modifica el búfer en el sitio, conserva el terminador y admite `NULL`. Usa tiempo O(n) y memoria auxiliar O(1).

La implementación de esta revisión se preparó con asistencia de IA. La ejecución de abajo verifica ese código.

## Ejecución del harness oficial

Pruebas ejecutadas con MSVC en C11 el 26 de noviembre de 2026, con advertencias tratadas como errores.

[Archivo Artesano](../laboratorios/sesion01/artesano_sesion01_c.c) · [Harness oficial](../laboratorios/sesion01/harness_sesion01_c.c)

### Archivo Artesano

```text
=== AUDITORIA SESION 01 - Inversion de buffer in-place ===
Caso                               esperado:              obtenido:              resultado
[ 1] invertir("HOLA")                   esperado: ALOH         obtenido: ALOH         OK
[ 2] invertir("abc")                    esperado: cba          obtenido: cba          OK
[ 3] invertir("")                       esperado:              obtenido:              OK
[ 4] invertir("AB")                     esperado: BA           obtenido: BA           OK
[ 5] invertir("a")                      esperado: a            obtenido: a            OK
[ 6] invertir(NULL)                     esperado: sin crash    obtenido: sin crash    OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion01.md
```

### Versión IA

Esta segunda implementación se preparó con asistencia de IA. La salida se capturó con el harness oficial y MSVC en modo C11, con advertencias tratadas como errores.

[Código de la segunda implementación](../laboratorios/sesion01/ia_sesion01_c.c)

```text
=== AUDITORIA SESION 01 - Inversion de buffer in-place ===
Caso                               esperado:              obtenido:              resultado
[ 1] invertir("HOLA")                   esperado: ALOH         obtenido: ALOH         OK
[ 2] invertir("abc")                    esperado: cba          obtenido: cba          OK
[ 3] invertir("")                       esperado:              obtenido:              OK
[ 4] invertir("AB")                     esperado: BA           obtenido: BA           OK
[ 5] invertir("a")                      esperado: a            obtenido: a            OK
[ 6] invertir(NULL)                     esperado: sin crash    obtenido: sin crash    OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion01.md
```
