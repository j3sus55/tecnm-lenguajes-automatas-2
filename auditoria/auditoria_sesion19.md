# Bitácora de Requerimientos y Auditoría IA — Sesión 19

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Estructuras (struct) y Miembros  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Offsets de miembros en estructuras.

*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en incluir tanto el relleno entre miembros como el relleno final de la estructura. Para los campos char, int y short, los desplazamientos son 0, 4 y 8. El último miembro termina en el byte 10, pero el tamaño total debe redondearse a 12 para conservar la alineación de cuatro bytes.
>
> También hay que validar los tipos antes de calcular el resultado y prevenir desbordamientos al avanzar el cursor. La búsqueda de un miembro debe comparar su nombre completo y devolver `-1` cuando esté ausente.

Implementación de referencia asistida por IA para `artesano_sesion19_c.c`. Alinea mediante incrementos y valida la disposición completa antes de buscar el miembro.

Se adoptan los anchos del modelo usado en los laboratorios, char=1, short=2, int=4, float=4, double=8 y puntero=8. El contrato de esta sesión exige que cada campo se alinee a su ancho.

Como decisiones defensivas adicionales, se devuelve `-1` para conteos negativos, nombres de campo NULL y resultados que excedan `INT_MAX`. El llamador debe proporcionar al menos `total` registros accesibles.

```c
#include "api_sesion19.h"
#include <limits.h>
#include <string.h>

static int ancho_tipo(const char *tipo) {
    if (tipo == NULL) return -1;
    if (strcmp(tipo, "char") == 0) return 1;
    if (strcmp(tipo, "short") == 0) return 2;

    if (strcmp(tipo, "int") == 0 || strcmp(tipo, "float") == 0) {
        return 4;
    }

    if (strcmp(tipo, "double") == 0 || strcmp(tipo, "puntero") == 0) {
        return 8;
    }

    return -1;
}

static int alinear(int cursor, int ancho) {
    while (cursor % ancho != 0) {
        if (cursor == INT_MAX) return -1;
        ++cursor;
    }

    return cursor;
}

int tamano_struct(const Campo *campos, int total) {
    if (campos == NULL || total < 0) return -1;

    int cursor = 0;
    int mayor = 1;

    for (int i = 0; i < total; ++i) {
        int ancho = ancho_tipo(campos[i].tipo);

        if (ancho < 0 || campos[i].nombre == NULL) return -1;

        cursor = alinear(cursor, ancho);

        if (cursor < 0 || cursor > INT_MAX - ancho) return -1;

        cursor += ancho;

        if (ancho > mayor) mayor = ancho;
    }

    return alinear(cursor, mayor);
}

int offset_campo(const Campo *campos, int total, const char *nombre) {
    if (nombre == NULL || tamano_struct(campos, total) < 0) {
        return -1;
    }

    int cursor = 0;

    for (int i = 0; i < total; ++i) {
        int ancho = ancho_tipo(campos[i].tipo);
        cursor = alinear(cursor, ancho);

        if (strcmp(campos[i].nombre, nombre) == 0) {
            return cursor;
        }

        cursor += ancho;
    }

    return -1;
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Calcular offsets alineando cada miembro a su ancho y redondear el tamaño final a la mayor alineación. Usar una tabla de tipos y aritmética modular. |
| **Output (O)** | Código modular para `ia_sesion19_c.c` y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion19.h.
El llamador proporciona total registros accesibles.
El harness utiliza -std=c11 -Wall -Wextra -Werror.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva las firmas:
int offset_campo(const Campo *campos, int total, const char *nombre);
int tamano_struct(const Campo *campos, int total);

Usa una tabla constante de anchos:
char=1, short=2, int=4, float=4, double=8 y puntero=8.

Implementa un recorrido compartido que:
1. Valide todos los campos.
2. Calcule el relleno mediante aritmética modular.
3. Alinee cada miembro antes de asignarle su offset.
4. Registre el offset del nombre solicitado.
5. Sume el ancho y actualice la mayor alineación.
6. Calcule el padding final.

Devuelve -1 para listas NULL, tipos desconocidos, tipos NULL,
conteos negativos y nombres de campo NULL.
offset_campo también devuelve -1 si el nombre solicitado es NULL
o está ausente.
Valida la lista completa incluso si el campo solicitado aparece
antes que un campo con tipo inválido.

Una lista válida con total cero tiene tamaño cero.
Comprueba cada suma para evitar superar INT_MAX y devuelve -1
si el resultado no puede representarse.

Output:
Entrega ia_sesion19_c.c sin main() y explica su complejidad.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion19.h"
#include <limits.h>
#include <string.h>

static int ancho_tipo(const char *tipo) {
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

    if (tipo == NULL) return -1;

    for (unsigned int i = 0; i < sizeof tipos / sizeof tipos[0]; ++i) {
        if (strcmp(tipo, tipos[i].nombre) == 0) {
            return tipos[i].ancho;
        }
    }

    return -1;
}

static int calcular(const Campo *campos, int total, const char *nombre) {
    if (campos == NULL || total < 0) return -1;

    int cursor = 0;
    int mayor = 1;
    int encontrado = -1;

    for (int i = 0; i < total; ++i) {
        int ancho = ancho_tipo(campos[i].tipo);

        if (ancho < 0 || campos[i].nombre == NULL) return -1;

        int relleno = (ancho - cursor % ancho) % ancho;

        if (cursor > INT_MAX - relleno) return -1;

        cursor += relleno;

        if (nombre != NULL && encontrado < 0 &&
            strcmp(campos[i].nombre, nombre) == 0) {
            encontrado = cursor;
        }

        if (cursor > INT_MAX - ancho) return -1;

        cursor += ancho;

        if (ancho > mayor) mayor = ancho;
    }

    int final = (mayor - cursor % mayor) % mayor;

    if (cursor > INT_MAX - final) return -1;

    if (nombre != NULL) return encontrado;

    return cursor + final;
}

int offset_campo(const Campo *campos, int total, const char *nombre) {
    if (nombre == NULL) return -1;

    return calcular(campos, total, nombre);
}

int tamano_struct(const Campo *campos, int total) {
    return calcular(campos, total, NULL);
}
```

Sea `n` la cantidad de campos y `L` la longitud máxima de los nombres comparados.

En ambas versiones, `tamano_struct` tiene tiempo `O(n)` para el conjunto fijo de tipos. `offset_campo` tiene tiempo `O(n · L)`, considerando las comparaciones de nombres. Todas las funciones utilizan espacio auxiliar `O(1)`.

En Artesano, cada alineación realiza como máximo siete incrementos. Su búsqueda ejecuta una validación previa y un recorrido adicional, conservando la complejidad lineal. La versión de IA combina la validación y el cálculo del offset en un recorrido.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Acceso a miembros inexistentes.

Las dos implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Ambas superaron los seis casos del harness original y terminaron con código de salida `0`.

También se ejecutaron comprobaciones complementarias en ambas versiones:

- Offsets y tamaño de una estructura char, double y char, comparados con `offsetof` y `sizeof` de una estructura C real.
- Disposición de campos short, float y puntero.
- Tipo desconocido situado después del miembro solicitado.
- Tipo NULL y nombre de campo NULL.
- Nombre solicitado NULL.
- Lista NULL, incluso con total cero.
- Conteo negativo.
- Búsqueda en una lista con cero campos.
- Tamaño cero para una lista válida sin campos.
- Comparación exacta de nombres con diferencias de mayúsculas.

Todas las comprobaciones terminaron correctamente. Queda pendiente repetir las pruebas con GCC y ejecutar casos de tamaño extremo para comprobar las protecciones contra desbordamiento.

```sh
gcc -std=c11 -Wall -Wextra -Werror harness_sesion19_c.c artesano_sesion19_c.c -o test_artesano && ./test_artesano

gcc -std=c11 -Wall -Wextra -Werror harness_sesion19_c.c ia_sesion19_c.c -o test_ia && ./test_ia
```

### Salida del harness

Ambas implementaciones produjeron las siguientes comprobaciones. El harness imprime su mensaje de cierre después del total mostrado.

```text
=== AUDITORIA SESION 19 - Offsets de miembros y tamano de struct ===
Caso                               esperado:              obtenido:              resultado
[ 1] struct char e int: b               esperado: 4            obtenido: 4            OK
[ 2] struct de tres campos: c           esperado: 8            obtenido: 8            OK
[ 3] tamano total con padding           esperado: 12           obtenido: 12           OK
[ 4] double seguido de char             esperado: 16           obtenido: 16           OK
[ 5] miembro inexistente                esperado: -1           obtenido: -1           OK
[ 6] struct sin campos                  esperado: 0            obtenido: 0            OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites de entrada superados sin *Crash* en las pruebas ejecutadas.
* [x] Los recorridos respetan total cuando el llamador proporciona esa cantidad de campos accesibles. Ambas implementaciones carecen de reservas dinámicas de memoria.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> El harness original deja sin comprobar un tipo desconocido situado después del miembro buscado. Una búsqueda que devuelva inmediatamente el primer offset puede aceptar una lista cuya disposición completa es inválida. Para cubrir esa condición, Artesano valida la estructura antes de buscar y la versión de IA conserva el offset encontrado mientras termina de revisar todos los campos.
>
> La prueba complementaria utilizó un primer campo válido y un segundo campo de tipo desconocido. Ambas implementaciones devolvieron `-1` al consultar el primero. También se verificaron el padding interno y final de una estructura char, double y char mediante comparación con `offsetof` y `sizeof`.
>
> La versión de IA entregada ya contempla estas condiciones. La auditoría amplió la cobertura de los seis casos originales.

---

**Firma digital:** [mi usuario de GitHub]
