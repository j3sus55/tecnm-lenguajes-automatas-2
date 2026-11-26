# Bitácora de Requerimientos y Auditoría IA — Sesión 12

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Atributos de Símbolos y Memoria  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Estructura de registro de símbolos.

*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en distinguir el desplazamiento de la variable del cursor que queda después de reservar su espacio. Para un `double` recibido con cursor 4, primero se debe avanzar al desplazamiento 8, guardar ese valor en el registro y devolver 16 como siguiente cursor. También hay que comprobar que el registro exista antes de acceder a sus campos y verificar que las operaciones de alineación y reserva no excedan el rango de `int`.

Implementación de referencia de la fase Artesano, elaborada con asistencia de IA. Archivo `artesano_sesion12_c.c`.

```c
#include "api_sesion12.h"
#include <limits.h>
#include <string.h>

int ancho_de_tipo(const char *tipo) {
    if (tipo == NULL) {
        return -1;
    }

    if (strcmp(tipo, "char") == 0) {
        return 1;
    }
    if (strcmp(tipo, "short") == 0) {
        return 2;
    }
    if (strcmp(tipo, "int") == 0) {
        return 4;
    }
    if (strcmp(tipo, "float") == 0) {
        return 4;
    }
    if (strcmp(tipo, "double") == 0) {
        return 8;
    }
    if (strcmp(tipo, "puntero") == 0) {
        return 8;
    }

    return -1;
}

int asignar_offset(Simbolo *simbolo, int cursor) {
    if (simbolo == NULL || cursor < 0) {
        return -1;
    }

    int ancho = ancho_de_tipo(simbolo->tipo);

    if (ancho < 0) {
        return -1;
    }

    int offset = cursor;

    while (offset % ancho != 0) {
        if (offset == INT_MAX) {
            return -1;
        }

        ++offset;
    }

    if (offset > INT_MAX - ancho) {
        return -1;
    }

    simbolo->offset = offset;

    return offset + ancho;
}
```

Esta implementación avanza byte por byte hasta encontrar una posición alineada. Los tamaños corresponden a la tabla fija del laboratorio.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| **Elemento C-R-E-O** | **Definición Técnica** |
| --- | --- |
| **Contexto (C)** | Lenguaje C99 y bibliotecas estándar. El harness compila con GCC usando `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Consultar los anchos mediante una tabla y calcular el relleno con aritmética modular. Guardar el desplazamiento alineado y devolver el cursor siguiente. |
| **Output (O)** | Código modular para `ia_sesion12_c.c` y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Desarrolla un módulo C que incluya api_sesion12.h y conserve
las firmas del contrato. El harness utiliza GCC con las opciones
-std=c11 -Wall -Wextra -Werror.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Implementa las funciones:
int ancho_de_tipo(const char *tipo);
int asignar_offset(Simbolo *simbolo, int cursor);

Para ancho_de_tipo, utiliza una tabla constante con estos valores:
char=1, short=2, int=4, float=4, double=8 y puntero=8.
Devuelve -1 cuando el nombre sea NULL o desconocido.

Para asignar_offset, valida el registro y rechaza cursores negativos.
Calcula mediante aritmética modular el relleno necesario para que
el offset sea múltiplo del ancho del tipo.
Guarda el offset alineado y devuelve offset + ancho.
Comprueba los desbordamientos antes de realizar las sumas.
Si ocurre un error, devuelve -1 y conserva el offset anterior.

Output:
Entrega el código de ia_sesion12_c.c sin main() y explica
la complejidad temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo

Archivo `ia_sesion12_c.c`.

```c
#include "api_sesion12.h"
#include <limits.h>
#include <string.h>

int ancho_de_tipo(const char *tipo) {
    static const struct {
        const char *nombre;
        int ancho;
    } tipos[] = {
        {"char", 1},
        {"short", 2},
        {"int", 4},
        {"float", 4},
        {"double", 8},
        {"puntero", 8}
    };

    if (tipo == NULL) {
        return -1;
    }

    for (unsigned int i = 0; i < sizeof tipos / sizeof tipos[0]; ++i) {
        if (strcmp(tipo, tipos[i].nombre) == 0) {
            return tipos[i].ancho;
        }
    }

    return -1;
}

int asignar_offset(Simbolo *simbolo, int cursor) {
    if (simbolo == NULL || cursor < 0) {
        return -1;
    }

    int ancho = ancho_de_tipo(simbolo->tipo);

    if (ancho < 0) {
        return -1;
    }

    int relleno = (ancho - cursor % ancho) % ancho;

    if (cursor > INT_MAX - relleno) {
        return -1;
    }

    int offset = cursor + relleno;

    if (offset > INT_MAX - ancho) {
        return -1;
    }

    simbolo->offset = offset;

    return offset + ancho;
}
```

Las funciones tienen tiempo `O(1)` y espacio auxiliar `O(1)` para el conjunto fijo de tipos del contrato. La búsqueda examina como máximo seis nombres de longitud acotada y el cálculo del relleno requiere un número fijo de operaciones.

En Artesano, la alineación necesita como máximo siete incrementos porque el mayor ancho permitido es ocho bytes.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Alineación de tipos y tamaños en bytes.

Las versiones Artesano e IA se comprobaron con el harness original, compilado con MSVC en modo C11 y las opciones `/W4 /WX`. Ambas terminaron con código de salida `0`.

Para repetir las pruebas con GCC en el laboratorio, se utilizan los siguientes comandos.

```sh
gcc -std=c11 -Wall -Wextra -Werror harness_sesion12_c.c artesano_sesion12_c.c -o test_artesano && ./test_artesano

gcc -std=c11 -Wall -Wextra -Werror harness_sesion12_c.c ia_sesion12_c.c -o test_ia && ./test_ia
```

### Salida del harness

Las dos implementaciones produjeron estas seis comprobaciones. El harness imprime su mensaje de cierre después del total mostrado.

```text
=== AUDITORIA SESION 12 - Registro de simbolo y offsets alineados ===
Caso                               esperado:              obtenido:              resultado
[ 1] ancho_de_tipo("int")               esperado: 4            obtenido: 4            OK
[ 2] ancho_de_tipo("double")            esperado: 8            obtenido: 8            OK
[ 3] int en cursor 0                    esperado: 4            obtenido: 4            OK
[ 4] char e int: offset de b            esperado: 4            obtenido: 4            OK
[ 5] double desde cursor 4              esperado: 8            obtenido: 8            OK
[ 6] registro NULL                      esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
```

- [x] Los seis casos del harness terminaron sin fallos.
- [x] La revisión del código confirma la validación del registro antes de escribir, el acceso acotado a la tabla y la ausencia de reservas dinámicas de memoria.
- [x] La complejidad teórica coincide con la implementación.
- [ ] Falta ejecutar pruebas complementarias con cursores negativos, tipos desconocidos y valores cercanos a `INT_MAX`.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> La auditoría identificó que el harness deja sin comprobar el desbordamiento de enteros al calcular desplazamientos cercanos a `INT_MAX`. Por ejemplo, reservar un `int` con un cursor que ya no permite sumar cuatro bytes debe producir un error. Ambas implementaciones incluyen verificaciones antes de sumar y guardan el nuevo offset únicamente después de validar toda la operación. La versión de IA entregada ya contempla esta protección. Su ejecución con entradas extremas queda pendiente como prueba complementaria.

---

**Firma digital:** `j3sus55`
