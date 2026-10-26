# Bitácora de Requerimientos y Auditoría IA — Sesión 01

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Encuadre, Diagnóstico de Punteros y Metodología IA  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles

**Matrícula:** 24531356 
**Track:** C

---

## 1. Fase 1: El Artesano (Dificultad Manual)

**Reto:** Inversión de buffer in-place (`char*`).

La principal dificultad al programar manualmente fue utilizar correctamente los punteros para recorrer la cadena e intercambiar los caracteres desde los extremos hacia el centro. También fue necesario cuidar los límites de memoria, conservar el terminador nulo `'\0'` y evitar crear un segundo arreglo, ya que el objetivo era realizar la inversión en el mismo buffer.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
|---|---|
| **Contexto (C)** | C99 con GCC, sin bibliotecas no estándar. |
| **Rol (R)** | Ingeniero de software especializado en sistemas y compiladores. |
| **Especificación (E)** | Inversión de cadenas mediante punteros, tiempo O(n) y memoria auxiliar O(1). |
| **Output (O)** | Código modular, pruebas y análisis de complejidad. |

### 2.2 Prompt Estructurado

Actúa como un ingeniero de software especializado en sistemas y compiladores. Desarrolla un programa en C99 compatible con GCC que invierta una cadena modificable en el mismo buffer, utilizando punteros para intercambiar los caracteres desde ambos extremos hacia el centro.

El programa debe cumplir los siguientes requisitos:
1. Implementar una función modular que reciba un puntero `char *`.
2. Validar si el puntero es NULL.
3. Manejar cadenas vacías y longitudes pares e impares.
4. No utilizar memoria dinámica ni crear un segundo buffer.
5. Conservar el terminador nulo `'\0'`.
6. Garantizar complejidad temporal O(n) y espacial auxiliar O(1).
7. Incluir pruebas para verificar los casos normales y los casos límite.
8. Entregar código completo y un análisis breve de complejidad.

---

## 3. Código Devuelto por el Modelo

```c
#include <stdio.h>

int invertir_buffer(char *cadena)
{
    char *izquierda;
    char *derecha;
    char temporal;

    if (cadena == NULL) {
        return 0;
    }

    izquierda = cadena;
    derecha = cadena;

    /* Buscar el terminador de la cadena */
    while (*derecha != '\0') {
        derecha++;
    }

    /* Si esta vacia, no necesita cambios */
    if (derecha == izquierda) {
        return 1;
    }

    /* Posicionarse en el ultimo caracter */
    derecha--;

    /* Intercambiar caracteres hasta llegar al centro */
    while (izquierda < derecha) {
        temporal = *izquierda;
        *izquierda = *derecha;
        *derecha = temporal;

        izquierda++;
        derecha--;
    }

    return 1;
}

int main(void)
{
    char texto1[] = "Hola";
    char texto2[] = "ABCDE";
    char texto3[] = "";
    char texto4[] = "A";
    char texto5[] = "AB";

    printf("=== PRUEBAS DE INVERSION ===\n\n");

    invertir_buffer(texto1);
    printf("Hola -> %s\n", texto1);

    invertir_buffer(texto2);
    printf("ABCDE -> %s\n", texto2);

    invertir_buffer(texto3);
    printf("Cadena vacia -> \"%s\"\n", texto3);

    invertir_buffer(texto4);
    printf("A -> %s\n", texto4);

    invertir_buffer(texto5);
    printf("AB -> %s\n", texto5);

    if (invertir_buffer(NULL) == 0) {
        printf("NULL -> Detectado correctamente\n");
    }

    printf("\nComplejidad temporal: O(n)\n");
    printf("Memoria auxiliar: O(1)\n");

    return 0;
}
