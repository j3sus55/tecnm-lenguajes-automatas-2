# Bitácora de Requerimientos y Auditoría IA — Sesión 11
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Arquitectura de la Tabla de Símbolos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356
**Track:** C (el harness evalúa C; la versión en Rust es opcional y no se califica con este harness)  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
*Reto:* Tabla Hash con encadenamiento  
*Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:*
> Esta implementación de referencia asistida por IA busca duplicados en todas las cubetas y enlaza el símbolo al final de su cadena. La versión Científico busca los duplicados de la misma forma y enlaza el símbolo al principio de su cadena.

```c
#include "api_sesion11.h"
#include <stdlib.h>
#include <string.h>

unsigned int hash_djb2(const char *clave) {
    unsigned int hash = 5381u;
    if (clave != NULL) {
        for (; *clave != '\0'; ++clave) {
            hash = (hash << 5) + hash + (unsigned char)*clave;
        }
    }
    return hash;
}

int insertar_simbolo(Simbolo *tabla[], unsigned int cubetas,
                     const char *nombre, int tipo, int offset) {
    if (tabla == NULL || cubetas == 0 || nombre == NULL) return -1;

    Simbolo datos = {0};
    size_t longitud = strlen(nombre);
    if (longitud >= sizeof datos.nombre) return -1;

    unsigned int indice = hash_djb2(nombre) % cubetas;
    Simbolo **destino = &tabla[indice];
    for (unsigned int i = 0; i < cubetas; ++i) {
        Simbolo **enlace = &tabla[i];
        while (*enlace != NULL) {
            if (strcmp((*enlace)->nombre, nombre) == 0) return 0;
            enlace = &(*enlace)->sig;
        }
        if (i == indice) destino = enlace;
    }

    Simbolo *nuevo = malloc(sizeof *nuevo);
    if (nuevo == NULL) return -1;
    memcpy(datos.nombre, nombre, longitud + 1);
    datos.tipo = tipo;
    datos.offset = offset;
    *nuevo = datos;
    *destino = nuevo;
    return 1;
}
```

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal
| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | C11 con biblioteca estándar y el contrato de `api_sesion11.h`. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores. |
| **Especificación (E)** | Función de dispersión djb2 y manejo de colisiones por encadenamiento. Devolver `1` al insertar, `0` si el nombre ya existe y `-1` ante argumentos inválidos o si falla la reserva de memoria. |
| **Output (O)** | Solo código modular y análisis de complejidad. |

### 2.2 Prompt Estructurado
```text
Contexto: laboratorio de C11 sobre una tabla de símbolos definida en api_sesion11.h.
Rol: ingeniero de software de sistemas y compiladores.
Especificación: conserva las firmas de hash_djb2 e insertar_simbolo.
Inicializa djb2 en 5381 y aplica hash = hash * 33 + byte por cada byte.
Calcula la cubeta con hash % cubetas y revisa todas las cadenas para
detectar duplicados antes de reservar memoria. Conserva el tipo y el offset del
símbolo existente si el nombre se repite. Enlaza cada símbolo nuevo con
la cadena anterior. Devuelve 1 al insertar, 0 ante un duplicado y -1 con
tabla nula, nombre nulo, cero cubetas, nombre que no cabe o fallo de malloc.
Output: entrega código C sin main() y explica la complejidad del hash y
del recorrido de la cadena.
```

---

## 3. Código Devuelto por el Modelo
```c
#include "api_sesion11.h"

#include <stdlib.h>
#include <string.h>

unsigned int hash_djb2(const char *clave) {
    unsigned int hash = 5381;
    const unsigned char *byte = (const unsigned char *)clave;

    while (*byte != 0) {
        hash = hash * 33u + *byte;
        ++byte;
    }
    return hash;
}

int insertar_simbolo(Simbolo *tabla[], unsigned int cubetas,
                     const char *nombre, int tipo, int offset) {
    if (tabla == NULL || cubetas == 0 || nombre == NULL) {
        return -1;
    }

    size_t longitud = strlen(nombre);
    if (longitud >= sizeof tabla[0]->nombre) {
        return -1;
    }

    unsigned int indice = hash_djb2(nombre) % cubetas;
    for (unsigned int i = 0; i < cubetas; ++i) {
        for (const Simbolo *actual = tabla[i]; actual != NULL; actual = actual->sig) {
            if (strcmp(actual->nombre, nombre) == 0) return 0;
        }
    }

    Simbolo *nuevo = malloc(sizeof *nuevo);
    if (nuevo == NULL) {
        return -1;
    }

    memcpy(nuevo->nombre, nombre, longitud + 1);
    nuevo->tipo = tipo;
    nuevo->offset = offset;
    nuevo->sig = tabla[indice];
    tabla[indice] = nuevo;
    return 1;
}
```

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)
*Pruebas ejecutadas:* Manejo de claves duplicadas y colisiones, hash de la clave vacía y de `abc`, conservación del offset y rechazo de una tabla nula. El harness original se compiló con MSVC en modo C11 (`/W4 /WX`). Se compilaron dos implementaciones distintas por separado. Ambas pasaron 6 de 6 y produjeron la salida de abajo.

### Salida del harness
```text
=== AUDITORIA SESION 11 - Tabla hash con encadenamiento ===
Caso                               esperado:              obtenido:              resultado
[ 1] hash_djb2("")                      esperado: 5381         obtenido: 5381         OK
[ 2] hash_djb2("abc")                   esperado: 193485963    obtenido: 193485963    OK
[ 3] insertar x dos veces               esperado: 0            obtenido: 0            OK
[ 4] dos claves, una cubeta             esperado: 2            obtenido: 2            OK
[ 5] offset de beta en cadena           esperado: 4            obtenido: 4            OK
[ 6] insertar en tabla NULL             esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE. Documenta tu auditoria en auditorias/auditoria_sesion11.md
```

* [x] Caso base y límites superados sin *Crash*.
* [ ] No existen accesos fuera de límites ni *Memory Leaks*. El harness original deja nodos sin liberar y no se ejecutó un analizador de memoria.
* [x] La complejidad teórica coincide con la implementada. El costo es `O(L + B + N)`, donde `L` es la longitud del nombre, `B` es el número de cubetas y `N` el número total de símbolos. El espacio auxiliar es constante, aparte del nodo que se inserta.

### Reporte Crítico
*¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?*
> El caso que faltó en el harness original fue un nombre demasiado largo para `nombre[16]`. La propuesta de IA ya incluía una comprobación de longitud antes de reservar memoria y copiar el nombre. En la prueba adicional, `abcdefghijklmno` se insertó y `abcdefghijklmnoA` devolvió `-1`. También se comprobó que reinsertar el nombre original devuelve `0` y conserva su tipo y offset.

---
**Firma digital:** `j3sus55`
