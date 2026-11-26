
# Bitácora de Requerimientos y Auditoría IA — Sesión 04

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Anatomía de un Árbol Sintáctico Abstracto (AST)  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles 

**Matrícula:** 24531356  

**Track:** C

---

## 1. Fase 1: El Artesano (Dificultad Manual)

**Reto:** Estructura de nodo binario (ASTNode).

La principal dificultad al realizar el ejercicio fue comprender cómo reservar memoria para los nodos y enlazar correctamente los hijos izquierdo y derecho. También fue necesario distinguir entre nodos numéricos y nodos de operaciones mediante el campo `tipo`. Para resolverlo, se utilizan punteros y memoria dinámica con `malloc()`. Además, se debe comprobar si la reserva de memoria fue exitosa y evitar acceder a nodos NULL durante la evaluación recursiva.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
|---|---|
| **Contexto (C)** | C11 con GCC, sin bibliotecas no estándar y compatible con `api_sesion04.h`. |
| **Rol (R)** | Ingeniero de software especializado en sistemas y compiladores. |
| **Especificación (E)** | Crear un AST con nodos numéricos y binarios, soportando los operadores `+`, `-` y `*`. Evaluar recursivamente con tiempo O(n). |
| **Output (O)** | Código modular sin `main()`, con manejo de NULL y análisis de complejidad. |

### 2.2 Prompt Estructurado

Actúa como un ingeniero de software especializado en sistemas y compiladores. Implementa en C las funciones declaradas en `api_sesion04.h`, respetando exactamente sus firmas:

ASTNode *crear_numero(int v);
ASTNode *crear_binario(char op, ASTNode *izq, ASTNode *der);
int evaluar(ASTNode *n);

Requisitos:
1. Reservar cada nodo con malloc().
2. Configurar los nodos numéricos con tipo NODO_NUM y guardar el número en valor.
3. Configurar los nodos binarios con tipo NODO_BINOP, guardar el operador y enlazar los hijos izquierdo y derecho.
4. Soportar únicamente los operadores +, - y *.
5. Devolver -1 si evaluar recibe NULL.
6. Evaluar recursivamente los hijos antes de aplicar el operador.
7. No incluir main(), porque el harness proporciona esa función.
8. Compilar con GCC usando -Wall -Wextra -Werror.
9. Entregar código modular con análisis de complejidad temporal y espacial.

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion04.h"
#include <stdlib.h>

ASTNode *crear_numero(int v)
{
    ASTNode *n = malloc(sizeof(ASTNode));

    if (n == NULL) {
        return NULL;
    }

    n->tipo = NODO_NUM;
    n->valor = v;
    n->op = '\0';
    n->izq = NULL;
    n->der = NULL;

    return n;
}

ASTNode *crear_binario(char op, ASTNode *izq, ASTNode *der)
{
    ASTNode *n;

    if (op != '+' && op != '-' && op != '*') {
        return NULL;
    }

    if (izq == NULL || der == NULL) {
        return NULL;
    }

    n = malloc(sizeof(ASTNode));

    if (n == NULL) {
        return NULL;
    }

    n->tipo = NODO_BINOP;
    n->valor = 0;
    n->op = op;
    n->izq = izq;
    n->der = der;

    return n;
}

int evaluar(ASTNode *n)
{
    int izquierda;
    int derecha;

    if (n == NULL) {
        return -1;
    }

    if (n->tipo == NODO_NUM) {
        return n->valor;
    }

    if (n->tipo != NODO_BINOP) {
        return -1;
    }

    izquierda = evaluar(n->izq);
    derecha = evaluar(n->der);

    if (izquierda == -1 || derecha == -1) {
        return -1;
    }

    switch (n->op) {
        case '+':
            return izquierda + derecha;

        case '-':
            return izquierda - derecha;

        case '*':
            return izquierda * derecha;

        default:
            return -1;
    }
}
```

### Análisis de complejidad

- **Creación de un nodo:** O(1), porque se reserva y configura una estructura.
- **Evaluación del árbol:** O(n), porque cada nodo se visita una vez.
- **Memoria auxiliar de evaluación:** O(h), donde h representa la altura del árbol debido a la recursión.
- **Memoria total del AST:** O(n), porque cada nodo ocupa memoria dinámica.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

**Pruebas del laboratorio:**
1. Evaluar el nodo numérico 42.
2. Evaluar la expresión 3 + 5 * 2.
3. Comprobar el hijo izquierdo del nodo de multiplicación.
4. Verificar el operador almacenado.
5. Evaluar NULL.
6. Evaluar la expresión 10 - 4.

### Salida esperada

```text
=== AUDITORIA SESION 04 - Nodo binario de AST ===
Caso                               esperado:              obtenido:              resultado
[ 1] evaluar(nodo 42)               esperado: 42            obtenido: 42            OK
[ 2] evaluar(3+5*2)                 esperado: 13            obtenido: 13            OK
[ 3] hijo izquierdo de (* 5 2)      esperado: 5             obtenido: 5             OK
[ 4] operador guardado              esperado: *             obtenido: *             OK
[ 5] evaluar(NULL)                  esperado: -1            obtenido: -1            OK
[ 6] evaluar(10-4)                  esperado: 6             obtenido: 6             OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE.
```

### Lista de auditoría

- [ ] Caso base y límites superados sin Crash.
- [ ] No existen accesos fuera de límites ni Memory Leaks.
- [ ] La complejidad teórica coincide con la implementada.

Las casillas deben marcarse después de ejecutar el programa y comprobar los resultados.

### Reporte Crítico

El principal caso borde que se debía considerar era evaluar un árbol vacío representado por NULL. Si se intentara acceder a sus campos sin comprobar el puntero, el programa podría provocar un error de segmentación. Para solucionarlo, se agregó una validación al inicio de la función evaluar(), que devuelve -1 cuando el nodo es NULL. También se verificó que los nodos binarios almacenen correctamente el operador y mantengan el orden de los hijos izquierdo y derecho. La evaluación recursiva permite calcular expresiones como 3 + 5 * 2 respetando la estructura del árbol. La implementación debe probarse con el harness para confirmar que los seis casos pasan. Para evitar fugas de memoria en un programa completo, se recomienda liberar los nodos al finalizar su uso mediante una función de destrucción del árbol.

---

**Firma digital:** [Usuario de GitHub]

---

## Comandos para ejecutar en Google Colab

Para probar la implementación artesanal:

```bash
!gcc -std=c11 -Wall -Wextra -Werror harness_sesion04_c.c artesano_sesion04_c.c -o test_artesano && ./test_artesano
```

Para probar la implementación de IA:

```bash
!gcc -std=c11 -Wall -Wextra -Werror harness_sesion04_c.c ia_sesion04_c.c -o test_ia && ./test_ia
```
