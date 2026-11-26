#include "api_sesion01.h"
#include <string.h>

void invertir(char *s) {
    if (s == NULL) return;
    size_t length = strlen(s);
    for (size_t left = 0; left < length / 2; ++left) {
        size_t right = length - 1 - left;
        char saved = s[left];
        s[left] = s[right];
        s[right] = saved;
    }
}
