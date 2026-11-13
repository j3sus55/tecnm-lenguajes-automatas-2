# Bitácora de Requerimientos y Auditoría IA — Sesión 09
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Reporte Formal de Errores Semánticos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C 

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Diagnóstico con línea y columna  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> El búfer pertenece al llamador. Con una capacidad de 10 bytes, nueve se pueden usar para texto y el último se reserva para el terminador. Por eso el diagnóstico completo de main.c se recorta a main.c:14. Una línea o columna igual a cero debe imprimirse como 0.

**Resolución paso a paso**

1. Comprobar que la salida exista y que la capacidad sea positiva.
2. Formar archivo, línea y columna con dos puntos como separadores y añadir el literal «: error: » seguido del mensaje.
3. Pasar la capacidad a snprintf. La función recorta el texto y termina la cadena cuando hay espacio.
4. Comprobar el resultado completo y el prefijo truncado con el harness.

**Código de referencia para la fase Artesano.** Elaborado con asistencia para mostrar el procedimiento.

```c
#include "api_sesion09.h"
#include <stdio.h>

void reportar(const char *archivo, int linea, int columna,
              const char *mensaje, char *salida, int cap) {
    if (salida == NULL || cap <= 0) {
        return;
    }

    snprintf(salida, (size_t)cap, "%s:%d:%d: error: %s",
             archivo, linea, columna, mensaje);
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C11, GCC y la biblioteca estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Formato exacto de GCC, truncamiento a cap−1 bytes y terminación NUL con capacidad positiva |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: C11 con GCC, usando api_sesion09.h. El harness aporta main.
Rol: Especialista en software de sistemas y compiladores.
Especificación: Implementa reportar con la firma original. Construye
archivo:linea:columna: error: mensaje por segmentos. Conserva hasta
cap-1 bytes y escribe el terminador si la salida es válida y cap > 0.
Con cap <= 0 o salida nula, retorna antes de escribir. Admite todo int
en línea y columna y conserva el texto del mensaje.
Output: Código modular sin main ni asignación dinámica y nota de
complejidad.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion09.h"
#include <limits.h>
#include <stdio.h>

static void agregar(char **cursor, size_t *restantes, const char *texto) {
    while (*restantes > 0 && *texto != '\0') {
        **cursor = *texto;
        ++*cursor;
        ++texto;
        --*restantes;
    }
}

void reportar(const char *archivo, int linea, int columna,
              const char *mensaje, char *salida, int cap) {
    if (salida == NULL || cap <= 0) {
        return;
    }

    char ubicacion[2 * sizeof(int) * CHAR_BIT + 4];
    int escritos = snprintf(ubicacion, sizeof ubicacion, ":%d:%d:",
                            linea, columna);
    if (escritos < 0) {
        salida[0] = '\0';
        return;
    }

    char *cursor = salida;
    size_t restantes = (size_t)cap - 1;
    agregar(&cursor, &restantes, archivo);
    agregar(&cursor, &restantes, ubicacion);
    agregar(&cursor, &restantes, " error: ");
    agregar(&cursor, &restantes, mensaje);
    *cursor = '\0';
}
```

---

La referencia Artesano usa una llamada a snprintf para todo el diagnóstico. La propuesta de IA formatea la ubicación y copia cuatro segmentos hasta llenar el espacio disponible. Con n como la longitud total del diagnóstico, el tiempo es O(n) para el formateo completo y O(min(n, cap)) para la copia por segmentos. Ambas usan O(1) de espacio auxiliar para el ancho fijo de int.

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Formato completo, posición cero y truncamiento; el harness evalúa el formateador.

### Salida del harness

Ambas versiones compilaron con -std=c11 -Wall -Wextra -Werror y produjeron la misma salida. Cada ejecución terminó con código 0.
```text
=== AUDITORIA SESION 09 - Diagnostico con linea y columna ===
Caso                               esperado:              obtenido:              resultado
[ 1] main.c:14:5
     esperado: main.c:14:5: error: tipo incompatible
     obtenido: main.c:14:5: error: tipo incompatible
     -> OK
[ 2] a.c:1:1
     esperado: a.c:1:1: error: x
     obtenido: a.c:1:1: error: x
     -> OK
[ 3] linea 0
     esperado: x.c:0:0: error: y
     obtenido: x.c:0:0: error: y
     -> OK
[ 4] cap=10 trunca                      esperado: main.c:14    obtenido: main.c:14    OK

RESULTADO: 4/4 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion09.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] Las pruebas de capacidad 0 y 1 conservaron los bytes testigo. El código no reserva memoria dinámica.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> El harness oficial cubre el truncamiento con cap = 10. El contrato requiere precisar los casos cap = 0 y cap = 1, porque un búfer sin espacio no admite terminador y uno de un byte solo admite el terminador. En la versión por segmentos, la guarda se aplica antes de convertir cap a size_t y restar uno. Las pruebas complementarias comprobaron cap = 0, cap = −1, cap = 1, salida nula y las posiciones INT_MIN e INT_MAX. Las cinco pasaron en ambas versiones.

---
**Firma digital:** @j3sus55
