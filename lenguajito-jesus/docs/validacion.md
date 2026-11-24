# Validación reproducible
Fecha: 2026-11-05. Revisión técnica: `058a674ce940ec9ff71e375545306224c8aeb3e5`.
[Workflow exitoso](https://github.com/j3sus55/tecnm-lenguajes-automatas-2/actions/runs/42102391957).
Entorno: Ubuntu x86-64, GCC, GNU as y Python 3.

```text
Hito 1: 4/4 pruebas superadas
Hito 2: 14/14 pruebas superadas
Hito 3: 15/15 pruebas superadas
Hito 4: 16/16 pruebas superadas
todos: 20/20 pruebas superadas
Propias: 24/24 pruebas superadas
```

Compilación C11 con -Wall -Wextra -Werror -O2 sin advertencias.
Segunda compilación con AddressSanitizer y UndefinedBehaviorSanitizer:
20/20 oficiales y 24/24 propias; sin errores reportados.

`file entrega/anidado`:
```text
ELF 64-bit LSB pie executable, x86-64, version 1 (SYSV), dynamically linked
```
readelf confirmó ELF64 y máquina x86-64. El binario se ejecutó y su salida
coincidió mediante diff con tests/validos/08_anidado.out.

Artefacto descargable en el workflow: **lenguajito-linux-x86-64**.
Incluye lenguajito, anidado.s, anidado.o, anidado y anidado.out.
Los artefactos de Actions caducan; se pueden regenerar con los comandos de demo.md.

## Auditoría de la primera ejecución
La compilación y todas las pruebas doradas pasaron. Una prueba propia encontró
que, después de consumir el punto y coma de una declaración inválida, el parser
descartaba la siguiente sentencia durante la recuperación.
Se corrigió sequence() para no sincronizar de nuevo si ya consumió un delimitador.
La segunda revisión pasó la suite completa y los sanitizadores.
No se cambiaron las pruebas para ocultar el fallo.

Estos resultados son evidencia automatizada, no sustituyen la revisión humana
ni demuestran que se hayan presentado las exposiciones.
