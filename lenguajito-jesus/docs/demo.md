# Guion de demostración
Todos los comandos se ejecutan desde proyecto_integrador en Linux.
No constituye evidencia de una exposición ya realizada.

## Hito 1 — 3 minutos
1. (0:00–0:40) make; explicar lexer y posiciones.
2. (0:40–1:30) ./lenguajito tests/hito1/01_expresion.lj --tokens
3. (1:30–2:15) ./lenguajito tests/hito1/01_expresion.lj --ast
   Explicar por qué 2*3 queda debajo de la suma.
4. (2:15–3:00) make test-hito1; mostrar S01 del programa inválido.

## Hito 2 — 3 minutos
Mostrar tests/validos/05_shadowing.lj con --run y explicar los dos símbolos x.
Mostrar tests/invalidos/02_redeclaracion.lj con --check.
Explicar que la inicialización posterior a if requiere valor en ambas ramas.
Terminar con make test-hito2.

## Hito 3 — 3 minutos
Comandos:
```bash
./lenguajito tests/tac/01_folding.lj --ast
./lenguajito tests/tac/01_folding.lj --tac
./lenguajito tests/validos/04_while.lj --tac
make test-hito3
```
Comparar el AST con operaciones constantes con el TAC ya plegado.
En while identificar etiqueta de inicio, salto condicional, incremento y retorno.
Explicar la pasada de temporales muertos y por qué preserva asignaciones de variables.
La ausencia de temporales muertos por sí sola no demuestra la pasada: mostrar su
función dead_temporaries en tac.c.

## Hito 4 — 5 minutos
- 0:00–1:00: ./lenguajito tests/validos/08_anidado.lj --run
  Explicar el flujo del programa antes de ver la salida.
- 1:00–2:00: ./lenguajito tests/invalidos/01_no_declarada.lj --check
  Identificar ruta, línea, columna y E01.
- 2:00–3:00: mostrar el plegado con --ast y --tac como en Hito 3.
- 3:00–4:20: construir ELF:
```bash
./lenguajito tests/validos/08_anidado.lj --asm > /tmp/demo-lj.s
as --64 /tmp/demo-lj.s -o /tmp/demo-lj.o
gcc /tmp/demo-lj.o -o /tmp/demo-lj
file /tmp/demo-lj
readelf -h /tmp/demo-lj
/tmp/demo-lj
```
- 4:20–5:00: make test; mostrar evidencia y explicar que --run ejecuta as/gcc.

## Preguntas que ambos deben poder contestar
¿Por qué no basta con inicializar una variable dentro de while?
¿Cómo se diferencia una redeclaración de shadowing?
¿Por qué printf necesita la pila alineada?
¿Qué diferencia un AST, TAC y ensamblador?
¿Qué parte corrigió la auditoría de IA?
