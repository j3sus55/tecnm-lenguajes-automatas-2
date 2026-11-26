# Bitácora de Requerimientos y Auditoría IA — Sesión 05
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Recorridos del AST y Notación Polaca  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Impresión en Postorden / RPN  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Se identificó la necesidad de gestionar de forma estricta los límites del búfer (cap) para evitar desbordamientos de memoria al concatenar tokens recursivamente, garantizando que el búfer finalice siempre con el carácter terminador '\0' y evitando espacios en blanco sobrantes al final de la cadena generada.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 / GCC sin bibliotecas no estándar, manejo seguro de cadenas con snprintf y strncat. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Invariantes de recorrido recursivo Postorden (Izquierda -> Derecha -> Raíz) con validación de punteros nulos y límites de capacidad. |
| **Output (O)** | Función modular postorden con cumplimiento estricto del contrato api_sesion05.h. |

### 2.2 Prompt Estructurado
Contexto: Desarrollo de un compilador en C99, módulo de generación de Notación Polaca Inversa (RPN) a partir de un AST.
Rol: Ingeniero especialista en estructuras de datos y compiladores.
Especificación: Implementar la función void postorden(ASTNode *n, char *buf, int cap) que recorra el árbol en postorden (izq, der, raíz), escribiendo los tokens separados por un solo espacio en 'buf', respetando estrictamente la capacidad 'cap' y sin imprimir directamente en consola. Manejar árboles NULL produciendo una cadena vacía.
Output: Código fuente en C puro compatible con los tipos de datos NODO_NUM y NODO_BINOP definidos en api_sesion05.h, sin incluir función main().

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion05.h"
#include <stdio.h>
#include <string.h>

static void append_token(char *buf, int cap, const char *token) {
    size_t current_len = strlen(buf);
    size_t token_len = strlen(token);
    size_t needed = current_len + token_len + (current_len > 0 ? 1 : 0);
    
    if (needed >= (size_t)cap) return;
    
    if (current_len > 0) {
        buf[current_len] = ' ';
        buf[current_len + 1] = '\0';
    }
    strncat(buf, token, cap - strlen(buf) - 1);
}

static void postorden_rec(ASTNode *n, char *buf, int cap) {
    if (n == NULL) return;
    postorden_rec(n->izq, buf, cap);
    postorden_rec(n->der, buf, cap);
    
    char token[32];
    if (n->tipo == NODO_NUM) {
        snprintf(token, sizeof(token), "%d", n->valor);
    } else {
        token[0] = n->op;
        token[1] = '\0';
    }
    append_token(buf, cap, token);
}

void postorden(ASTNode *n, char *buf, int cap) {
    if (buf != NULL && cap > 0) buf[0] = '\0';
    if (n == NULL || buf == NULL || cap <= 0) return;
    postorden_rec(n, buf, cap);
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
Pruebas ejecutadas: recorrido postorden, árbol nulo y expresiones con operadores anidados.

- [x] Caso base y límites superados sin Crash.
- [ ] No existen accesos fuera de límites ni Memory Leaks. No se ejecutaron sanitizadores.
- [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?
> El tamaño del búfer limita cuántos tokens caben. Ambas implementaciones comprueban la capacidad antes de escribir el separador y el token. El harness oficial verifica la salida de cinco árboles y no mide la profundidad máxima de la pila.

---
**Firma digital:** `j3sus55`

## Ejecución del harness oficial

Pruebas ejecutadas con MSVC en C11 el 26 de noviembre de 2026, con advertencias tratadas como errores.

[Versión IA](../laboratorios/sesion05/ia_sesion05_c.c) · [Harness oficial](../laboratorios/sesion05/harness_sesion05_c.c)

### Versión IA

```text
=== AUDITORIA SESION 05 - Recorrido postorden y RPN ===
Caso                               esperado:              obtenido:              resultado
[ 1] hoja 42                            esperado: 42           obtenido: 42           OK
[ 2] 3+(5*2)                            esperado: 3 5 2 * +    obtenido: 3 5 2 * +    OK
[ 3] arbol NULL                         esperado:              obtenido:              OK
[ 4] (1+2)*(3+4)
     esperado: 1 2 + 3 4 + *
     obtenido: 1 2 + 3 4 + *
     -> OK
[ 5] 8-3                                esperado: 8 3 -        obtenido: 8 3 -        OK

RESULTADO: 5/5 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion05.md
```

### Archivo Artesano preparado con IA

Esta segunda implementación se preparó con asistencia de IA. La salida se capturó con el harness oficial y MSVC en modo C11, con advertencias tratadas como errores.

[Código de la segunda implementación](../laboratorios/sesion05/artesano_sesion05_c.c)

```text
=== AUDITORIA SESION 05 - Recorrido postorden y RPN ===
Caso                               esperado:              obtenido:              resultado
[ 1] hoja 42                            esperado: 42           obtenido: 42           OK
[ 2] 3+(5*2)                            esperado: 3 5 2 * +    obtenido: 3 5 2 * +    OK
[ 3] arbol NULL                         esperado:              obtenido:              OK
[ 4] (1+2)*(3+4)
     esperado: 1 2 + 3 4 + *
     obtenido: 1 2 + 3 4 + *
     -> OK
[ 5] 8-3                                esperado: 8 3 -        obtenido: 8 3 -        OK

RESULTADO: 5/5 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion05.md
```
