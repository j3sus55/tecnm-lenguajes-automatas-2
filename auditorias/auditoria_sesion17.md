# Bitácora de Requerimientos y Auditoría IA — Sesión 17

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Comprobación Semántica de Retornos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Validación de tipo de sentencia return.

*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en respetar la prioridad de los diagnósticos. Primero se validan los argumentos y los conteos de rutas. Después se comprueba el tipo del retorno y finalmente la cobertura de rutas. Si una función promete `int`, devuelve `double` y además deja una ruta abierta, corresponde informar incompatibilidad de tipo con `1`.
>
> También hay que tratar `void` por separado. Una función `void` acepta una expresión identificada como `void`, mientras que devolver un valor numérico produce un error de tipo. La obligación de retornar en todas las rutas se aplica a las funciones que devuelven un valor.

Implementación de referencia asistida por IA para `artesano_sesion17_c.c`. Utiliza rangos numéricos y condiciones para comprobar la compatibilidad.

El material proporciona la regla de ampliación y ejemplos con `int`, `double` y `void`, pero deja abierta la lista completa de tipos. Para ambas implementaciones se adopta la jerarquía `char < short < int < float < double`, con `void` tratado aparte. Los tipos desconocidos producen `-1`.

```c
#include "api_sesion17.h"
#include <string.h>

static int rango_tipo(const char *tipo) {
    if (strcmp(tipo, "void") == 0) return 0;
    if (strcmp(tipo, "char") == 0) return 1;
    if (strcmp(tipo, "short") == 0) return 2;
    if (strcmp(tipo, "int") == 0) return 3;
    if (strcmp(tipo, "float") == 0) return 4;
    if (strcmp(tipo, "double") == 0) return 5;

    return -1;
}

int diagnosticar_retorno(const char *tipo_funcion,
                        const char *tipo_expresion,
                        int rutas_totales, int rutas_con_return) {
    if (tipo_funcion == NULL || tipo_expresion == NULL ||
        rutas_totales <= 0 || rutas_con_return < 0 ||
        rutas_con_return > rutas_totales) {
        return -1;
    }

    int destino = rango_tipo(tipo_funcion);
    int origen = rango_tipo(tipo_expresion);

    if (destino < 0 || origen < 0) {
        return -1;
    }

    if (destino == 0) {
        if (origen == 0) return 0;
        return 1;
    }

    if (origen == 0 || origen > destino) {
        return 1;
    }

    if (rutas_con_return < rutas_totales) {
        return 2;
    }

    return 0;
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Comprobar compatibilidad del retorno mediante una matriz y verificar rutas de ejecución con retorno garantizado. Priorizar el error de tipo sobre la falta de retorno. |
| **Output (O)** | Código modular para `ia_sesion17_c.c` y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion17.h.
El harness utiliza -std=c11 -Wall -Wextra -Werror.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva la firma:
int diagnosticar_retorno(const char *tipo_funcion,
                        const char *tipo_expresion,
                        int rutas_totales, int rutas_con_return);

Asigna índices a void, char, short, int, float y double.
Usa una matriz constante para expresar la compatibilidad.
Las filas representan el tipo prometido por la función y las
columnas el tipo de la expresión.

Aplica la jerarquía char < short < int < float < double.
Acepta conversiones hacia un rango igual o mayor.
void solo es compatible con void.

Devuelve:
-1 si un tipo es NULL o desconocido, si rutas_totales es menor
o igual a cero, o si rutas_con_return está fuera del intervalo
de cero a rutas_totales.
1 si los tipos son incompatibles.
2 si una función que devuelve un valor tiene rutas sin retorno.
0 si las comprobaciones son correctas.

Respeta el orden de validación:
entradas, compatibilidad de tipos y cobertura de rutas.
Una función void con expresión void puede finalizar sin un
return explícito en todas las rutas.

Output:
Entrega ia_sesion17_c.c sin main() y explica su complejidad.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion17.h"
#include <string.h>

static int indice_tipo(const char *tipo) {
    static const char *const nombres[] = {
        "void", "char", "short", "int", "float", "double"
    };

    for (int i = 0; i < 6; ++i) {
        if (strcmp(tipo, nombres[i]) == 0) {
            return i;
        }
    }

    return -1;
}

int diagnosticar_retorno(const char *tipo_funcion,
                        const char *tipo_expresion,
                        int rutas_totales, int rutas_con_return) {
    static const unsigned char compatibles[6][6] = {
        {1, 0, 0, 0, 0, 0},
        {0, 1, 0, 0, 0, 0},
        {0, 1, 1, 0, 0, 0},
        {0, 1, 1, 1, 0, 0},
        {0, 1, 1, 1, 1, 0},
        {0, 1, 1, 1, 1, 1}
    };

    if (tipo_funcion == NULL || tipo_expresion == NULL ||
        rutas_totales <= 0 || rutas_con_return < 0 ||
        rutas_con_return > rutas_totales) {
        return -1;
    }

    int destino = indice_tipo(tipo_funcion);
    int origen = indice_tipo(tipo_expresion);

    if (destino < 0 || origen < 0) {
        return -1;
    }

    if (!compatibles[destino][origen]) {
        return 1;
    }

    if (destino != 0 && rutas_con_return < rutas_totales) {
        return 2;
    }

    return 0;
}
```

Ambas versiones tienen tiempo `O(1)` y espacio auxiliar `O(1)` para el conjunto fijo de tipos definido. La matriz contiene 36 posiciones y la búsqueda de tipos realiza como máximo seis comparaciones con nombres de longitud acotada.

El conteo de rutas se recibe como argumento. La función compara esos conteos sin recorrer un árbol sintáctico ni generar las rutas de control.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Funciones sin retorno o retornos mixtos.

Las dos implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Ambas superaron los seis casos del harness original y terminaron con código de salida `0`.

También se ejecutaron veinte comprobaciones complementarias en cada versión:

- Tipos de función o expresión NULL.
- Conteos de rutas negativos y retornos superiores al total.
- Incompatibilidad de tipo junto con una ruta sin retorno.
- Función void que devuelve un valor y deja una ruta abierta.
- Compatibilidad entre void y void con cero retornos explícitos y con cobertura completa.
- Rechazo de una expresión void en una función int.
- Función int con rutas válidas pero sin retornos.
- Ampliación de float a double y rechazo de la conversión inversa.
- Ampliación de char a short y rechazo de la conversión inversa.
- Tipos desconocidos, cadenas vacías y diferencias de mayúsculas.
- Conteos iguales a INT_MAX, con cobertura completa e incompleta.

Todas las comprobaciones complementarias terminaron correctamente. Queda pendiente repetir la compilación y ejecución con GCC en el entorno del laboratorio y comprobar de manera exhaustiva las 36 combinaciones de tipos.

```sh
gcc -std=c11 -Wall -Wextra -Werror harness_sesion17_c.c artesano_sesion17_c.c -o test_artesano && ./test_artesano

gcc -std=c11 -Wall -Wextra -Werror harness_sesion17_c.c ia_sesion17_c.c -o test_ia && ./test_ia
```

### Salida del harness

Ambas implementaciones produjeron las siguientes comprobaciones. El harness imprime su mensaje de cierre después del total mostrado.

```text
=== AUDITORIA SESION 17 - Validacion de retornos y rutas ===
Caso                               esperado:              obtenido:              resultado
[ 1] int con int y dos rutas            esperado: 0            obtenido: 0            OK
[ 2] void devuelve valor                esperado: 1            obtenido: 1            OK
[ 3] ruta sin retorno                   esperado: 2            obtenido: 2            OK
[ 4] double acepta int                  esperado: 0            obtenido: 0            OK
[ 5] int no acepta double               esperado: 1            obtenido: 1            OK
[ 6] cero rutas de ejecucion            esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites de entrada superados sin *Crash* en las pruebas ejecutadas.
* [x] La revisión confirma que los índices se validan antes de acceder a la matriz. Ambas versiones carecen de reservas dinámicas de memoria.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> El harness original comprueba por separado la incompatibilidad de tipos y las rutas sin retorno, pero deja sin evaluar su aparición simultánea. Si se revisan las rutas primero, una llamada como `diagnosticar_retorno("int", "double", 2, 1)` puede devolver `2`, aunque el contrato exige priorizar el error de tipo con `1`.
>
> Se agregó esa prueba complementaria y ambas versiones devolvieron `1`. También se comprobó que una función void con expresión void pueda finalizar sin retornos explícitos y que los conteos incoherentes produzcan `-1`. La versión de IA entregada ya contempla estas condiciones, por lo que la auditoría amplió la cobertura de pruebas.

---

**Firma digital:** [mi usuario de GitHub]
