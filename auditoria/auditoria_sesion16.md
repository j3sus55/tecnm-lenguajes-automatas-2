# Bitácora de Requerimientos y Auditoría IA — Sesión 16

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Firmas de Función y Parámetros  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Registro de prototipos y aridad.

*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en distinguir tres resultados al revisar una llamada. Primero se debe localizar el nombre completo de la función. Si existe, se compara su aridad con la cantidad de argumentos recibidos. Una coincidencia devuelve `1`, una cantidad incorrecta devuelve `0` y un nombre ausente devuelve `-1`.
>
> También hay que separar el número de funciones del catálogo de la cantidad de parámetros de cada función. Por ejemplo, un catálogo con una sola entrada puede registrar una función que requiere tres argumentos. Para evitar accesos fuera del arreglo, el recorrido debe respetar el valor de `total`, que debe corresponder a la cantidad de registros disponibles.

Implementación de referencia asistida por IA para `artesano_sesion16_c.c`. Utiliza índices para localizar la firma y después compara la aridad.

```c
#include "api_sesion16.h"
#include <string.h>

int verificar_llamada(const Funcion *catalogo, int total,
                      const char *nombre, int nargs) {
    if (catalogo == NULL || nombre == NULL || total < 0 || nargs < 0) {
        return -1;
    }

    int encontrada = -1;

    for (int i = 0; i < total; ++i) {
        if (catalogo[i].nombre != NULL &&
            strcmp(catalogo[i].nombre, nombre) == 0) {
            encontrada = i;
            break;
        }
    }

    if (encontrada == -1 || catalogo[encontrada].aridad < 0) {
        return -1;
    }

    if (catalogo[encontrada].aridad == nargs) {
        return 1;
    }

    return 0;
}
```

Como medidas defensivas adicionales, se rechaza un `total` negativo, se omiten registros con nombre NULL y se devuelve `-1` si la firma encontrada tiene aridad negativa.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Comprobar la coincidencia estricta entre la aridad registrada y los argumentos recibidos. Buscar nombres completos mediante un recorrido con punteros. |
| **Output (O)** | Código modular para `ia_sesion16_c.c` y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion16.h.
El catálogo contiene total registros accesibles.
El harness utiliza -std=c11 -Wall -Wextra -Werror.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva la firma:
int verificar_llamada(const Funcion *catalogo, int total,
                      const char *nombre, int nargs);

Recorre el catálogo mediante un puntero a Funcion y un contador
de registros pendientes.

Compara nombres completos utilizando strcmp.
Al encontrar la función, devuelve 1 si su aridad coincide con nargs
y 0 si la cantidad de argumentos es diferente.
Si se termina el catálogo sin encontrar el nombre, devuelve -1.

Una función con aridad cero acepta únicamente cero argumentos.
Devuelve -1 para catálogo NULL, nombre NULL, total negativo
o cantidad de argumentos negativa.

Como validaciones defensivas, omite entradas con nombre NULL
y rechaza una firma encontrada cuya aridad sea negativa.
Conserva el catálogo durante la consulta.

Output:
Entrega ia_sesion16_c.c sin main() y explica su complejidad
temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion16.h"
#include <string.h>

int verificar_llamada(const Funcion *catalogo, int total,
                      const char *nombre, int nargs) {
    if (catalogo == NULL || nombre == NULL || total < 0 || nargs < 0) {
        return -1;
    }

    const Funcion *firma = catalogo;
    int pendientes = total;

    while (pendientes > 0) {
        if (firma->nombre != NULL && strcmp(firma->nombre, nombre) == 0) {
            if (firma->aridad < 0) {
                return -1;
            }

            return firma->aridad == nargs;
        }

        ++firma;
        --pendientes;
    }

    return -1;
}
```

Sea `n` el número de registros del catálogo y `L` la longitud máxima de los nombres comparados.

Ambas implementaciones tienen tiempo `O(n · L)` en el peor caso y espacio auxiliar `O(1)`. Si la función aparece al principio del catálogo, la búsqueda termina con la primera coincidencia. Si está ausente, se revisan las `n` entradas.

La versión Artesano guarda el índice encontrado antes de verificar la aridad. La versión de IA recorre los registros mediante un puntero y devuelve el diagnóstico al encontrar la firma.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Llamadas con número incorrecto de args.

Las dos implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Ambas superaron los seis casos del harness original y terminaron con código de salida `0`.

También se ejecutaron comprobaciones complementarias en ambas versiones:

- Búsqueda de funciones en la segunda y última posición de un catálogo.
- Rechazo de un argumento adicional para una función de aridad cero.
- Comparación exacta de nombres, incluyendo prefijos y diferencias entre mayúsculas y minúsculas.
- Catálogo con cero registros.
- Cantidad de registros negativa.
- Catálogo NULL y nombre NULL.
- Cantidad de argumentos negativa.
- Entrada con nombre NULL.
- Firma registrada con aridad negativa.
- Coincidencia de aridad con el valor `INT_MAX`.
- Nombre de consulta con el mismo contenido almacenado en otra dirección de memoria.

Todas estas comprobaciones terminaron correctamente. Queda pendiente repetir la compilación y ejecución con GCC en el entorno del laboratorio y realizar una comprobación instrumental de memoria.

```sh
gcc -std=c11 -Wall -Wextra -Werror harness_sesion16_c.c artesano_sesion16_c.c -o test_artesano && ./test_artesano

gcc -std=c11 -Wall -Wextra -Werror harness_sesion16_c.c ia_sesion16_c.c -o test_ia && ./test_ia
```

### Salida del harness

Ambas versiones produjeron las siguientes comprobaciones. El harness imprime su mensaje de cierre después del total mostrado.

```text
=== AUDITORIA SESION 16 - Registro de prototipos y aridad ===
Caso                               esperado:              obtenido:              resultado
[ 1] suma con 3 argumentos              esperado: 1            obtenido: 1            OK
[ 2] suma con 2 argumentos              esperado: 0            obtenido: 0            OK
[ 3] suma con 4 argumentos              esperado: 0            obtenido: 0            OK
[ 4] funcion no registrada              esperado: -1           obtenido: -1           OK
[ 5] main sin argumentos                esperado: 1            obtenido: 1            OK
[ 6] nombre NULL                        esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites de entrada superados sin *Crash* en las pruebas ejecutadas.
* [x] La revisión confirma recorridos acotados por `total`, siempre que el llamador proporcione esa cantidad de registros accesibles. Ambas versiones carecen de reservas dinámicas de memoria.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> El harness original utiliza catálogos de una sola función, por lo que deja sin comprobar que la búsqueda alcance registros posteriores. Una implementación que consultara únicamente la primera entrada podría superar los seis casos y fallar con un catálogo mayor. Para detectar ese problema se agregó un catálogo con tres funciones y se comprobó la búsqueda en la segunda y última posición.
>
> También se verificó que una función con aridad cero rechace una llamada con un argumento. La versión de IA entregada ya contempla ambos comportamientos. Las pruebas complementarias confirmaron el recorrido completo cuando hace falta y la comparación estricta de la aridad.

---

**Firma digital:** [mi usuario de GitHub]
