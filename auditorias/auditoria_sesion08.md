# Bitácora de Requerimientos y Auditoría IA — Sesión 08
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Inferencia y Promoción de Tipos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Reglas de casteo implícito  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> La dificultad es que `TIPO_ERROR` vale 0, pero no representa un tipo numérico angosto. Primero se comprueba ese centinela. Después se aplica la jerarquía `CHAR < INT < FLOAT` mediante condiciones explícitas. La pérdida se determina según el tipo de origen: de `FLOAT` a `INT` o `CHAR`, y de `INT` a `CHAR`. La interfaz fija las firmas y el harness aporta `main()`.

**Resolución paso a paso.** `CHAR + INT` produce `INT`; `INT + FLOAT` produce `FLOAT`; `FLOAT + FLOAT` conserva `FLOAT`; `ERROR + INT` devuelve `ERROR`. La conversión de `INT` a `CHAR` da 1 y la conversión de `CHAR` a `INT` da 0.

**Implementación de referencia para el apartado Artesano.** Preparada con asistencia para explicar el razonamiento manual. No constituye evidencia de una ejecución de la fase sin IA.

```c
#include "api_sesion08.h"

Tipo promover(Tipo a, Tipo b) {
    if (a == TIPO_ERROR || b == TIPO_ERROR) {
        return TIPO_ERROR;
    }
    if (a == TIPO_FLOAT || b == TIPO_FLOAT) {
        return TIPO_FLOAT;
    }
    if (a == TIPO_INT || b == TIPO_INT) {
        return TIPO_INT;
    }
    return TIPO_CHAR;
}

int hay_perdida(Tipo destino, Tipo origen) {
    switch (origen) {
        case TIPO_FLOAT:
            return destino == TIPO_INT || destino == TIPO_CHAR;
        case TIPO_INT:
            return destino == TIPO_CHAR;
        default:
            return 0;
    }
}

const char *nombre_tipo(Tipo t) {
    switch (t) {
        case TIPO_CHAR: return "char";
        case TIPO_INT: return "int";
        case TIPO_FLOAT: return "float";
        default: return "error";
    }
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C11 con GCC y `-Wall -Wextra -Werror` |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Promoción por rango, detección de estrechamiento y propagación de `TIPO_ERROR` |
| **Output (O)** | Solo código modular y análisis de complejidad |

### 2.2 Prompt Estructurado
```text
Contexto: Laboratorio de C11. Incluye api_sesion08.h. El harness compila
con GCC, -Wall -Wextra -Werror, y ya define main.
Rol: Especialista en software de sistemas y compiladores.
Especificación: Implementa promover, hay_perdida y nombre_tipo con las
firmas del encabezado. Usa los rangos del enum (CHAR < INT < FLOAT) y
propaga ERROR desde cualquier operando al promover. Reporta pérdida
cuando dos tipos numéricos válidos se convierten hacia un rango menor.
Trata ERROR como centinela fuera de las conversiones numéricas.
Usa una tabla estática de nombres con comprobación de límites.
Salida: Código modular en C, sin main ni memoria dinámica, y una nota
sobre complejidad de tiempo y espacio.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion08.h"

Tipo promover(Tipo a, Tipo b) {
    if (a == TIPO_ERROR || b == TIPO_ERROR) {
        return TIPO_ERROR;
    }
    return a > b ? a : b;
}

int hay_perdida(Tipo destino, Tipo origen) {
    return destino != TIPO_ERROR && origen != TIPO_ERROR && destino < origen;
}

const char *nombre_tipo(Tipo t) {
    static const char *const nombres[] = {
        [TIPO_ERROR] = "error",
        [TIPO_CHAR] = "char",
        [TIPO_INT] = "int",
        [TIPO_FLOAT] = "float"
    };

    if (t < TIPO_ERROR || t > TIPO_FLOAT) {
        return nombres[TIPO_ERROR];
    }
    return nombres[t];
}
```

---

La versión Artesano explicita las reglas mediante ramas por tipo. La propuesta de IA usa el orden del enum para elegir el mayor y consulta una tabla estática de nombres. Ambas tienen tiempo y espacio auxiliar `O(1)`.

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Mezcla de tipos enteros y flotantes

### Salida del harness

Se compiló y ejecutó el mismo harness por separado con cada archivo C. Ambos procesos terminaron con código 0 y produjeron la misma salida literal.
```text
=== AUDITORIA SESION 08 - Reglas de promocion y perdida ===
Caso                               esperado:              obtenido:              resultado
[ 1] promover(CHAR,INT)                 esperado: int          obtenido: int          OK
[ 2] promover(INT,FLOAT)                esperado: float        obtenido: float        OK
[ 3] promover(FLOAT,FLOAT)              esperado: float        obtenido: float        OK
[ 4] promover(ERROR,INT)                esperado: error        obtenido: error        OK
[ 5] hay_perdida(CHAR,INT)              esperado: 1            obtenido: 1            OK
[ 6] hay_perdida(INT,CHAR)              esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion08.md
```

* [x] Caso base y límites superados sin *Crash*.
* [x] Revisión del código: la tabla comprueba el índice y ninguna versión reserva memoria dinámica. El harness no incluye sanitizadores.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> El harness no prueba `hay_perdida(TIPO_ERROR, TIPO_FLOAT)`. Si se comparan los valores del enum sin filtrar el centinela, `0 < 3` produciría un aviso de estrechamiento para una conversión inválida. La versión de IA comprueba ambos operandos antes de comparar los rangos. También revisé el alcance de la regla: `hay_perdida(FLOAT, INT)` devuelve 0 según la jerarquía del ejercicio. La función trabaja con tipos, así que una posible pérdida de precisión de un entero concreto queda fuera de su contrato.

---
**Firma digital:** `j3sus55`

## Ejecución del harness oficial

Salida capturada el 26 de noviembre de 2026 con MSVC en modo C11.

**Artesano**

```text
=== AUDITORIA SESION 08 - Reglas de promocion y perdida ===
Caso                               esperado:              obtenido:              resultado
[ 1] promover(CHAR,INT)                 esperado: int          obtenido: int          OK
[ 2] promover(INT,FLOAT)                esperado: float        obtenido: float        OK
[ 3] promover(FLOAT,FLOAT)              esperado: float        obtenido: float        OK
[ 4] promover(ERROR,INT)                esperado: error        obtenido: error        OK
[ 5] hay_perdida(CHAR,INT)              esperado: 1            obtenido: 1            OK
[ 6] hay_perdida(INT,CHAR)              esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion08.md
```

**Científico (IA)**

```text
=== AUDITORIA SESION 08 - Reglas de promocion y perdida ===
Caso                               esperado:              obtenido:              resultado
[ 1] promover(CHAR,INT)                 esperado: int          obtenido: int          OK
[ 2] promover(INT,FLOAT)                esperado: float        obtenido: float        OK
[ 3] promover(FLOAT,FLOAT)              esperado: float        obtenido: float        OK
[ 4] promover(ERROR,INT)                esperado: error        obtenido: error        OK
[ 5] hay_perdida(CHAR,INT)              esperado: 1            obtenido: 1            OK
[ 6] hay_perdida(INT,CHAR)              esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion08.md
```
