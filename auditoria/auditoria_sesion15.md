# Bitácora de Requerimientos y Auditoría IA — Sesión 15

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Bloques Anidados y Ciclo de Vida  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Operaciones Push y Pop de ámbito.

*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en conservar el enlace al padre antes de liberar el ámbito actual. Si se ejecuta `free(actual)` y después se consulta `actual->padre`, se accede a memoria liberada. También hay que distinguir la raíz de los ámbitos reservados dinámicamente, porque el harness construye la raíz como una variable local y esa memoria debe conservarse.
>
> Cada ámbito creado mediante `push_ambito` debe cerrarse una sola vez mediante `pop_ambito`, comenzando por el más interno. El nombre se conserva mediante el puntero recibido, por lo que su cadena debe permanecer válida mientras se utilice.

Implementación de referencia asistida por IA para `artesano_sesion15_c.c`. Utiliza `malloc` y asigna cada campo del registro por separado.

```c
#include "api_sesion15.h"
#include <stdlib.h>

Ambito *push_ambito(Ambito *actual, const char *nombre) {
    if (actual == NULL || nombre == NULL) {
        return NULL;
    }

    Ambito *nuevo = malloc(sizeof *nuevo);

    if (nuevo == NULL) {
        return NULL;
    }

    nuevo->nombre = nombre;
    nuevo->padre = actual;

    return nuevo;
}

Ambito *pop_ambito(Ambito *actual) {
    if (actual == NULL || actual->padre == NULL) {
        return NULL;
    }

    Ambito *padre = actual->padre;

    free(actual);

    return padre;
}
```

Después de cerrar un bloque, el llamador debe actualizar su puntero activo con el padre devuelto. Ante un fallo de `push_ambito`, debe conservar el puntero anterior para poder cerrar los ámbitos existentes.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Reservar un ámbito enlazado al padre y liberar el ámbito actual al cerrar un bloque. Conservar la raíz y devolver el padre antes de perder su referencia. |
| **Output (O)** | Código modular para `ia_sesion15_c.c` y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion15.h.
El harness compila con -std=c11 -Wall -Wextra -Werror.
La raíz tiene padre NULL y pertenece al llamador.
Los ámbitos descendientes se crean mediante push_ambito.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva estas firmas:
Ambito *push_ambito(Ambito *actual, const char *nombre);
Ambito *pop_ambito(Ambito *actual);

En push_ambito:
Devuelve NULL si actual o nombre son NULL.
Reserva un registro mediante calloc.
Comprueba si la reserva tuvo éxito.
Inicializa el registro mediante un literal compuesto de Ambito,
conservando el puntero al nombre y enlazando el padre.
Devuelve el nuevo ámbito.
Acepta el nombre vacío porque el contrato únicamente rechaza NULL.

En pop_ambito:
Devuelve NULL si actual es NULL o representa la raíz.
Conserva la raíz.
Para un ámbito descendiente, guarda primero el padre,
libera el registro actual y devuelve el padre guardado.
La cadena del nombre pertenece al llamador.

Output:
Entrega ia_sesion15_c.c sin main() y explica la complejidad
temporal y espacial de ambas operaciones.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion15.h"
#include <stdlib.h>

Ambito *push_ambito(Ambito *actual, const char *nombre) {
    if (actual == NULL || nombre == NULL) {
        return NULL;
    }

    Ambito *nuevo = calloc(1, sizeof *nuevo);

    if (nuevo != NULL) {
        *nuevo = (Ambito){nombre, actual};
    }

    return nuevo;
}

Ambito *pop_ambito(Ambito *actual) {
    Ambito *padre = NULL;

    if (actual != NULL) {
        padre = actual->padre;

        if (padre != NULL) {
            free(actual);
        }
    }

    return padre;
}
```

Ambas implementaciones realizan un número constante de operaciones por llamada. Bajo el modelo habitual de análisis del laboratorio, `push_ambito` y `pop_ambito` tienen tiempo `O(1)`, considerando la reserva y liberación de un registro de tamaño fijo.

Cada llamada exitosa a `push_ambito` agrega espacio `O(1)`. Una cadena con `h` ámbitos dinámicos ocupa `O(h)`. `pop_ambito` utiliza espacio auxiliar `O(1)` y libera un registro.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Aislamiento de variables fuera de bloque.

El harness comprueba la estructura de la cadena mediante nombres, profundidad y retorno al padre. La interfaz de esta sesión contiene los campos `nombre` y `padre`, por lo que la visibilidad se representa mediante el cambio del ámbito activo al cerrar un bloque.

Las dos implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Ambas superaron el harness original y terminaron con código de salida `0`.

También se ejecutaron las siguientes comprobaciones complementarias en ambas versiones:

- `pop_ambito(NULL)` devuelve NULL.
- `push_ambito` rechaza un nombre NULL.
- `push_ambito(NULL, NULL)` devuelve NULL.
- El cierre de la raíz conserva su nombre y su padre.
- Un nombre vacío permite crear y cerrar un ámbito.
- Cien ciclos de apertura y cierre de dos bloques anidados conservan los enlaces y devuelven el padre esperado.

Todas estas comprobaciones terminaron correctamente. Queda pendiente repetir las pruebas con GCC, simular fallos de asignación de memoria y ejecutar un detector de fugas.

```sh
gcc -std=c11 -Wall -Wextra -Werror harness_sesion15_c.c artesano_sesion15_c.c -o test_artesano && ./test_artesano

gcc -std=c11 -Wall -Wextra -Werror harness_sesion15_c.c ia_sesion15_c.c -o test_ia && ./test_ia
```

### Salida del harness

Ambas versiones produjeron las siguientes comprobaciones. El harness imprime su mensaje de cierre después del total mostrado.

```text
=== AUDITORIA SESION 15 - Push y pop de ambitos ===
Caso                               esperado:              obtenido:              resultado
[ 1] nombre del ambito nuevo            esperado: if           obtenido: if           OK
[ 2] profundidad con dos push           esperado: 3            obtenido: 3            OK
[ 3] pop devuelve el padre              esperado: 1            obtenido: 1            OK
[ 4] pop sobre la raiz                  esperado: 1            obtenido: 1            OK
[ 5] push con padre NULL                esperado: 1            obtenido: 1            OK
[ 6] profundidad tras pop               esperado: 2            obtenido: 2            OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites de entrada comprobados sin *Crash* en las pruebas ejecutadas.
* [ ] No existen accesos fuera de límites ni *Memory Leaks*. La revisión confirma que el padre se obtiene antes de liberar y que cada reserva exitosa de las pruebas tiene su cierre. Falta confirmación mediante un detector de memoria.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> La auditoría identificó que el harness deja sin comprobar `pop_ambito(NULL)` y `push_ambito` con un nombre NULL. Se agregaron pruebas complementarias para confirmar que ambas implementaciones devuelven NULL sin acceder a memoria inválida.
>
> Otro punto importante es la propiedad del nombre. En el primer caso del harness, el puntero al nombre se consulta después de liberar el ámbito. Conservar el puntero recibido permite utilizar el literal `"if"` porque su almacenamiento continúa vigente. Las implementaciones liberan únicamente el registro dinámico y mantienen la cadena bajo responsabilidad del llamador.
>
> La versión de IA entregada ya contempla estas condiciones. El fallo de reserva está comprobado en el código mediante la validación del resultado de calloc, aunque su simulación queda pendiente.

---

**Firma digital:** [mi usuario de GitHub]
