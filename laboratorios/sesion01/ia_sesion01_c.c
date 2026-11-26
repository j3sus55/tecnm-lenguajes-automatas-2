#include "api_sesion01.h"
#include <string.h>

void invertir(char *s) {
    if (s == NULL || *s == '\0') return;

    char *last = s + strlen(s) - 1;
    while (s < last) {
        char first = *s;
        *s++ = *last;
        *last-- = first;
    }
}
