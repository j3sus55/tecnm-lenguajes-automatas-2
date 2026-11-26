# Bitácora de Requerimientos y Auditoría IA — Sesión 16

**Tema:** Firmas de Función y Parámetros Formales

La función recorre el catálogo y compara el nombre con `strcmp`. Devuelve `1` si coincide la aridad, `0` si difiere y `-1` si no hay un nombre válido registrado. La búsqueda usa espacio auxiliar constante.

La implementación se preparó con asistencia de IA.

[Código probado](../laboratorios/sesion16/artesano_sesion16_c.c) · [Harness oficial](../laboratorios/sesion16/harness_sesion16_c.c)

## Evidencia de ejecución

Compilación con MSVC en modo C11 y advertencias tratadas como errores.

```text
=== AUDITORIA SESION 16 - Registro de prototipos y aridad ===
Caso                               esperado:              obtenido:              resultado
[ 1] suma con 3 argumentos              esperado: 1            obtenido: 1            OK
[ 2] suma con 2 argumentos              esperado: 0            obtenido: 0            OK
[ 3] suma con 4 argumentos              esperado: 0            obtenido: 0            OK
[ 4] funcion no registrada              esperado: -1           obtenido: -1           OK
[ 5] main sin argumentos                esperado: 1            obtenido: 1            OK
[ 6] nombre NULL                        esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion16.md
```
