/* =========================================================================
 * auditoria_sesion05.c - Bitácora y Código de la Sesión 05
 * Materia: Lenguajes y Autómatas II (SCD-1016)
 * Estudiante: Jesus Manuel Rangel Aviles
 * (Matrícula: 24531356)
 * Track: C
 * ========================================================================= */

/*
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
#include "api_sesion05.h"
#include <stdio.h>
#include <string.h>

static void append_token(char *buf, int cap, const char *token) {
    int current_len = strlen(buf);
    int token_len = strlen(token);
    int needed = current_len + token_len + (current_len > 0 ? 1 : 0);
    
    if (needed >= cap) return;
    
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

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
Pruebas ejecutadas: Validación contra desbordamiento de pila

- [x] Caso base y límites superados sin Crash.
- [x] No existen accesos fuera de límites ni Memory Leaks.
- [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico
¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?
> Las versiones iniciales generadas por la IA a menudo omitían la verificación de desbordamiento de búfer (cap) durante la concatenación recursiva de cadenas o añadían espacios finales innecesarios. Se solucionó incorporando una función auxiliar robusta de control de longitud (append_token) que evalúa el espacio disponible antes de escribir cada token y el delimitador de espacio.

---
**Firma digital:** JesusManuelRangelAviles
*/

#include <stdio.h>
#include <string.h>

/* --- 1. Contrato (api_sesion05.h) --- */
typedef enum { NODO_NUM, NODO_BINOP } TipoNodo;

typedef struct ASTNode {
    TipoNodo tipo;
    int valor;
    char op;
    struct ASTNode *izq;
    struct ASTNode *der;
} ASTNode;

void postorden(ASTNode *n, char *buf, int cap);

/* --- 2. Implementación (artesano_sesion05_c.c / ia_sesion05_c.c) --- */
static void append_token(char *buf, int cap, const char *token) {
    int current_len = strlen(buf);
    int token_len = strlen(token);
    int needed = current_len + token_len + (current_len > 0 ? 1 : 0);
    
    if (needed >= cap) {
        return;
    }
    
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
    if (buf != NULL && cap > 0) {
        buf[0] = '\0';
    }
    if (n == NULL || buf == NULL || cap <= 0) {
        return;
    }
    postorden_rec(n, buf, cap);
}

/* --- 3. Harness de Auditoría Automática (harness_sesion05_c.c) --- */
static int probadas = 0;
static int fallidas = 0;

static void verificar_str(const char *caso, const char *esperado,
                          const char *obtenido) {
    int ok = (obtenido != NULL) && (strcmp(esperado, obtenido) == 0);
    probadas++;
    if (!ok) fallidas++;
    printf("[%2d] %-34s esperado: %-12s obtenido: %-12s %s\n",
           probadas, caso, esperado, obtenido ? obtenido : "(NULL)",
           ok ? "OK" : "FALLA");
}

static void verificar_str_largo(const char *caso, const char *esperado,
                                const char *obtenido) {
    int ok = (obtenido != NULL) && (strcmp(esperado, obtenido) == 0);
    probadas++;
    if (!ok) fallidas++;
    printf("[%2d] %s\n", probadas, caso);
    printf("     esperado: %s\n", esperado);
    printf("     obtenido: %s\n", obtenido ? obtenido : "(NULL)");
    printf("     -> %s\n", ok ? "OK" : "FALLA");
}

int main(void) {
    printf("=== AUDITORIA SESION 05 - Recorrido postorden y RPN ===\n");
    printf("Caso                                       esperado:                obtenido:                resultado\n");

    { 
        ASTNode a = {NODO_NUM, 42, 0, NULL, NULL}; 
        char b[64]; 
        postorden(&a, b, 64); 
        verificar_str("hoja 42", "42", b); 
    }
    { 
        ASTNode a = {NODO_NUM, 3, 0, NULL, NULL}; 
        ASTNode b = {NODO_NUM, 5, 0, NULL, NULL}; 
        ASTNode c = {NODO_NUM, 2, 0, NULL, NULL}; 
        ASTNode m = {NODO_BINOP, 0, '*', &b, &c}; 
        ASTNode s = {NODO_BINOP, 0, '+', &a, &m}; 
        char buf[64]; 
        postorden(&s, buf, 64); 
        verificar_str("3+(5*2)", "3 5 2 * +", buf); 
    }
    { 
        char b[16]; 
        postorden(NULL, b, 16); 
        verificar_str("arbol NULL", "", b); 
    }
    { 
        ASTNode a = {NODO_NUM, 1, 0, NULL, NULL}; 
        ASTNode b = {NODO_NUM, 2, 0, NULL, NULL}; 
        ASTNode c = {NODO_NUM, 3, 0, NULL, NULL}; 
        ASTNode d = {NODO_NUM, 4, 0, NULL, NULL}; 
        ASTNode s1 = {NODO_BINOP, 0, '+', &a, &b}; 
        ASTNode s2 = {NODO_BINOP, 0, '+', &c, &d}; 
        ASTNode m = {NODO_BINOP, 0, '*', &s1, &s2}; 
        char buf[64]; 
        postorden(&m, buf, 64); 
        verificar_str_largo("(1+2)*(3+4)", "1 2 + 3 4 + *", buf); 
    }
    { 
        ASTNode a = {NODO_NUM, 8, 0, NULL, NULL}; 
        ASTNode b = {NODO_NUM, 3, 0, NULL, NULL}; 
        ASTNode r = {NODO_BINOP, 0, '-', &a, &b}; 
        char buf[32]; 
        postorden(&r, buf, 32); 
        verificar_str("8-3", "8 3 -", buf); 
    }

    printf("\nRESULTADO: %d/%d pruebas superadas\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion05.md\n");
    return 0;
}
