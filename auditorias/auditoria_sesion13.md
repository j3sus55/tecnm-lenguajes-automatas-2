# Bitácora de Requerimientos y Auditoría IA — Sesión 13

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Manejo de Ámbitos (Scope Global/Local)  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Pila de tablas de símbolos (Stack Scopes).

*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en aplicar reglas distintas para declarar y buscar. Al declarar, se revisan los nombres del ámbito actual para permitir que un bloque interno tenga una variable con el mismo nombre que una global. Al buscar, se recorre primero el ámbito actual y después sus padres, devolviendo el nombre del primer ámbito que contiene el identificador. También hay que respetar el límite de ocho entradas para evitar escribir fuera del arreglo.
>
> Los nombres se almacenan mediante punteros. Por ello, las cadenas deben permanecer válidas mientras los ámbitos las utilicen.

Implementación de referencia asistida por IA para `artesano_sesion13_c.c`. Utiliza recorridos iterativos para revisar las declaraciones y subir por los ámbitos.

```c
#include "api_sesion13.h"
#include <string.h>

int declarar(Ambito *ambito, const char *nombre) {
    if (ambito == NULL || nombre == NULL || nombre[0] == '\0') {
        return -1;
    }

    if (ambito->total < 0 || ambito->total > AMBITO_MAX) {
        return -1;
    }

    for (int i = 0; i < ambito->total; ++i) {
        if (strcmp(ambito->nombres[i], nombre) == 0) {
            return 0;
        }
    }

    if (ambito->total == AMBITO_MAX) {
        return -1;
    }

    ambito->nombres[ambito->total] = nombre;
    ++ambito->total;

    return 1;
}

const char *buscar(const Ambito *ambito, const char *nombre) {
    if (nombre == NULL || nombre[0] == '\0') {
        return NULL;
    }

    while (ambito != NULL) {
        if (ambito->total < 0 || ambito->total > AMBITO_MAX) {
            return NULL;
        }

        for (int i = 0; i < ambito->total; ++i) {
            if (strcmp(ambito->nombres[i], nombre) == 0) {
                return ambito->nombre;
            }
        }

        ambito = ambito->padre;
    }

    return NULL;
}
```

Se reciben ámbitos inicializados con sus entradas activas válidas y una cadena de padres que termina en `NULL`, conforme al laboratorio.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Implementar ocultamiento de variables. Detectar duplicados dentro del ámbito actual y buscar desde el ámbito más interno hasta la raíz. Usar una función auxiliar de búsqueda local y recursión para recorrer los padres. |
| **Output (O)** | Código modular para `ia_sesion13_c.c` y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion13.h.
Los ámbitos llegan inicializados, sus entradas activas contienen
cadenas válidas y la cadena de padres termina en NULL.
El harness compila con -std=c11 -Wall -Wextra -Werror.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva estas firmas:
int declarar(Ambito *ambito, const char *nombre);
const char *buscar(const Ambito *ambito, const char *nombre);

Usa una función auxiliar estática que encuentre el índice de un
nombre dentro de un solo ámbito.

declarar debe devolver:
1 cuando inserta el nombre en el ámbito actual.
0 cuando el nombre ya está declarado en ese mismo ámbito.
-1 cuando recibe un ámbito NULL, un nombre NULL, un nombre vacío
o cuando intenta insertar un nombre nuevo en un ámbito lleno.

Comprueba los duplicados antes de rechazar una tabla llena.
Permite declarar un nombre que también exista en un ámbito padre.
Respeta AMBITO_MAX y valida el rango de total antes de indexar.
Almacena el puntero recibido sin reservar memoria dinámica.

buscar debe revisar primero el ámbito actual y después realizar
una llamada recursiva sobre el padre.
Devuelve el nombre del ámbito más cercano que contiene el
identificador. Devuelve NULL si no existe o la entrada es inválida.
La búsqueda debe conservar el contenido de los ámbitos.

Output:
Entrega ia_sesion13_c.c sin main() y explica su complejidad.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion13.h"
#include <string.h>

static int indice_local(const Ambito *ambito, const char *nombre) {
    for (int i = ambito->total; i > 0; --i) {
        if (strcmp(ambito->nombres[i - 1], nombre) == 0) {
            return i - 1;
        }
    }

    return -1;
}

int declarar(Ambito *ambito, const char *nombre) {
    if (ambito == NULL || nombre == NULL || nombre[0] == '\0') {
        return -1;
    }

    if (ambito->total < 0 || ambito->total > AMBITO_MAX) {
        return -1;
    }

    if (indice_local(ambito, nombre) >= 0) {
        return 0;
    }

    if (ambito->total == AMBITO_MAX) {
        return -1;
    }

    ambito->nombres[ambito->total] = nombre;
    ++ambito->total;

    return 1;
}

const char *buscar(const Ambito *ambito, const char *nombre) {
    if (ambito == NULL || nombre == NULL || nombre[0] == '\0') {
        return NULL;
    }

    if (ambito->total < 0 || ambito->total > AMBITO_MAX) {
        return NULL;
    }

    if (indice_local(ambito, nombre) >= 0) {
        return ambito->nombre;
    }

    return buscar(ambito->padre, nombre);
}
```

Sea `n` el número máximo de declaraciones revisadas por ámbito, `h` la cantidad de ámbitos recorridos y `L` la longitud máxima de los nombres comparados.

- `declarar` utiliza tiempo `O(n · L)` y espacio auxiliar `O(1)` en ambas versiones.
- `buscar` utiliza tiempo `O(h · n · L)` en ambas versiones.
- La búsqueda iterativa de Artesano utiliza espacio auxiliar `O(1)`.
- La búsqueda recursiva de IA utiliza espacio auxiliar `O(h)` debido a las llamadas pendientes.

Como `AMBITO_MAX` es ocho, el factor `n` está acotado por el contrato. La memoria ocupada por los ámbitos y las cadenas pertenece al código que los proporciona.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Búsqueda jerárquica de identificadores.

Ambas implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Las dos superaron el harness original y terminaron con código de salida `0`.

También se ejecutaron pruebas complementarias en ambas versiones para comprobar:

- Declaración con ámbito NULL, nombre NULL y nombre vacío.
- Búsqueda con ámbito NULL, nombre NULL y nombre vacío.
- Inserción de los ocho nombres permitidos.
- Rechazo de un noveno nombre.
- Detección de un duplicado cuando el ámbito ya está lleno.
- Búsqueda del último nombre almacenado.
- Ocultamiento en un hijo y conservación de la declaración del padre.
- Conservación del contador después de entradas rechazadas.

Todas las comprobaciones complementarias terminaron correctamente.

Queda pendiente repetir la compilación con GCC en el entorno del laboratorio y probar cadenas de ámbitos de gran profundidad. La versión recursiva consume pila proporcionalmente a esa profundidad.

```sh
gcc -std=c11 -Wall -Wextra -Werror harness_sesion13_c.c artesano_sesion13_c.c -o test_artesano && ./test_artesano

gcc -std=c11 -Wall -Wextra -Werror harness_sesion13_c.c ia_sesion13_c.c -o test_ia && ./test_ia
```

### Salida del harness

Las dos versiones produjeron las siguientes comprobaciones. El harness imprime su mensaje de cierre después del total mostrado.

```text
=== AUDITORIA SESION 13 - Pila de ambitos y busqueda jerarquica ===
Caso                               esperado:              obtenido:              resultado
[ 1] declarar(&g, "x")                  esperado: 1            obtenido: 1            OK
[ 2] buscar en el mismo ambito          esperado: global       obtenido: global       OK
[ 3] buscar subiendo al padre           esperado: global       obtenido: global       OK
[ 4] shadowing en el hijo               esperado: bloque       obtenido: bloque       OK
[ 5] tres niveles hasta la raiz         esperado: raiz         obtenido: raiz         OK
[ 6] nombre no declarado                esperado: (nulo)       obtenido: (nulo)       OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites de capacidad superados sin *Crash* en las pruebas ejecutadas.
* [x] Los índices respetan la capacidad del arreglo en ámbitos válidos. Las implementaciones carecen de reservas dinámicas que requieran liberación.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> El harness original deja sin evaluar la declaración de un nombre repetido dentro de un ámbito lleno. Si se comprueba la capacidad antes de buscar duplicados, esa operación devuelve `-1`, aunque el nombre ya existe y corresponde devolver `0`. Para cubrir el caso, ambas implementaciones revisan primero los duplicados y después la capacidad. La prueba complementaria llenó las ocho posiciones, confirmó que repetir `"a"` devuelve `0` y comprobó que insertar `"extra"` devuelve `-1`, conservando `total` en ocho.
>
> La versión de IA entregada ya incluye este orden de validación. Además, su búsqueda recursiva requiere considerar la profundidad de los ámbitos, porque cada nivel puede agregar una llamada a la pila.

---

**Firma digital:** `j3sus55`
