# Bitácora de Requerimientos y Auditoría IA — Sesión 10
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Hito 1: Mini-Evaluador de Expresiones  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Integración AST y Tipado Estático  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> El reto fue respetar la precedencia sin cambiar la firma de la API. La implementación separa `primaria` (enteros y paréntesis), `termino` (multiplicación y división) y `expr` (suma y resta). Así, `2+3*4` da 14 y `(2+3)*4` da 20. La solución de referencia para esta fase se preparó con ayuda de IA.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C11, biblioteca estándar de C y la interfaz de `api_sesion10.h`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Enteros decimales, operadores binarios `+`, `-`, `*` y `/`, espacios y paréntesis. División entre cero con código 1 y sintaxis inválida con código 2. |
| **Output (O)** | Código C sin `main()` y análisis de complejidad. |

### 2.2 Prompt Estructurado
```text
Contexto: laboratorio de C11 con un harness que enlaza api_sesion10.h.
Rol: ingeniero de software de sistemas y compiladores.
Especificación: implementa long evaluar_expresion(const char *fuente, int *error)
por descenso recursivo. Acepta enteros decimales, espacios, paréntesis y los
operadores binarios +, -, * y /. Respeta la precedencia y la asociatividad
izquierda. Rechaza el menos unario y exige consumir toda la entrada.
Devuelve 0 si hay error e informa 1 para división entre cero y 2 para sintaxis
inválida. En caso de éxito, escribe 0 en error si el puntero está disponible.
Salida: entrega un archivo C sin main() y explica su complejidad.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion10.h"

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

typedef struct {
    const char *s;
    int error;
} Parser;

static long expr(Parser *p);

static void saltar(Parser *p) {
    while (isspace((unsigned char)*p->s)) {
        ++p->s;
    }
}

static long primaria(Parser *p) {
    saltar(p);
    if (*p->s == '(') {
        ++p->s;
        long valor = expr(p);
        saltar(p);
        if (p->error == 0) {
            if (*p->s == ')') {
                ++p->s;
            } else {
                p->error = 2;
            }
        }
        return valor;
    }

    if (*p->s < '0' || *p->s > '9') {
        p->error = 2;
        return 0;
    }

    long valor = 0;
    while (*p->s >= '0' && *p->s <= '9') {
        int digito = *p->s++ - '0';
        if (valor > (LONG_MAX - digito) / 10) {
            p->error = 2;
            return 0;
        }
        valor = valor * 10 + digito;
    }
    return valor;
}

static int desborda_producto(long a, long b) {
    if (a > 0) {
        return b > LONG_MAX / a || b < LONG_MIN / a;
    }
    if (a < 0) {
        return (b > 0 && a < LONG_MIN / b)
            || (b < 0 && a < LONG_MAX / b);
    }
    return 0;
}

static long termino(Parser *p) {
    long valor = primaria(p);
    while (p->error == 0) {
        saltar(p);
        char op = *p->s;
        if (op != '*' && op != '/') {
            break;
        }
        ++p->s;
        long derecho = primaria(p);
        if (p->error != 0) {
            break;
        }
        if (op == '*') {
            if (desborda_producto(valor, derecho)) {
                p->error = 2;
                break;
            }
            valor *= derecho;
        } else {
            if (derecho == 0) {
                p->error = 1;
                break;
            }
            if (valor == LONG_MIN && derecho == -1) {
                p->error = 2;
                break;
            }
            valor /= derecho;
        }
    }
    return valor;
}

static long expr(Parser *p) {
    long valor = termino(p);
    while (p->error == 0) {
        saltar(p);
        char op = *p->s;
        if (op != '+' && op != '-') {
            break;
        }
        ++p->s;
        long derecho = termino(p);
        if (p->error != 0) {
            break;
        }
        if (op == '+') {
            if ((derecho > 0 && valor > LONG_MAX - derecho)
                || (derecho < 0 && valor < LONG_MIN - derecho)) {
                p->error = 2;
                break;
            }
            valor += derecho;
        } else {
            if ((derecho > 0 && valor < LONG_MIN + derecho)
                || (derecho < 0 && valor > LONG_MAX + derecho)) {
                p->error = 2;
                break;
            }
            valor -= derecho;
        }
    }
    return valor;
}

long evaluar_expresion(const char *fuente, int *error) {
    Parser p = {fuente, 0};
    long valor = 0;
    if (fuente == NULL) {
        p.error = 2;
    } else {
        valor = expr(&p);
        saltar(&p);
        if (p.error == 0 && *p.s != '\0') {
            p.error = 2;
        }
    }
    if (error != NULL) {
        *error = p.error;
    }
    return p.error == 0 ? valor : 0;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Precedencia, paréntesis, división entre cero, sintaxis inválida, espacios y resultado negativo. Se compiló el harness original con MSVC en modo C11 (`/W4 /WX`). Las dos versiones pasaron 6 de 6 casos. La salida de abajo corresponde a El Científico.

### Salida del harness
```text
=== AUDITORIA SESION 10 - Mini-evaluador de expresiones (Hito 1) ===
Caso                               esperado:              obtenido:              resultado
[ 1] "2+3*4"                            esperado: 14           obtenido: 14           OK
[ 2] "(2+3)*4"                          esperado: 20           obtenido: 20           OK
[ 3] "10/0" -> error 1                  esperado: 1            obtenido: 1            OK
[ 4] "2+" -> error 2                    esperado: 2            obtenido: 2            OK
[ 5] "  7 "                             esperado: 7            obtenido: 7            OK
[ 6] "5-10"                             esperado: -5           obtenido: -5           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion10.md
```

* [x] Caso base y límites superados sin *Crash*.
* [ ] Accesos fuera de límites y fugas. El código no asigna memoria dinámica. No se ejecutó un analizador de memoria.
* [x] El analizador recorre la entrada en O(n) y usa O(d) de pila, donde d es la profundidad de los paréntesis.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> Las seis pruebas del harness original no cubren el desbordamiento de `long`. La solución generada ya comprueba el rango del literal y de las operaciones antes de calcular. En pruebas adicionales, `LONG_MAX` fue aceptado, mientras que sumarle 1 o agregarle un dígito produjo el código 2. También pasaron cinco casos adicionales de sintaxis, asociatividad y división entre cero.

---
**Firma digital:** `j3sus55`
