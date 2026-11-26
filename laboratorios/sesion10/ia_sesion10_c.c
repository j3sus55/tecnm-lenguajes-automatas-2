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
