# Bitácora de Requerimientos y Auditoría IA — Sesión 07
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Evaluación de Expresiones Aritméticas  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Evaluador recursivo de operaciones  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> La dificultad técnica consiste en distinguir un resultado válido de cero de un cero devuelto por error. Cada llamada recursiva debe revisar el estado después de evaluar un hijo. Si el hijo izquierdo falla, se omiten el hijo derecho y la operación del padre. En una división o un módulo, se comprueba el divisor antes de aplicar el operador.
>
> El archivo `artesano_sesion07_c.c` es una implementación de referencia asistida por IA. Comparte un estado de error entre las llamadas recursivas y usa un estado local en la función pública. Se supone un árbol finito y acíclico y un `int` de 32 bits, como en el entorno de prueba. Para casos que el laboratorio deja sin definir, ambas versiones devuelven `0` cuando hay error, usan el código `2` para un operador desconocido o un resultado fuera del rango de `int`, y permiten `error == NULL` para consultar solo el valor.

```c
#include "api_sesion07.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

static int evaluar_nodo(const ASTNode *node, int *error) {
    if (node == NULL || (node->tipo != NODO_NUM && node->tipo != NODO_BINOP)) {
        *error = 2;
        return 0;
    }
    if (node->tipo == NODO_NUM) return node->valor;

    int64_t left = evaluar_nodo(node->izq, error);
    if (*error != 0) return 0;
    int64_t right = evaluar_nodo(node->der, error);
    if (*error != 0) return 0;

    if ((node->op == '/' || node->op == '%') && right == 0) {
        *error = 1;
        return 0;
    }

    int64_t result;
    switch (node->op) {
        case '+': result = left + right; break;
        case '-': result = left - right; break;
        case '*': result = left * right; break;
        case '/': result = left / right; break;
        case '%': result = left % right; break;
        default:
            *error = 2;
            return 0;
    }

    if (result < INT_MIN || result > INT_MAX) {
        *error = 2;
        return 0;
    }
    return (int)result;
}

int evaluar(ASTNode *n, int *error) {
    int status = 0;
    int result = evaluar_nodo(n, &status);
    if (error != NULL) *error = status;
    return result;
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C11 con la biblioteca estándar y la cabecera `api_sesion07.h` del laboratorio. La compilación indicada usa GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores encargado del evaluador de expresiones constantes de un AST. |
| **Especificación (E)** | Conservar `int evaluar(ASTNode *n, int *error);` y usar los tipos declarados en la cabecera. Evaluar hojas `NODO_NUM` y nodos `NODO_BINOP` para `+`, `-`, `*`, `/` y `%`. Comprobar el divisor antes de dividir o calcular el residuo. Informar código `1` para divisor cero y `2` para nodo inválido, incluido `NULL`. Propagar el primer error de un hijo sin aplicar la operación del padre. |
| **Output (O)** | Código completo de `ia_sesion07_c.c` sin `main()` y análisis de complejidad temporal y espacial. El `main()` lo aporta el harness. |

### 2.2 Prompt Estructurado
```text
Contexto
Escribe en C11 con la biblioteca estándar. Incluye "api_sesion07.h".
La cabecera del laboratorio define TipoNodo con NODO_NUM y NODO_BINOP.
ASTNode contiene TipoNodo tipo, int valor, char op, struct ASTNode *izq
y struct ASTNode *der. No redefinas estos tipos ni cambies la firma
int evaluar(ASTNode *n, int *error);
El harness de GCC usa -std=c11 -Wall -Wextra -Werror.

Rol
Actúa como ingeniero de software de sistemas y compiladores. Implementa
un evaluador recursivo de expresiones aritméticas sobre un AST.

Especificación
Devuelve el valor de la hoja NODO_NUM. En NODO_BINOP evalúa primero el
hijo izquierdo y luego el derecho. Admite +, -, *, / y %. Comprueba el
divisor antes de ejecutar / o %. Si es cero, informa error 1. Para un
nodo inválido, incluido NULL, informa error 2. Si falla un hijo,
propaga su error inmediatamente y omite la operación del padre.

Usa una estructura privada con el valor y el estado de cada subárbol.
En la función pública escribe 0 en *error al terminar con éxito.
Como convenciones adicionales, devuelve 0 cuando hay error, usa el
código 2 para operadores desconocidos y resultados fuera del rango de
int, y permite error == NULL. Supón un int de 32 bits y un árbol finito
y acíclico.

Output
Entrega ia_sesion07_c.c completo, sin main() y sin bibliotecas no
estándar. Explica el tiempo y el espacio auxiliar en función del
número de nodos y de la altura del árbol.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion07.h"
#include <limits.h>
#include <stddef.h>
#include <stdint.h>

typedef struct {
    int valor;
    int error;
} Resultado;

static Resultado evaluar_interno(const ASTNode *n) {
    if (n == NULL) return (Resultado){0, 2};
    if (n->tipo == NODO_NUM) return (Resultado){n->valor, 0};
    if (n->tipo != NODO_BINOP) return (Resultado){0, 2};

    Resultado izq = evaluar_interno(n->izq);
    if (izq.error) return izq;
    Resultado der = evaluar_interno(n->der);
    if (der.error) return der;

    int64_t a = izq.valor;
    int64_t b = der.valor;
    int64_t valor;
    switch (n->op) {
        case '+': valor = a + b; break;
        case '-': valor = a - b; break;
        case '*': valor = a * b; break;
        case '/':
            if (b == 0) return (Resultado){0, 1};
            valor = a / b;
            break;
        case '%':
            if (b == 0) return (Resultado){0, 1};
            valor = a % b;
            break;
        default:
            return (Resultado){0, 2};
    }

    if (valor < INT_MIN || valor > INT_MAX) return (Resultado){0, 2};
    return (Resultado){(int)valor, 0};
}

int evaluar(ASTNode *n, int *error) {
    Resultado r = evaluar_interno(n);
    if (error != NULL) *error = r.error;
    return r.valor;
}
```

**Análisis de complejidad.** La versión Artesano propaga el error mediante un estado compartido por puntero. La versión del modelo devuelve un `Resultado` por cada llamada, de modo que el valor y el error del subárbol viajan juntos. Ambas visitan cada nodo del árbol como máximo una vez y hacen trabajo constante por nodo. El tiempo en el peor caso es `O(n)` para `n` nodos y el espacio auxiliar es `O(h)` para una altura `h`, por la pila de recursión. En un árbol completamente sesgado, `h` puede crecer hasta `n`.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Expresiones complejas y división por 0

Se compiló el harness adjunto por separado con `artesano_sesion07_c.c` y `ia_sesion07_c.c` usando Microsoft Visual C++ (MSVC) en modo C11, con `/std:c11 /utf-8 /W4 /WX`. Ambas ejecuciones del harness terminaron con código de salida `0` y produjeron la misma salida. Se comprobaron expresiones anidadas, resta asociada a la izquierda, división válida, división y residuo entre cero, y árbol nulo.

También se ejecutaron cuatro verificaciones con `assert` para cada implementación. Se comprobó la propagación de un error desde el hijo izquierdo, el resultado fuera de rango de `INT_MIN / -1`, el restablecimiento de `*error` a `0` al evaluar una hoja válida y una llamada con `error == NULL`. Las cuatro pasaron en ambas versiones. Quedó pendiente repetir la compilación con GCC, ejecutar un análisis con sanitizadores y medir el límite de profundidad de la pila.

### Salida del harness
```text
=== AUDITORIA SESION 07 - Evaluador recursivo con deteccion de errores ===
Caso                               esperado:              obtenido:              resultado
[ 1] (2+3)*4                            esperado: 20           obtenido: 20           OK
[ 2] 7/0 -> error 1                     esperado: 1            obtenido: 1            OK
[ 3] 7%0 -> error 1                     esperado: 1            obtenido: 1            OK
[ 4] arbol NULL -> error 2              esperado: 2            obtenido: 2            OK
[ 5] 10-2-3                             esperado: 5            obtenido: 5            OK
[ 6] (8/2)+1                            esperado: 5            obtenido: 5            OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion07.md
```

* [x] Caso base y límites superados sin *Crash*.
* [ ] No existen accesos fuera de límites ni *Memory Leaks*.
* [x] La complejidad teórica coincide con la implementada.

La primera verificación se respalda con la ejecución de los casos indicados. Para la segunda, la revisión confirmó que los evaluadores no reservan memoria dinámica y comprueban `NULL` antes de acceder a un nodo. La casilla queda abierta porque faltó la ejecución con sanitizadores y la prueba de profundidad máxima. La tercera se verificó por revisión del recorrido y de las llamadas recursivas.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> El harness prueba `7 / 0` como expresión completa. Para verificar qué sucede cuando el error está dentro de otra operación, construí un nodo suma cuyo hijo izquierdo era `7 / 0` y cuyo hijo derecho era `NULL`. En ambos programas se comprobó con `assert` que la evaluación devolviera `0` con error `1`. Si la implementación hubiera seguido evaluando el hijo derecho y sustituido el diagnóstico, habría devuelto error `2`. Las dos versiones ya contemplaban ese caso y la prueba confirmó que conservan el primer error.

---
**Firma digital:** j3sus55
