# Bitácora de Requerimientos y Auditoría IA — Sesión 14

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Declaración vs Uso de Variables  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Detección de variables no declaradas.

*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en mantener la regla de declaración previa sin alterar la tabla durante una consulta. `usar` debe responder si el nombre existe y `declarar` debe incrementar el contador únicamente cuando inserta un identificador nuevo. Para detectar duplicados se necesita comparar el contenido completo de las cadenas mediante `strcmp`, porque dos direcciones distintas pueden contener el mismo nombre. Además, `"x"` y `"xy"` deben reconocerse como identificadores diferentes.
>
> La tabla tiene capacidad para dieciséis nombres. Antes de insertar hay que comprobar ese límite. Como se almacenan punteros, las cadenas deben permanecer válidas y conservar su contenido mientras estén registradas.

Implementación de referencia asistida por IA para `artesano_sesion14_c.c`. Cada función realiza su propio recorrido de la tabla desde la primera entrada.

```c
#include "api_sesion14.h"
#include <string.h>

int declarar(Tabla *tabla, const char *nombre) {
    if (tabla == NULL || nombre == NULL || nombre[0] == '\0') {
        return -1;
    }

    if (tabla->total < 0 || tabla->total > TABLA_MAX) {
        return -1;
    }

    for (int i = 0; i < tabla->total; ++i) {
        if (strcmp(tabla->nombres[i], nombre) == 0) {
            return 0;
        }
    }

    if (tabla->total == TABLA_MAX) {
        return -1;
    }

    tabla->nombres[tabla->total] = nombre;
    ++tabla->total;

    return 1;
}

int usar(const Tabla *tabla, const char *nombre) {
    if (tabla == NULL || nombre == NULL || nombre[0] == '\0') {
        return 0;
    }

    if (tabla->total < 0 || tabla->total > TABLA_MAX) {
        return 0;
    }

    for (int i = 0; i < tabla->total; ++i) {
        if (strcmp(tabla->nombres[i], nombre) == 0) {
            return 1;
        }
    }

    return 0;
}
```

Se recibe una tabla inicializada, con `total` indicando cuántas entradas contienen cadenas válidas.

El contrato establece la capacidad, pero deja sin especificar el retorno al intentar insertar un nombre nuevo en una tabla llena. En esta implementación se adopta `-1`, conservando el contenido y el contador.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Aplicar el invariante de definición previa. Comparar identificadores completos, rechazar redeclaraciones y conservar el contador ante operaciones rechazadas. Reutilizar `usar` para detectar duplicados en `declarar`. |
| **Output (O)** | Código modular para `ia_sesion14_c.c` y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion14.h.
La tabla llega inicializada y sus entradas activas contienen
cadenas válidas.
El harness utiliza -std=c11 -Wall -Wextra -Werror.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva las firmas:
int declarar(Tabla *tabla, const char *nombre);
int usar(const Tabla *tabla, const char *nombre);

Implementa usar con un recorrido desde la última entrada activa.
Compara los nombres completos mediante strcmp.
Devuelve 1 cuando el identificador está declarado y 0 cuando
está ausente, la tabla es NULL o el nombre es NULL o vacío.
La consulta debe conservar la tabla.

En declarar, valida primero los argumentos y el rango de total.
Reutiliza usar para detectar duplicados.
Devuelve 0 para un nombre existente y conserva total.
Inserta un nombre nuevo al final y devuelve 1.
Devuelve -1 para argumentos inválidos.

Respeta TABLA_MAX. Para un nombre nuevo en una tabla llena,
adopta -1 como retorno de error.
Comprueba los duplicados antes de rechazar por capacidad.
Almacena el puntero recibido y utiliza memoria estática de la tabla.

Output:
Entrega ia_sesion14_c.c sin main() y explica su complejidad.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion14.h"
#include <string.h>

int usar(const Tabla *tabla, const char *nombre) {
    if (tabla == NULL || nombre == NULL || nombre[0] == '\0') {
        return 0;
    }

    if (tabla->total < 0 || tabla->total > TABLA_MAX) {
        return 0;
    }

    int pendientes = tabla->total;

    while (pendientes > 0) {
        --pendientes;

        if (strcmp(tabla->nombres[pendientes], nombre) == 0) {
            return 1;
        }
    }

    return 0;
}

int declarar(Tabla *tabla, const char *nombre) {
    if (tabla == NULL || nombre == NULL || nombre[0] == '\0') {
        return -1;
    }

    if (tabla->total < 0 || tabla->total > TABLA_MAX) {
        return -1;
    }

    if (usar(tabla, nombre)) {
        return 0;
    }

    if (tabla->total == TABLA_MAX) {
        return -1;
    }

    tabla->nombres[tabla->total] = nombre;
    ++tabla->total;

    return 1;
}
```

Sea `n` el número de identificadores registrados y `L` la longitud máxima de las cadenas comparadas.

En ambas implementaciones, `usar` y `declarar` tienen tiempo `O(n · L)` en el peor caso y espacio auxiliar `O(1)`. El número de entradas está acotado por `TABLA_MAX`, cuyo valor es dieciséis.

La versión de IA concentra la búsqueda en `usar`, que también resuelve la detección de duplicados al declarar.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Uso antes de declarar y redeclaración.

Las dos implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Ambas superaron los seis casos del harness original y terminaron con código de salida `0`.

También se ejecutaron comprobaciones complementarias en ambas versiones:

- Declaración con tabla NULL, nombre NULL y nombre vacío.
- Uso con tabla NULL, nombre NULL y nombre vacío.
- Inserción de los dieciséis nombres permitidos.
- Rechazo de un nombre adicional al alcanzar la capacidad.
- Redeclaración de un nombre existente con la tabla llena.
- Consulta de la primera y la última entrada.
- Consulta de un nombre cuya inserción fue rechazada.
- Diferenciación entre `"xy"` y `"x"`.
- Detección de duplicados con cadenas de igual contenido almacenadas en direcciones distintas.
- Diferenciación entre `"xy"` y `"XY"`.
- Conservación del contador después de operaciones rechazadas.

Todas estas comprobaciones terminaron correctamente. Queda pendiente repetir la compilación y ejecución con GCC en el entorno del laboratorio.

```sh
gcc -std=c11 -Wall -Wextra -Werror harness_sesion14_c.c artesano_sesion14_c.c -o test_artesano && ./test_artesano

gcc -std=c11 -Wall -Wextra -Werror harness_sesion14_c.c ia_sesion14_c.c -o test_ia && ./test_ia
```

### Salida del harness

Ambas versiones produjeron las siguientes comprobaciones. El harness imprime su mensaje de cierre después del total mostrado.

```text
=== AUDITORIA SESION 14 - Declaracion previa y redeclaracion ===
Caso                               esperado:              obtenido:              resultado
[ 1] usar sin declarar                  esperado: 0            obtenido: 0            OK
[ 2] usar despues de declarar           esperado: 1            obtenido: 1            OK
[ 3] redeclarar el mismo nombre         esperado: 0            obtenido: 0            OK
[ 4] total tras redeclarar              esperado: 1            obtenido: 1            OK
[ 5] prefijo no es el mismo nombre      esperado: 0            obtenido: 0            OK
[ 6] declarar nombre vacio              esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites de capacidad superados sin *Crash* en las pruebas ejecutadas.
* [x] Los índices respetan el arreglo para tablas inicializadas con entradas válidas. Ambas implementaciones carecen de reservas dinámicas que requieran liberación.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> La auditoría identificó un caso que el harness original deja sin comprobar, comparar dos cadenas con el mismo contenido ubicadas en direcciones distintas. Comparar los punteros con `==` podría aceptar una redeclaración porque las direcciones difieren. Ambas implementaciones utilizan `strcmp`, y la prueba complementaria confirmó que un arreglo con el contenido `"xy"` se reconoce como duplicado de la cadena previamente registrada.
>
> También se comprobó el comportamiento de una tabla llena. Repetir un nombre existente devuelve `0`, mientras que insertar un nombre nuevo devuelve `-1` según la decisión adoptada para ese límite. En ambos casos el contador permanece en dieciséis. La versión de IA entregada ya contempla estas condiciones.

---

**Firma digital:** `j3sus55`
