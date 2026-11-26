# Auditoría de la sesión 07

El evaluador recorre el árbol, propaga el primer error y devuelve código 1 al dividir o calcular el residuo entre cero. Un árbol inválido o un resultado fuera del rango de `int` genera el código 2. Usa tiempo O(n) y espacio O(h) en la pila.

La implementación de esta revisión se preparó con asistencia de IA. La ejecución de abajo verifica ese código.

## Ejecución del harness oficial

Pruebas ejecutadas con MSVC en C11 el 26 de noviembre de 2026, con advertencias tratadas como errores.

[Archivo Artesano](../laboratorios/sesion07/artesano_sesion07_c.c) · [Harness oficial](../laboratorios/sesion07/harness_sesion07_c.c)

### Archivo Artesano

```text
=== AUDITORIA SESION 07 - Evaluador recursivo con deteccion de errores ===
Caso                               esperado:              obtenido:              resultado
[ 1] (2+3)*4                            esperado: 20           obtenido: 20           OK
[ 2] 7/0 -> error 1                     esperado: 1            obtenido: 1            OK
[ 3] 7%0 -> error 1                     esperado: 1            obtenido: 1            OK
[ 4] arbol NULL -> error 2              esperado: 2            obtenido: 2            OK
[ 5] 10-2-3                             esperado: 5            obtenido: 5            OK
[ 6] (8/2)+1                            esperado: 5            obtenido: 5            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion07.md
```

### Versión IA

Esta segunda implementación se preparó con asistencia de IA. La salida se capturó con el harness oficial y MSVC en modo C11, con advertencias tratadas como errores.

[Código de la segunda implementación](../laboratorios/sesion07/ia_sesion07_c.c)

```text
=== AUDITORIA SESION 07 - Evaluador recursivo con deteccion de errores ===
Caso                               esperado:              obtenido:              resultado
[ 1] (2+3)*4                            esperado: 20           obtenido: 20           OK
[ 2] 7/0 -> error 1                     esperado: 1            obtenido: 1            OK
[ 3] 7%0 -> error 1                     esperado: 1            obtenido: 1            OK
[ 4] arbol NULL -> error 2              esperado: 2            obtenido: 2            OK
[ 5] 10-2-3                             esperado: 5            obtenido: 5            OK
[ 6] (8/2)+1                            esperado: 5            obtenido: 5            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion07.md
```
