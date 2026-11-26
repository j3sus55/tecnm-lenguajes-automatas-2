#include "api_sesion10.h"

#include <ctype.h>
#include <limits.h>
#include <stdlib.h>

typedef struct {
    const char *s;
    int error;
} Parser;

static long leer(Parser *p, int minima);

static void saltar(Parser *p) {
    while (isspace((unsigned char)*p->s)) {
        ++p->s;
    }
}

static long primaria(Parser *p) {
    saltar(p);
    if (*p->s == '(') {
        ++p->s;
        long valor = leer(p, 1);
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

static int precedencia(char op) {
    switch (op) {
        case '+': case '-': return 1;
        case '*': case '/': return 2;
        default: return 0;
    }
}

static long operar(Parser *p, char op, long a, long b) {
    switch (op) {
        case '+':
            if ((b > 0 && a > LONG_MAX - b)
                || (b < 0 && a < LONG_MIN - b)) break;
            return a + b;
        case '-':
            if ((b > 0 && a < LONG_MIN + b)
                || (b < 0 && a > LONG_MAX + b)) break;
            return a - b;
        case '*':
            if (desborda_producto(a, b)) break;
            return a * b;
        case '/':
            if (b == 0) {
                p->error = 1;
                return 0;
            }
            if (a == LONG_MIN && b == -1) break;
            return a / b;
    }
    p->error = 2;
    return 0;
}

static long leer(Parser *p, int minima) {
    long valor = primaria(p);
    while (p->error == 0) {
        saltar(p);
        char op = *p->s;
        int nivel = precedencia(op);
        if (nivel < minima) break;
        ++p->s;
        long derecho = leer(p, nivel + 1);
        if (p->error != 0) return 0;
        valor = operar(p, op, valor, derecho);
    }
    return valor;
}

long evaluar_expresion(const char *fuente, int *error) {
    Parser p = {fuente, 0};
    long valor = 0;
    if (fuente == NULL) {
        p.error = 2;
    } else {
        valor = leer(&p, 1);
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
