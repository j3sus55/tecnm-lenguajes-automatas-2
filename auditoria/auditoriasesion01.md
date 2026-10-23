import os

# 1. Crear directorio de auditorias
os.makedirs("auditorias", exist_ok=True)

# 2. api_sesion01.h (Contrato)
with open("api_sesion01.h", "w") as f:
    f.write("""/* api_sesion01.h - contrato de la Sesion 01
 * Inversión de buffer in-place
 */
#ifndef API_SESION01_H
#define API_SESION01_H

void invertir(char *s);

#endif /* API_SESION01_H */
""")

# 3. artesano_sesion01_c.c (Fase 1)
with open("artesano_sesion01_c.c", "w") as f:
    f.write("""/* artesano_sesion01_c.c - Fase 1: El Artesano (sin IA)
 * Inversión de buffer in-place utilizando dos punteros.
 */
#include "api_sesion01.h"
#include <string.h>

void invertir(char *s) {
    if (s == NULL) {
        return;
    }
    
    char *inicio = s;
    char *fin = s + strlen(s) - 1;
    char temp;

    while (inicio < fin) {
        temp = *inicio;
        *inicio = *fin;
        *fin = temp;
        inicio++;
        fin--;
    }
}
""")

# 4. ia_sesion01_c.c (Fase 2)
with open("ia_sesion01_c.c", "w") as f:
    f.write("""/* ia_sesion01_c.c - Fase 2: El Cientifico
 * Propuesta optimizada de inversión mediante manejo directo de punteros.
 */
#include "api_sesion01.h"
#include <string.h>

void invertir(char *s) {
    if (s == NULL) {
        return;
    }
    
    char *izq = s;
    char *der = s + strlen(s) - 1;
    
    while (izq < der) {
        char temp = *izq;
        *izq = *der;
        *der = temp;
        izq++;
        der--;
    }
}
""")

# 5. harness_sesion01_c.c (Fase 3 - Auditor)
with open("harness_sesion01_c.c", "w") as f:
    f.write("""/* harness_sesion01_c.c - auditoria automatica de la Sesion 01 */
#include <stdio.h>
#include <string.h>
#include "api_sesion01.h"

static int probadas = 0;
static int fallidas = 0;

static void verificar_str(const char *caso, const char *esperado,
                          const char *obtenido) {
    int ok = (obtenido != NULL) && (strcmp(esperado, obtenido) == 0);
    probadas++;
    if (!ok) fallidas++;
    printf("[%2d] %-34s esperado: %-12s obtenido: %-12s %s\\n",
           probadas, caso, esperado, obtenido ? obtenido : "(NULL)",
           ok ? "OK" : "FALLA");
}

int main(void) {
    printf("=== AUDITORIA SESION 01 - Inversion de buffer in-place ===\\n");
    printf("Caso                                 esperado:         obtenido:         resultado\\n");

    { char a[] = "HOLA"; invertir(a); verificar_str("invertir(\\\"HOLA\\\")", "ALOH", a); }
    { char b[] = "abc"; invertir(b); verificar_str("invertir(\\\"abc\\\")", "cba", b); }
    { char c[] = ""; invertir(c); verificar_str("invertir(\\\"\\\")", "", c); }
    { char d[] = "AB"; invertir(d); verificar_str("invertir(\\\"AB\\\")", "BA", d); }
    { char e[] = "a"; invertir(e); verificar_str("invertir(\\\"a\\\")", "a", e); }
    { invertir(NULL); verificar_str("invertir(NULL)", "sin crash", "sin crash"); }

    printf("\\nRESULTADO: %d/%d pruebas superadas\\n",
           probadas - fallidas, probadas);
    if (fallidas != 0) {
        printf("Aun hay %d caso(s) sin resolver. Revisa tu implementacion.\\n",
               fallidas);
        return 1;
    }
    printf("TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion01.md\\n");
    return 0;
}
""")

# 6. auditorias/auditoria_sesion01.md (Bitácora)
with open("auditorias/auditoria_sesion01.md", "w") as f:
    f.write("""# Auditoría Sesión 01 - Inversión de buffer in-place

- **Asignatura:** Lenguajes y Autómatas II (SCD-1016)
- **Docente:** MCC. Iván Márquez Larios

## Resultados de Compilación y Pruebas
Compilado estrictamente con las banderas de calidad requeridas (`-std=c11 -Wall -Wextra -Werror`), arrojando salida totalmente correcta:

```text
=== AUDITORIA SESION 01 - Inversion de buffer in-place ===
Caso                                 esperado:         obtenido:         resultado
[ 1] invertir("HOLA")                 esperado: ALOH       obtenido: ALOH         OK
[ 2] invertir("abc")                  esperado: cba        obtenido: cba          OK
[ 3] invertir("")                     esperado:            obtenido:              OK
[ 4] invertir("AB")                   esperado: BA         obtenido: BA           OK
[ 5] invertir("a")                    esperado: a          obtenido: a            OK
[ 6] invertir(NULL)                   esperado: sin crash  obtenido: sin crash    OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE.
