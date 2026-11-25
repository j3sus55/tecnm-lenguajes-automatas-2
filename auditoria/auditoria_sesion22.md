# Bitácora de Requerimientos y Auditoría IA — Sesión 22

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Generación de TAC para Expresiones  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Traductor de nodos AST a TAC  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en conservar los resultados de ambos hijos antes de emitir la instrucción del operador. Para `a + b * c`, primero debe generarse `_t0 = b * c` y después `_t1 = a + _t0`. El recorrido en postorden conserva la agrupación representada en el árbol.
>
> También hay que evitar que dos resultados apunten a un mismo buffer temporal reutilizado. El Pool proporcionado conserva cada nombre generado en una posición diferente. Antes de emitir se comprueban la capacidad de líneas y las 64 posiciones disponibles en ese Pool.

Implementación de referencia asistida por IA para `artesano_sesion22_c.c`. Utiliza recursión para traducir primero el hijo izquierdo, después el derecho y finalmente el operador.

El contrato deja sin especificar el diagnóstico de entradas inválidas o capacidad insuficiente. Ambas versiones adoptan `-1` para esos casos y pueden conservar un prefijo de instrucciones ya emitidas. El llamador debe descartar esa salida si recibe `-1`.

Se utiliza el Pool original, con capacidad para 64 operadores por traducción. Cada llamada reinicia los temporales desde `_t0`. Se reciben árboles binarios válidos con hojas de tipo `'H'`, operadores de tipo `'B'` y almacenamiento de salida independiente del AST. Los nombres de las hojas deben terminar en nulo dentro de `valor[16]`.

```c
#include "api_sesion22.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    char nombre[64][16];
    int usados;
} Pool;

const char *nuevo_temporal(Pool *p, int *contador) {
    if (p->usados >= 64) return "_t?";

    snprintf(p->nombre[p->usados], sizeof p->nombre[0],
             "_t%d", (*contador)++);

    return p->nombre[p->usados++];
}

static const char *traducir(const Nodo *n, char lineas[][64], int max,
                            Pool *pool, int *contador, int *usadas,
                            int profundidad) {
    if (n == NULL || profundidad > 64) return NULL;

    if (n->tipo == 'H') {
        if (memchr(n->valor, '\0', sizeof n->valor) == NULL) {
            return NULL;
        }

        return n->valor;
    }

    if (n->tipo != 'B' || n->op == '\0') return NULL;

    const char *izq = traducir(n->izq, lineas, max, pool, contador,
                               usadas, profundidad + 1);

    if (izq == NULL) return NULL;

    const char *der = traducir(n->der, lineas, max, pool, contador,
                               usadas, profundidad + 1);

    if (der == NULL || *usadas >= max || pool->usados >= 64) {
        return NULL;
    }

    const char *temporal = nuevo_temporal(pool, contador);

    snprintf(lineas[*usadas], 64, "%s = %s %c %s",
             temporal, izq, n->op, der);

    ++*usadas;

    return temporal;
}

int generar_tac(const Nodo *raiz, char lineas[][64], int max) {
    if (raiz == NULL || lineas == NULL || max < 0) return -1;

    Pool pool = {{{0}}, 0};
    int contador = 0;
    int usadas = 0;

    if (traducir(raiz, lineas, max, &pool, &contador,
                 &usadas, 0) == NULL) {
        return -1;
    }

    return usadas;
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Conservación de precedencia de operadores mediante postorden. Implementar el recorrido con una pila explícita y emitir una línea por operador utilizando `nuevo_temporal`. |
| **Output (O)** | Solo código modular y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion22.h.
El harness utiliza -std=c11 -Wall -Wextra -Werror.
El AST contiene hojas H y operadores binarios B.
El arreglo de salida tiene max filas de 64 bytes y es independiente
del almacenamiento del AST.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva la firma:
int generar_tac(const Nodo *raiz, char lineas[][64], int max);

Incluye sin modificaciones el Pool y nuevo_temporal del laboratorio.
Usa una pila explícita para simular el postorden.
Cada marco debe conservar el nodo, su etapa de procesamiento
y el resultado del hijo izquierdo.

Procesa primero el hijo izquierdo, después el derecho y finalmente
emite "_tX = izquierdo operador derecho".
Solicita cada temporal mediante nuevo_temporal.
Las hojas devuelven su valor y generan cero líneas.
Devuelve el número de instrucciones emitidas.

Respeta max y termina cada línea dentro de sus 64 bytes.
Reinicia los temporales en cada llamada.
Comprueba el límite de 64 temporales antes de solicitar uno.
Una pila de 65 marcos cubre los árboles admitidos por ese Pool.

Como decisiones defensivas, devuelve -1 para entradas inválidas,
hijos ausentes, hojas sin terminador nulo o capacidad insuficiente.
Puede quedar un prefijo de líneas emitidas cuando ocurre un error.
Acepta una hoja con max cero porque no necesita instrucciones.

Output:
Entrega ia_sesion22_c.c sin main() y explica su complejidad
temporal y espacial.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion22.h"
#include <stdio.h>
#include <string.h>

typedef struct {
    char nombre[64][16];
    int usados;
} Pool;

const char *nuevo_temporal(Pool *p, int *contador) {
    if (p->usados >= 64) return "_t?";

    snprintf(p->nombre[p->usados], sizeof p->nombre[0],
             "_t%d", (*contador)++);

    return p->nombre[p->usados++];
}

typedef struct {
    const Nodo *nodo;
    int etapa;
    const char *izquierdo;
} Marco;

int generar_tac(const Nodo *raiz, char lineas[][64], int max) {
    if (raiz == NULL || lineas == NULL || max < 0) return -1;

    Marco pila[65];
    int cima = 0;

    pila[0] = (Marco){raiz, 0, NULL};

    Pool pool = {{{0}}, 0};
    int contador = 0;
    int usadas = 0;
    const char *resultado = NULL;

    while (cima >= 0) {
        Marco *m = &pila[cima];
        const Nodo *n = m->nodo;

        if (n == NULL) return -1;

        if (n->tipo == 'H') {
            if (memchr(n->valor, '\0', sizeof n->valor) == NULL) {
                return -1;
            }

            resultado = n->valor;
            --cima;
        } else if (n->tipo != 'B' || n->op == '\0') {
            return -1;
        } else if (m->etapa < 2) {
            const Nodo *hijo;

            if (m->etapa == 0) {
                hijo = n->izq;
                m->etapa = 1;
            } else {
                m->izquierdo = resultado;
                hijo = n->der;
                m->etapa = 2;
            }

            if (cima == 64) return -1;

            ++cima;
            pila[cima] = (Marco){hijo, 0, NULL};
        } else {
            if (usadas >= max || pool.usados >= 64) return -1;

            const char *temporal = nuevo_temporal(&pool, &contador);

            snprintf(lineas[usadas], 64, "%s = %s %c %s",
                     temporal, m->izquierdo, n->op, resultado);

            ++usadas;
            resultado = temporal;
            --cima;
        }
    }

    return usadas;
}
```

Sea `n` la cantidad de nodos y `h` la altura del árbol.

Ambas implementaciones tienen tiempo `O(n)` para los árboles admitidos. Cada nodo se procesa un número constante de veces, y las hojas y temporales tienen longitudes acotadas.

Artesano utiliza espacio de llamadas `O(h)` más el Pool fijo. La versión de IA reserva una pila fija de 65 marcos y el Pool de 64 temporales, por lo que su espacio auxiliar reservado es `O(1)` con estos límites. Si las capacidades se generalizaran, el almacenamiento del recorrido dependería de la altura y el Pool de la cantidad de operadores.

Las hojas no emiten instrucciones y cada operador válido emite exactamente una. Los punteros a temporales se utilizan mientras el Pool local permanece vigente.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Expresiones anidadas con paréntesis

Ambas implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Las dos superaron los seis casos del harness original y terminaron con código de salida cero.

También se ejecutaron pruebas complementarias de cantidad de líneas devueltas, capacidad exacta, capacidad insuficiente, max cero, raíz NULL, salida NULL, max negativo y operador con un hijo ausente. Un centinela confirmó que se conservó una fila situada fuera de la capacidad permitida.

Además, se tradujo una cadena con 64 operadores y se comprobó su última instrucción `_t63 = _t62 + b`. Un árbol con 65 operadores devolvió `-1`. Una llamada posterior confirmó que la numeración vuelve a `_t0`. Todas estas pruebas terminaron correctamente.

Queda pendiente repetir la compilación y ejecución con GCC y ampliar las pruebas de árboles inválidos y valores de hoja próximos al límite de su almacenamiento.

### Salida del harness

Salida obtenida en ambas implementaciones, presentada hasta el total. El programa imprime después su mensaje de cierre.

```text
=== AUDITORIA SESION 22 - Traductor de AST a TAC con precedencia ===
Caso                               esperado:              obtenido:              resultado
[ 1] a + b * c                          esperado: _t0 = b * c  obtenido: _t0 = b * c  OK
[ 2] a + b * c (2a linea)
     esperado: _t1 = a + _t0
     obtenido: _t1 = a + _t0
     -> OK
[ 3] (a + b) * c
     esperado: _t1 = _t0 * c
     obtenido: _t1 = _t0 * c
     -> OK
[ 4] x * (y - z) + w
     esperado: _t2 = _t1 + w
     obtenido: _t2 = _t1 + w
     -> OK
[ 5] hoja sin operaciones               esperado: 0            obtenido: 0            OK
[ 6] (a + b) * (c - d)
     esperado: _t2 = _t0 * _t1
     obtenido: _t2 = _t0 * _t1
     -> OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites superados sin *Crash* en las pruebas ejecutadas.
* [x] La revisión confirma controles de capacidad para líneas, pila y Pool, además de escritura acotada mediante snprintf. Ambas versiones carecen de reservas dinámicas de memoria.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> El harness original utiliza árboles pequeños y buffers suficientes, por lo que deja sin evaluar el agotamiento del Pool. La función proporcionada devuelve `"_t?"` después de 64 temporales, lo que permitiría generar instrucciones con nombres repetidos si se continuara traduciendo. Ambas implementaciones comprueban la capacidad antes de solicitar un temporal y devuelven `-1` al excederla.
>
> Se verificó una cadena de 64 operadores, cuya última instrucción utiliza `_t63`, y después otra de 65 operadores, que fue rechazada. También se comprobó que una capacidad de una línea conserve intacta la fila siguiente cuando el árbol requiere dos instrucciones.
>
> La versión de IA entregada ya incluye estas protecciones. Las pruebas complementarias ampliaron la cobertura del harness. Cuando se devuelve `-1`, el prefijo emitido puede permanecer en el buffer y debe descartarse.

---

**Firma digital:** [mi usuario de GitHub]
