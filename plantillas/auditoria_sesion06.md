# Bitácora de Requerimientos y Auditoría IA — Sesión 06

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Sistema de Tipos Primitivos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356
**Track:** C

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Chequeo de compatibilidad de tipos.

> Al pasar la matriz de compatibilidad a C, la fila debe representar el tipo destino y la columna el tipo origen. Si se invierten, se acepta `FLOAT → INT` y se rechaza `INT → FLOAT`. También hay que verificar los índices antes de acceder a la matriz. La fila y la columna de `TIPO_ERROR` contienen ceros para rechazar todas las asignaciones que lo involucren.

Implementación de referencia para `artesano_sesion06_c.c`, elaborada con asistencia de IA.

```c
#include "api_sesion06.h"

/* Filas: destino. Columnas: origen. Orden: ERROR, CHAR, INT, FLOAT. */
static const int tabla[TIPO_FLOAT + 1][TIPO_FLOAT + 1] = {
    {0, 0, 0, 0},
    {0, 1, 0, 0},
    {0, 1, 1, 0},
    {0, 1, 1, 1}
};

int compatible(Tipo destino, Tipo origen) {
    if ((unsigned int)destino > TIPO_FLOAT ||
        (unsigned int)origen > TIPO_FLOAT) {
        return 0;
    }

    return tabla[destino][origen];
}

const char *nombre_tipo(Tipo t) {
    static const char *const nombres[] = {"error", "char", "int", "float"};

    if ((unsigned int)t > TIPO_FLOAT) {
        return "desconocido";
    }

    return nombres[t];
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| **Elemento C-R-E-O** | **Definición Técnica** |
| --- | --- |
| **Contexto (C)** | Lenguaje C y cabecera `api_sesion06.h`. El harness del laboratorio usa GCC en modo C11. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Aplicar la jerarquía `CHAR < INT < FLOAT` mediante comparación de rangos. Rechazar `TIPO_ERROR` en ambos operandos y devolver el nombre de cada tipo mediante `switch`. |
| **Output (O)** | Código modular para `ia_sesion06_c.c` y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto: Implementa un módulo en C que incluya api_sesion06.h.
El harness lo compila con GCC y las opciones -std=c11 -Wall -Wextra -Werror.

Rol: Actúa como ingeniero de software de sistemas y compiladores.

Especificación: Conserva las firmas int compatible(Tipo destino, Tipo origen)
y const char *nombre_tipo(Tipo t). Valida que los dos operandos estén entre
TIPO_CHAR y TIPO_FLOAT. Después compara sus rangos directamente y devuelve 1
si el destino tiene un rango igual o mayor al origen. Devuelve 0 en los
demás casos. Usa switch para devolver "char", "int", "float" o "error"
según el tipo. Para cualquier otro valor devuelve "desconocido".

Output: Entrega el contenido de ia_sesion06_c.c sin main() y explica
su complejidad de tiempo y espacio.
```

---

## 3. Código Devuelto por el Modelo

Archivo `ia_sesion06_c.c`.

```c
#include "api_sesion06.h"

int compatible(Tipo destino, Tipo origen) {
    if (destino < TIPO_CHAR || destino > TIPO_FLOAT ||
        origen < TIPO_CHAR || origen > TIPO_FLOAT) {
        return 0;
    }

    return destino >= origen;
}

const char *nombre_tipo(Tipo t) {
    switch (t) {
        case TIPO_CHAR:
            return "char";
        case TIPO_INT:
            return "int";
        case TIPO_FLOAT:
            return "float";
        case TIPO_ERROR:
            return "error";
        default:
            return "desconocido";
    }
}
```

Las dos funciones usan tiempo `O(1)` y espacio auxiliar `O(1)`. `compatible` hace un número fijo de comparaciones y `nombre_tipo` selecciona un literal de cadena entre un conjunto fijo de opciones.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Asignaciones compatibles e incompatibles y nombres de tipos.

Las dos versiones produjeron la misma salida con el harness original, compilado con MSVC en modo C11. Ambas terminaron con código de salida `0`.

Para repetir las pruebas con GCC, ejecuta cada línea desde la carpeta que contiene el harness, la cabecera y las dos implementaciones.

```sh
gcc -std=c11 -Wall -Wextra -Werror harness_sesion06_c.c artesano_sesion06_c.c -o test_artesano && ./test_artesano
gcc -std=c11 -Wall -Wextra -Werror harness_sesion06_c.c ia_sesion06_c.c -o test_ia && ./test_ia
```

### Salida del harness

Se muestran las seis comprobaciones hasta el total. El harness imprime `TODO EN VERDE` después del resultado.

```text
=== AUDITORIA SESION 06 - Tabla de compatibilidad de tipos ===
Caso                               esperado:              obtenido:              resultado
[ 1] compatible(INT,INT)                esperado: 1            obtenido: 1            OK
[ 2] compatible(FLOAT,INT)              esperado: 1            obtenido: 1            OK
[ 3] compatible(INT,FLOAT)              esperado: 0            obtenido: 0            OK
[ 4] compatible(INT,ERROR)              esperado: 0            obtenido: 0            OK
[ 5] nombre_tipo(INT)                   esperado: int          obtenido: int          OK
[ 6] nombre_tipo(ERROR)                 esperado: error        obtenido: error        OK

RESULTADO: 6/6 pruebas superadas
```

- [x] Los casos base y las pruebas de límites terminaron sin fallos.
- [x] Artesano valida los índices antes de acceder a la matriz. Ambas implementaciones carecen de reservas dinámicas de memoria.
- [x] La complejidad teórica coincide con la implementación.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> La comparación `destino >= origen` por sí sola aceptaría `compatible(TIPO_ERROR, TIPO_ERROR)` porque ambos valores son iguales. El harness del laboratorio prueba `TIPO_ERROR` como origen de una asignación a `TIPO_INT`, así que agregué la comprobación de error en ambos operandos a las pruebas complementarias. La versión de IA cubre el caso al validar los dos rangos antes de compararlos. En ambas versiones también se verificaron las 16 combinaciones de tipos, los cuatro nombres y el valor fuera del rango `(Tipo)4`.

---

**Firma digital:** [Usuario de GitHub]
