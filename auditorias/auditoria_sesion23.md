# Bitácora de Requerimientos y Auditoría IA — Sesión 23

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Etiquetas y Saltos Incondicionales  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Generador de etiquetas e instrucciones goto  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en utilizar el valor actual del contador antes de incrementarlo para producir etiquetas consecutivas. También hay que distinguir entre la instrucción `GOTO L2` y la marca `L2:`, conservando el mismo nombre de destino.
>
> La restricción de memoria principal es reservar espacio para el terminador nulo dentro de la capacidad del buffer. Además, una etiqueta NULL debe producir una cadena vacía y el modo de salto debe aceptar cualquier valor distinto de cero, incluyendo valores negativos.

Implementación de referencia asistida por IA para `artesano_sesion23_c.c`. Utiliza `snprintf` para generar las etiquetas y formatear el flujo.

Para los casos que el laboratorio deja sin definir, ambas versiones adoptan las siguientes decisiones. Un destino NULL o una capacidad no positiva producen una salida inmediata y conservan el contador. Un contador NULL, negativo o igual a `INT_MAX` deja una cadena vacía en un destino válido. El límite superior se rechaza para evitar desbordar el incremento.

Cuando el buffer es insuficiente se conserva un prefijo terminado en nulo y el contador válido se incrementa. La unicidad de las etiquetas completas requiere capacidad suficiente y conservar el mismo contador durante la secuencia.

El llamador debe proporcionar al menos `cap` bytes accesibles. La etiqueta de entrada y el contador deben ocupar almacenamiento independiente del destino.

```c
#include "api_sesion23.h"
#include <limits.h>
#include <stdio.h>

void nueva_etiqueta(char *dst, int cap, int *contador) {
    if (dst == NULL || cap <= 0) return;

    dst[0] = '\0';

    if (contador == NULL || *contador < 0 || *contador == INT_MAX) {
        return;
    }

    snprintf(dst, (size_t)cap, "L%d", *contador);

    ++*contador;
}

void emitir_flujo(char *dst, int cap, const char *etiqueta, int es_salto) {
    if (dst == NULL || cap <= 0) return;

    dst[0] = '\0';

    if (etiqueta == NULL) return;

    if (es_salto != 0) {
        snprintf(dst, (size_t)cap, "GOTO %s", etiqueta);
    } else {
        snprintf(dst, (size_t)cap, "%s:", etiqueta);
    }
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Trazabilidad del flujo de control. Generar etiquetas consecutivas y construir instrucciones GOTO o marcas mediante conversión decimal manual y copia acotada. |
| **Output (O)** | Solo código modular y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion23.h.
El harness utiliza -std=c11 -Wall -Wextra -Werror.
El destino tiene al menos cap bytes accesibles.
Las entradas y el contador son independientes del almacenamiento de dst.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva las firmas:
void nueva_etiqueta(char *dst, int cap, int *contador);
void emitir_flujo(char *dst, int cap, const char *etiqueta, int es_salto);

Implementa una función auxiliar de copia acotada que conserve
una posición para el terminador nulo.

En nueva_etiqueta, convierte manualmente el contador a decimal
mediante divisiones y residuos. Escribe L seguido de los dígitos
del valor actual y después incrementa el contador.

En emitir_flujo:
Si es_salto es distinto de cero, escribe "GOTO etiqueta".
Si es_salto es cero, escribe "etiqueta:".
Si etiqueta es NULL, produce una cadena vacía.
Conserva las etiquetas vacías como cadenas válidas.

Si dst es NULL o cap no es positivo, termina sin escribir.
Si el contador es NULL, negativo o INT_MAX, deja una cadena vacía
en un destino válido y conserva el contador.

Cuando el buffer sea insuficiente, escribe un prefijo terminado
en nulo e incrementa el contador válido.
Utiliza almacenamiento automático local y evita reservas dinámicas.

Output:
Entrega ia_sesion23_c.c sin main() y explica su complejidad.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion23.h"
#include <limits.h>
#include <stddef.h>

static void agregar(char *dst, int cap, int *usados, const char *texto) {
    while (*texto != '\0' && *usados < cap - 1) {
        dst[*usados] = *texto;
        ++*usados;
        ++texto;
    }

    dst[*usados] = '\0';
}

void nueva_etiqueta(char *dst, int cap, int *contador) {
    if (dst == NULL || cap <= 0) return;

    dst[0] = '\0';

    if (contador == NULL || *contador < 0 || *contador == INT_MAX) {
        return;
    }

    char digitos[sizeof(int) * CHAR_BIT + 1];
    unsigned int valor = (unsigned int)*contador;
    int cantidad = 0;

    do {
        digitos[cantidad++] = (char)('0' + valor % 10);
        valor /= 10;
    } while (valor != 0);

    int usados = 0;

    agregar(dst, cap, &usados, "L");

    while (cantidad > 0) {
        char caracter[2] = {digitos[--cantidad], '\0'};
        agregar(dst, cap, &usados, caracter);
    }

    ++*contador;
}

void emitir_flujo(char *dst, int cap, const char *etiqueta, int es_salto) {
    if (dst == NULL || cap <= 0) return;

    dst[0] = '\0';

    if (etiqueta == NULL) return;

    int usados = 0;

    if (es_salto != 0) {
        agregar(dst, cap, &usados, "GOTO ");
    }

    agregar(dst, cap, &usados, etiqueta);

    if (es_salto == 0) {
        agregar(dst, cap, &usados, ":");
    }
}
```

Sea `d` la cantidad de dígitos del contador y `L` la longitud de la salida completa.

`nueva_etiqueta` tiene tiempo `O(d)` en ambas implementaciones. Para una plataforma con int de tamaño fijo, la cantidad máxima de dígitos está acotada.

`emitir_flujo` tiene tiempo `O(L)` en Artesano, considerando el procesamiento de la cadena mediante `snprintf`. La versión de IA realiza copia acotada, con tiempo `O(min(L, cap))`.

Ambas versiones utilizan espacio auxiliar `O(1)` para los tipos de la plataforma. El arreglo local de dígitos tiene un tamaño fijo suficiente para representar el contador.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Saltos adelante y atrás estructurados

Ambas implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Las dos superaron los seis casos del harness original y terminaron con código de salida cero.

También se ejecutaron pruebas complementarias de generación consecutiva de `L9` y `L10`, actualización del contador, modo de salto negativo, etiqueta NULL en modo marca, etiquetas vacías, buffers pequeños, capacidades cero, uno y negativa, destino NULL, contador NULL, contador negativo y contador igual a `INT_MAX`.

Una prueba con centinela comprobó que la escritura truncada conservó un byte situado fuera de la capacidad declarada. Todas las comprobaciones terminaron correctamente.

Queda pendiente repetir la compilación y ejecución con GCC y verificar exhaustivamente las capacidades próximas a la longitud completa de cada salida. Las pruebas realizadas comprueban el texto generado; la resolución y ejecución de destinos pertenecen a etapas posteriores.

### Salida del harness

Salida obtenida en ambas implementaciones, presentada hasta el total. El programa imprime después su mensaje de cierre.

```text
=== AUDITORIA SESION 23 - Etiquetas y saltos incondicionales ===
Caso                               esperado:              obtenido:              resultado
[ 1] nueva_etiqueta(L0)                 esperado: L0           obtenido: L0           OK
[ 2] nueva_etiqueta(L5)                 esperado: L5           obtenido: L5           OK
[ 3] emitir_flujo(GOTO L2)              esperado: GOTO L2      obtenido: GOTO L2      OK
[ 4] emitir_flujo(marca L2)             esperado: L2:          obtenido: L2:          OK
[ 5] emitir_flujo(NULL)                 esperado:              obtenido:              OK
[ 6] nueva_etiqueta(L10)                esperado: L10          obtenido: L10          OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites superados sin *Crash* en las pruebas ejecutadas.
* [x] La revisión confirma escritura acotada a cap y ausencia de reservas dinámicas. La prueba con centinela confirmó la conservación del byte externo a la capacidad declarada.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> El harness original utiliza `es_salto` con valores cero y uno, pero el contrato establece que cualquier valor distinto de cero debe producir GOTO. Una implementación que compruebe únicamente `es_salto == 1` podría superar esos casos y fallar con valores negativos. Se agregó una prueba con `es_salto = -3`, y ambas versiones produjeron `GOTO L2`.
>
> También se comprobó el incremento del contador mediante dos llamadas consecutivas iniciadas en nueve. Se obtuvieron `L9` y `L10`, con valores finales del contador diez y once respectivamente.
>
> La versión de IA entregada ya contempla estas condiciones. La auditoría amplió la cobertura del harness sin requerir una corrección del código.

---

**Firma digital:** [mi usuario de GitHub]
