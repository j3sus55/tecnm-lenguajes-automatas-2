# Bitácora de Requerimientos y Auditoría IA — Sesión 18

**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  

**Tema:** Arreglos y Verificación de Límites Estáticos

La función toma los anchos de una tabla, rechaza tipos desconocidos y dimensiones negativas, y comprueba que el producto quepa en `long` antes de multiplicar. Un índice es válido si es mayor o igual a cero y menor que el número de elementos.

La implementación se preparó con asistencia de IA.

[Código probado](../laboratorios/sesion18/artesano_sesion18_c.c) · [Harness oficial](../laboratorios/sesion18/harness_sesion18_c.c)

## Evidencia de ejecución

Compilación con MSVC en modo C11 y advertencias tratadas como errores.

```text
=== AUDITORIA SESION 18 - Tamano de arreglo y limites ===
Caso                               esperado:              obtenido:              resultado
[ 1] int[10]                            esperado: 40           obtenido: 40           OK
[ 2] double[3]                          esperado: 24           obtenido: 24           OK
[ 3] int[-4]                            esperado: -1           obtenido: -1           OK
[ 4] indice 0 de 5                      esperado: 1            obtenido: 1            OK
[ 5] indice 5 de 5                      esperado: 0            obtenido: 0            OK
[ 6] indice negativo                    esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion18.md
```

---
**Firma digital:** `j3sus55`
