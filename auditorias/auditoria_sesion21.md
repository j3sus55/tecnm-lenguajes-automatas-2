# Bitácora de Requerimientos y Auditoría IA — Sesión 21

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Fundamentos de Código de 3 Direcciones  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)

---

## 1. Fase 1: El Artesano (Dificultad Manual)

*Reto:* Representación en Cuádruplos (op,a,b,r)  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*

> La dificultad técnica consiste en generar el temporal con el valor actual del contador y después incrementarlo. Si el incremento se realiza primero, la secuencia iniciada en cero comienza incorrectamente con `_t1`. También hay que conservar exactamente los paréntesis, comas y espacios del cuádruplo.
>
> La restricción de memoria principal es respetar la capacidad del buffer, incluyendo el terminador nulo. Los campos NULL deben convertirse en `"_"` antes de utilizarlos como cadenas.

Implementación de referencia asistida por IA para `artesano_sesion21_c.c`. Utiliza `snprintf` para limitar la escritura al buffer recibido.

Para los casos que el laboratorio deja sin definir, se adoptan estas decisiones. Un destino NULL o una capacidad no positiva producen una salida inmediata y conservan el contador. Un contador NULL, negativo o igual a `INT_MAX` deja una cadena vacía en un destino válido. Esto evita desbordar el contador. Si el buffer es pequeño, se guarda un prefijo terminado en nulo y el contador válido se incrementa.

El llamador debe proporcionar un buffer de al menos `cap` bytes. Las cadenas de entrada y el contador deben ocupar almacenamiento independiente del destino. Para obtener temporales completos y únicos, el buffer debe tener capacidad suficiente.

```c
#include "api_sesion21.h"
#include <limits.h>
#include <stdio.h>

void nuevo_temporal(char *dst, int cap, int *contador) {
    if (dst == NULL || cap <= 0) return;

    dst[0] = '\0';

    if (contador == NULL || *contador < 0 || *contador == INT_MAX) {
        return;
    }

    snprintf(dst, (size_t)cap, "_t%d", *contador);

    ++*contador;
}

void emitir_cuadruplo(char *dst, int cap, const char *op,
                     const char *a1, const char *a2, const char *res) {
    if (dst == NULL || cap <= 0) return;

    if (op == NULL) op = "_";
    if (a1 == NULL) a1 = "_";
    if (a2 == NULL) a2 = "_";
    if (res == NULL) res = "_";

    snprintf(dst, (size_t)cap, "(%s, %s, %s, %s)", op, a1, a2, res);
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C99 y bibliotecas estándar. El harness utiliza GCC con `-std=c11 -Wall -Wextra -Werror`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Generación de temporales únicos (_t0..) y cuádruplos con formato exacto. Convertir el contador a decimal manualmente y construir la salida mediante copia acotada. |
| **Output (O)** | Solo código modular y análisis de complejidad. |

### 2.2 Prompt Estructurado

```text
Contexto:
Implementa un módulo C que incluya api_sesion21.h.
El harness utiliza -std=c11 -Wall -Wextra -Werror.
El destino tiene al menos cap bytes accesibles.
Las entradas y el contador ocupan almacenamiento independiente del destino.

Rol:
Actúa como ingeniero de software de sistemas y compiladores.

Especificación:
Conserva las firmas:
void nuevo_temporal(char *dst, int cap, int *contador);
void emitir_cuadruplo(char *dst, int cap, const char *op,
                     const char *a1, const char *a2, const char *res);

Implementa una función auxiliar de copia acotada que reserve
una posición para el terminador nulo.

En nuevo_temporal, convierte manualmente el contador a decimal
mediante divisiones y residuos. Escribe el prefijo _t, agrega los
dígitos en orden y después incrementa el contador.

En emitir_cuadruplo, construye el formato (op, a1, a2, res).
Representa cada campo NULL como un guion bajo.
Conserva las cadenas vacías como campos vacíos.

Si dst es NULL o cap no es positivo, termina sin escribir.
Con un destino válido, rechaza contador NULL, negativo o INT_MAX
dejando una cadena vacía.
Cuando el buffer es insuficiente, conserva un prefijo terminado
en nulo. Incrementa el contador válido incluso si hubo truncamiento.

Escribe el resultado en dst, utiliza únicamente almacenamiento
automático local y evita variables globales y reservas dinámicas.

Output:
Entrega ia_sesion21_c.c sin main() y explica su complejidad.
```

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion21.h"
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

void nuevo_temporal(char *dst, int cap, int *contador) {
    if (dst == NULL || cap <= 0) return;

    dst[0] = '\0';

    if (contador == NULL || *contador < 0 || *contador == INT_MAX) {
        return;
    }

    char digitos[sizeof(int) * CHAR_BIT + 1];
    int cantidad = 0;
    unsigned int valor = (unsigned int)*contador;

    do {
        digitos[cantidad++] = (char)('0' + valor % 10);
        valor /= 10;
    } while (valor != 0);

    int usados = 0;

    agregar(dst, cap, &usados, "_t");

    while (cantidad > 0) {
        char caracter[2] = {digitos[--cantidad], '\0'};
        agregar(dst, cap, &usados, caracter);
    }

    ++*contador;
}

void emitir_cuadruplo(char *dst, int cap, const char *op,
                     const char *a1, const char *a2, const char *res) {
    if (dst == NULL || cap <= 0) return;

    const char *campos[] = {op, a1, a2, res};
    int usados = 0;

    dst[0] = '\0';

    agregar(dst, cap, &usados, "(");

    for (int i = 0; i < 4; ++i) {
        if (i > 0) {
            agregar(dst, cap, &usados, ", ");
        }

        const char *texto = campos[i];

        if (texto == NULL) {
            texto = "_";
        }

        agregar(dst, cap, &usados, texto);
    }

    agregar(dst, cap, &usados, ")");
}
```

Sea `d` la cantidad de dígitos del contador y `L` la longitud total del cuádruplo completo.

`nuevo_temporal` tiene tiempo `O(d)` en ambas versiones. Como el contador es int, la cantidad de dígitos está acotada para una plataforma determinada.

`emitir_cuadruplo` tiene tiempo `O(L)` en la versión Artesano, considerando el procesamiento de las cadenas por `snprintf`. La versión de IA copia como máximo `cap - 1` caracteres, con tiempo `O(min(L, cap))`.

Ambas implementaciones utilizan espacio auxiliar `O(1)` para los tipos de la plataforma. La versión de IA utiliza un arreglo local de tamaño fijo para invertir los dígitos.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

*Pruebas ejecutadas:* Expresiones aritméticas compuestas

Ambas implementaciones se compilaron con MSVC en modo C11 y las opciones `/W4 /WX`. Las dos superaron los seis casos del harness original y terminaron con código de salida cero.

También se ejecutaron pruebas complementarias del cambio de `_t9` a `_t10`, actualización del contador, cuatro campos NULL, campos vacíos, buffers pequeños, capacidades cero y uno, destino NULL, contador NULL, contador negativo y contador igual a `INT_MAX`. Se comprobó además un byte centinela fuera de la capacidad declarada del buffer. Todas estas pruebas terminaron correctamente.

Queda pendiente repetir la compilación y ejecución con GCC en el entorno del laboratorio y comprobar exhaustivamente las capacidades próximas a cada longitud de salida.

### Salida del harness

Salida obtenida en ambas implementaciones, presentada hasta el total. El programa imprime después su mensaje de cierre.

```text
=== AUDITORIA SESION 21 - Cuadruplos y temporales unicos ===
Caso                               esperado:              obtenido:              resultado
[ 1] nuevo_temporal(_t0)                esperado: _t0          obtenido: _t0          OK
[ 2] nuevo_temporal(_t7)                esperado: _t7          obtenido: _t7          OK
[ 3] cuadruplo de a + b
     esperado: (+, a, b, _t0)
     obtenido: (+, a, b, _t0)
     -> OK
[ 4] cuadruplo de _t0 * c
     esperado: (*, _t0, c, _t1)
     obtenido: (*, _t0, c, _t1)
     -> OK
[ 5] asignacion simple                  esperado: (=, a, _, x) obtenido: (=, a, _, x) OK
[ 6] goto sin operandos
     esperado: (goto, L1, _, _)
     obtenido: (goto, L1, _, _)
     -> OK

RESULTADO: 6/6 pruebas superadas
```

* [x] Caso base y límites superados sin *Crash* en las pruebas ejecutadas.
* [x] La revisión confirma escrituras acotadas a cap y ausencia de reservas dinámicas. La prueba con centinela confirmó la conservación del byte situado fuera de la capacidad declarada.
* [x] La complejidad teórica coincide con la implementada.

### Reporte Crítico

*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*

> El harness original verifica los textos `_t0` y `_t7`, pero deja sin comprobar el incremento del contador. Una implementación que formatee correctamente y conserve el contador podría superar esos dos casos y generar nombres repetidos en llamadas consecutivas. Se agregó una prueba iniciada en nueve que confirmó `_t9`, contador diez, y después `_t10`, contador once.
>
> También se comprobaron buffers insuficientes. Las dos versiones conservaron un prefijo terminado en nulo sin modificar el centinela externo. El truncamiento puede hacer que distintos temporales produzcan el mismo prefijo, por lo que la unicidad del nombre completo requiere capacidad suficiente.
>
> La versión de IA entregada ya contempla el incremento y los límites de escritura. La auditoría amplió la cobertura del harness sin requerir una corrección del código.

---

**Firma digital:** `j3sus55`
