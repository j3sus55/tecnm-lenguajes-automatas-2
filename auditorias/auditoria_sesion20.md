# Bitácora de Requerimientos y Auditoría IA — Sesión 20

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Hito 2: Analizador Semántico y Scopes  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Integración de Tabla de Símbolos y AST  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en distinguir la resolución de un nombre del conteo de identificadores visibles. Para resolver un nombre hay que recorrer los ámbitos desde el actual hacia la raíz y detenerse en la declaración más cercana. Para contar los visibles hay que evitar sumar varias veces el mismo identificador cuando aparece en diferentes niveles.
>
> Otra restricción es que `AMBITO_MAX` limita los símbolos de cada ámbito. Una cadena de varios ámbitos puede contener más de ocho nombres distintos, por lo que un arreglo auxiliar de ocho posiciones sería insuficiente para guardar todos los visibles.

Implementación de referencia asistida por IA para `artesano_sesion20_c.c`.

```c
#include "api_sesion20.h"
#include <string.h>

int profundidad_de(const Ambito *actual, const char *nombre) {
    if (actual == NULL || nombre == NULL) {
        return -2;
    }

    int nivel = 0;

    for (const Ambito *p = actual; p != NULL; p = p->padre) {
        for (int i = 0; i < p->total; ++i) {
            if (strcmp(p->simbolos[i], nombre) == 0) {
                return nivel;
            }
        }

        ++nivel;
    }

    return -1;
}

static int visto_antes(const Ambito *inicio, const Ambito *limite,
                       int posicion) {
    const char *nombre = limite->simbolos[posicion];

    for (const Ambito *p = inicio; p != NULL; p = p->padre) {
        int fin = p->total;

        if (p == limite) {
            fin = posicion;
        }

        for (int i = 0; i < fin; ++i) {
            if (strcmp(p->simbolos[i], nombre) == 0) {
                return 1;
            }
        }

        if (p == limite) {
            break;
        }
    }

    return 0;
}

int contar_visibles(const Ambito *actual) {
    if (actual == NULL) {
        return -2;
    }

    int visibles = 0;

    for (const Ambito *p = actual; p != NULL; p = p->padre) {
        for (int i = 0; i < p->total; ++i) {
            if (!visto_antes(actual, p, i)) {
                ++visibles;
            }
        }
    }

    return visibles;
}
```

La implementación recibe ámbitos inicializados con `total` entre cero y `AMBITO_MAX`, cadenas válidas en las entradas activas y enlaces padre que terminan en NULL. La profundidad y el resultado del conteo deben caber en int.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Contrato de Validación Semántica Integral. Resolver nombres desde el ámbito más interno y contar identificadores distintos respetando shadowing. |
| **Output (O)** | Solo código modular y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion20.h.
Los ámbitos están inicializados, sus entradas activas contienen
cadenas válidas y los enlaces padre terminan en NULL.
La profundidad y el conteo son representables mediante int.
El harness utiliza -std=c11 -Wall -Wextra -Werror.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva las firmas:
int profundidad_de(const Ambito *actual, const char *nombre);
int contar_visibles(const Ambito *actual);

Implementa profundidad_de mediante recursión.
Busca primero en el ámbito actual y después en su padre.
Devuelve cero para una declaración local y aumenta la profundidad
por cada nivel recorrido.
Devuelve -1 si el nombre está ausente y -2 si algún argumento es NULL.
Compara los nombres completos mediante strcmp.

Implementa contar_visibles obteniendo recursivamente el conteo del padre.
Agrega un nombre local únicamente si no aparece en los padres
ni en las entradas anteriores del mismo ámbito.
Una declaración que oculta otra conserva la cantidad de nombres distintos.
Devuelve -2 para actual NULL y cero para una cadena sin símbolos.

Conserva los datos de los ámbitos y evita reservas dinámicas.
Respeta AMBITO_MAX como capacidad individual de cada ámbito.

Output:
Entrega ia_sesion20_c.c sin main() y explica la complejidad
temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion20.h"
#include <string.h>

int profundidad_de(const Ambito *actual, const char *nombre) {
    if (actual == NULL || nombre == NULL) {
        return -2;
    }

    for (int i = 0; i < actual->total; ++i) {
        if (strcmp(actual->simbolos[i], nombre) == 0) {
            return 0;
        }
    }

    if (actual->padre == NULL) {
        return -1;
    }

    int nivel = profundidad_de(actual->padre, nombre);

    if (nivel < 0) {
        return nivel;
    }

    return nivel + 1;
}

int contar_visibles(const Ambito *actual) {
    if (actual == NULL) {
        return -2;
    }

    int visibles = 0;

    if (actual->padre != NULL) {
        visibles = contar_visibles(actual->padre);
    }

    for (int i = 0; i < actual->total; ++i) {
        int repetido = 0;

        for (int j = 0; j < i; ++j) {
            if (strcmp(actual->simbolos[j], actual->simbolos[i]) == 0) {
                repetido = 1;
                break;
            }
        }

        if (!repetido &&
            (actual->padre == NULL ||
             profundidad_de(actual->padre, actual->simbolos[i]) == -1)) {
            ++visibles;
        }
    }

    return visibles;
}
```

Sea `h` la cantidad de ámbitos y `L` la longitud máxima de los nombres, considerando al menos una operación por comparación.

Como cada ámbito contiene como máximo ocho símbolos, `profundidad_de` tiene tiempo `O(h · L)` y `contar_visibles` tiene tiempo `O(h² · L)` en el peor caso.

La implementación iterativa de Artesano utiliza espacio auxiliar `O(1)`. La versión recursiva de IA utiliza espacio auxiliar `O(h)` por las llamadas pendientes.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Casos complejos de shadowing y ámbito

Ambas implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Las dos superaron los seis casos del harness original y terminaron con código de salida cero.

También se ejecutaron pruebas complementarias de argumentos NULL, ámbitos vacíos, ámbitos intermedios sin símbolos, nombres repetidos dentro del mismo ámbito, shadowing en tres niveles, comparación exacta de nombres y dieciséis identificadores distintos distribuidos entre dos ámbitos completos. Todas terminaron correctamente.

Queda pendiente repetir las pruebas con GCC y evaluar cadenas de gran profundidad, especialmente por el consumo de pila de la versión recursiva.

### Salida del harness

Salida obtenida en ambas implementaciones, presentada hasta el total de pruebas. El programa imprime después su mensaje de cierre.

```text
=== AUDITORIA SESION 20 - Resolucion de nombres y visibilidad ===
Caso                               esperado:              obtenido:              resultado
[ 1] nombre en el ambito actual         esperado: 0            obtenido: 0            OK
[ 2] nombre dos niveles arriba          esperado: 2            obtenido: 2            OK
[ 3] shadowing gana el interno          esperado: 0            obtenido: 0            OK
[ 4] nombre no visible                  esperado: -1           obtenido: -1           OK
[ 5] visibles sin repetir               esperado: 3            obtenido: 3            OK
[ 6] ambito NULL                        esperado: -2           obtenido: -2           OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites superados sin *Crash* en las pruebas ejecutadas.
* [x] Los accesos permanecen dentro de las entradas activas de ámbitos válidos. Ambas implementaciones carecen de reservas dinámicas de memoria.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> El harness original deja sin evaluar una cadena con más de ocho nombres visibles. Una solución que use `AMBITO_MAX` como límite global puede perder símbolos o escribir fuera de un arreglo auxiliar. Se agregó una prueba con dos ámbitos completos y dieciséis nombres distintos, y ambas implementaciones devolvieron dieciséis.
>
> También se comprobó que un identificador repetido en tres niveles tenga profundidad cero desde el ámbito más interno y contribuya una sola vez al conteo. La versión de IA entregada ya contempla estas condiciones, por lo que la auditoría amplió la cobertura de pruebas sin requerir una corrección del código.

---

**Firma digital:** `j3sus55`
