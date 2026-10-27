# Bitácora de Requerimientos y Auditoría IA — Sesión 02

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Gestión en GitHub Web y Nodos Dinámicos  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Mnuel Rangel Aviles

**Matrícula:** 24531356  
**Track:** C

---

# 1. Fase 1: El Artesano (Dificultad Manual)

**Reto:** Lista enlazada de tokens en Heap.

**Describe qué problema o restricción de memoria/sintaxis identificaste al codificar a mano:**

> Durante la implementación manual se identificó que cada nodo debía reservar memoria dinámica mediante `malloc`, además de copiar correctamente la cadena del token para evitar que el puntero apuntara a memoria temporal. También fue necesario liberar todos los nodos con `free` para prevenir fugas de memoria y evitar errores como *Use-After-Free* o desreferenciar punteros nulos.

---

# 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

## 2.1 Matriz de Especificación Formal

| **Elemento C-R-E-O** | **Definición Técnica** |
|-----------------------|-------------------------|
| **Contexto (C)** | C99 / GCC sin bibliotecas no estándar |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores |
| **Especificación (E)** | Gestión de memoria dinámica mediante listas enlazadas |
| **Output (O)** | Código modular y análisis de complejidad |

## 2.2 Prompt Estructurado

```text
Actúa como un ingeniero de software especializado en sistemas y compiladores.

Genera un programa en C99 compatible con GCC que implemente una lista enlazada dinámica para almacenar tokens en el Heap.

Requisitos:
- Utilizar malloc y free correctamente.
- Copiar cada cadena de forma segura.
- Implementar funciones crearNodo, insertarFinal, imprimirLista y liberarLista.
- Validar punteros nulos.
- Evitar Memory Leaks y Use-After-Free.
- Incluir análisis de complejidad temporal y espacial.
```

---

# 3. Código Devuelto por el Modelo

```c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct Nodo {
    char *token;
    struct Nodo *siguiente;
} Nodo;

Nodo *crearNodo(const char *texto) {
    if (texto == NULL) return NULL;

    Nodo *nuevo = (Nodo *)malloc(sizeof(Nodo));
    if (nuevo == NULL) return NULL;

    nuevo->token = (char *)malloc(strlen(texto) + 1);
    if (nuevo->token == NULL) {
        free(nuevo);
        return NULL;
    }

    strcpy(nuevo->token, texto);
    nuevo->siguiente = NULL;
    return nuevo;
}

void insertarFinal(Nodo **cabeza, const char *texto) {
    if (cabeza == NULL) return;

    Nodo *nuevo = crearNodo(texto);
    if (nuevo == NULL) return;

    if (*cabeza == NULL) {
        *cabeza = nuevo;
        return;
    }

    Nodo *actual = *cabeza;
    while (actual->siguiente != NULL) {
        actual = actual->siguiente;
    }

    actual->siguiente = nuevo;
}

void imprimirLista(Nodo *cabeza) {
    while (cabeza != NULL) {
        printf("%s -> ", cabeza->token);
        cabeza = cabeza->siguiente;
    }
    printf("NULL\n");
}

void liberarLista(Nodo **cabeza) {
    if (cabeza == NULL) return;

    Nodo *actual = *cabeza;

    while (actual != NULL) {
        Nodo *temp = actual;
        actual = actual->siguiente;
        free(temp->token);
        free(temp);
    }

    *cabeza = NULL;
}

int main() {
    Nodo *lista = NULL;

    insertarFinal(&lista, "int");
    insertarFinal(&lista, "main");
    insertarFinal(&lista, "(");
    insertarFinal(&lista, ")");

    printf("Lista de tokens:\n");
    imprimirLista(lista);

    liberarLista(&lista);

    if (lista == NULL) {
        printf("Memoria liberada correctamente.\n");
    }

    return 0;
}
```

## Análisis de Complejidad

| Operación | Complejidad |
|-----------|------------|
| Crear nodo | O(1) |
| Insertar al final | O(n) |
| Imprimir lista | O(n) |
| Liberar lista | O(n) |

**Complejidad espacial:** O(n), donde *n* es el número de nodos almacenados.

---

# 4. Fase 3: El Auditor (Resultados del Test Harness)

**Pruebas ejecutadas:** Prevención de Use-After-Free y cadenas nulas.

- [x] Caso base superado.
- [x] Inserción de múltiples nodos.
- [x] Liberación completa de memoria.
- [x] Sin accesos fuera de límites.
- [x] Sin Memory Leaks.
- [x] Complejidad implementada consistente con la teórica.

## Reporte Crítico

**¿Qué caso borde o problema omitió la IA y cómo lo solucionaste?**

> Un caso borde importante fue la posibilidad de recibir una cadena nula (`NULL`) al crear un nodo. Si no se valida ese parámetro, `strlen(NULL)` provocaría un fallo del programa. La solución consistió en verificar que el puntero recibido sea válido antes de reservar memoria. Además, después de liberar la lista se asignó la cabeza a `NULL` para evitar posibles accesos posteriores a memoria ya liberada (*Use-After-Free*).
