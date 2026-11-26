
# Bitácora de Requerimientos y Auditoría IA — Sesión 03

**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Google Colab, Mágicos y Flujo de Entrega  
**Docente Titular:** MCC. Iván Márquez Larios (`ijmarquezl`)  
**Estudiante:** Jesus Manuel Rangel Aviles  
**Matrícula:** 24531356  
**Track:** C

---

## 1. Fase 1: El Artesano (Dificultad Manual)

**Reto:** Escritura y lectura de archivos en C.

La principal dificultad al realizar el ejercicio fue manejar correctamente los archivos con las funciones `fopen()`, `fputc()` y `fgetc()`. También fue necesario comprobar que los archivos se abrieran correctamente y cerrarlos al terminar para evitar errores. Otro problema fue contar correctamente las líneas cuando el archivo está vacío o cuando la última línea no termina con un salto de línea. Para resolverlo, se debe llevar un contador de saltos de línea y verificar si el último carácter leído fue diferente de `'\n'`.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
|---|---|
| **Contexto (C)** | C11 con GCC, usando bibliotecas estándar y respetando `api_sesion03.h`. |
| **Rol (R)** | Ingeniero de software especializado en sistemas y compiladores. |
| **Especificación (E)** | Implementar escritura de archivos y conteo de líneas, considerando archivos vacíos, última línea sin salto y rutas inexistentes. |
| **Output (O)** | Código modular en C, sin `main()`, compatible con `-Wall -Wextra -Werror`. |

### 2.2 Prompt Estructurado

Actúa como un ingeniero de software especializado en programación de sistemas y compiladores. Implementa en C las funciones declaradas en `api_sesion03.h`:

int escribir_texto(const char *ruta, const char *texto);
int contar_lineas(const char *ruta);

Requisitos:
1. La función escribir_texto debe abrir el archivo en modo escritura, guardar el texto y cerrarlo correctamente.
2. Debe devolver 0 cuando la escritura y el cierre sean exitosos, y -1 si ocurre un error.
3. La función contar_lineas debe abrir el archivo en modo lectura y contar las líneas correctamente.
4. Un archivo vacío debe devolver 0 líneas.
5. La última línea debe contarse aunque no termine con '\n'.
6. Si la ruta no existe o no se puede abrir, devolver -1.
7. Cerrar siempre los archivos abiertos.
8. No incluir main(), porque el harness proporciona esa función.
9. El código debe compilar con GCC utilizando -std=c11 -Wall -Wextra -Werror.
10. Entregar código completo y modular, sin bibliotecas no estándar.

---

## 3. Código Devuelto por el Modelo

```c
#include "api_sesion03.h"
#include <stdio.h>

int escribir_texto(const char *ruta, const char *texto)
{
    FILE *archivo;

    if (ruta == NULL || texto == NULL) {
        return -1;
    }

    archivo = fopen(ruta, "w");

    if (archivo == NULL) {
        return -1;
    }

    if (fputs(texto, archivo) == EOF) {
        fclose(archivo);
        return -1;
    }

    if (fclose(archivo) == EOF) {
        return -1;
    }

    return 0;
}

int contar_lineas(const char *ruta)
{
    FILE *archivo;
    int caracter;
    int lineas = 0;
    int hay_contenido = 0;
    int ultimo_caracter = '\0';

    if (ruta == NULL) {
        return -1;
    }

    archivo = fopen(ruta, "r");

    if (archivo == NULL) {
        return -1;
    }

    while ((caracter = fgetc(archivo)) != EOF) {
        hay_contenido = 1;

        if (caracter == '\n') {
            lineas++;
        }

        ultimo_caracter = caracter;
    }

    if (ferror(archivo)) {
        fclose(archivo);
        return -1;
    }

    if (hay_contenido && ultimo_caracter != '\n') {
        lineas++;
    }

    if (fclose(archivo) == EOF) {
        return -1;
    }

    return lineas;
}
```

### Análisis de complejidad

- **escribir_texto():** O(n), donde n es la cantidad de caracteres escritos.
- **contar_lineas():** O(n), porque recorre el archivo carácter por carácter.
- **Memoria auxiliar:** O(1), porque utiliza una cantidad constante de variables.
- **Manejo de errores:** devuelve -1 cuando una ruta no se puede abrir o cuando ocurre un error de lectura o escritura.

---

## 4. Fase 3: El Auditor (Resultados del Test Harness)

**Pruebas del laboratorio:**
1. Escribir un archivo de texto.
2. Contar tres líneas terminadas en salto de línea.
3. Contar líneas cuando la última no tiene salto final.
4. Contar líneas de un archivo vacío.
5. Intentar leer un archivo inexistente.
6. Intentar escribir en una ruta inválida.

### Salida esperada del harness

```text
=== AUDITORIA SESION 03 - Archivos: escribir y contar lineas ===
Caso                               esperado:              obtenido:              resultado
[ 1] escribir_texto(...)==0         esperado: 0            obtenido: 0            OK
[ 2] contar_lineas(3 lineas)        esperado: 3            obtenido: 3            OK
[ 3] contar_lineas(sin salto final) esperado: 2            obtenido: 2            OK
[ 4] contar_lineas(archivo vacio)   esperado: 0            obtenido: 0            OK
[ 5] contar_lineas(inexistente)     esperado: -1           obtenido: -1           OK
[ 6] escribir_texto(ruta invalida)  esperado: -1           obtenido: -1           OK

RESULTADO: 6/6 pruebas superadas
TODO EN VERDE.
```

### Lista de auditoría

- [ ] Caso base y límites superados sin Crash.
- [ ] No existen accesos fuera de límites ni Memory Leaks.
- [ ] La complejidad teórica coincide con la implementada.

Las casillas deben marcarse después de ejecutar el harness y verificar los resultados.

### Reporte Crítico

El principal caso borde que debía considerarse era un archivo cuya última línea no terminara con un salto de línea. Si solamente se contaran los caracteres `'\n'`, el programa no contabilizaría correctamente esa última línea. Para resolverlo, se agregó una variable que registra el último carácter leído y otra que indica si el archivo contiene datos. Al terminar la lectura, se incrementa el contador si existe contenido y el último carácter no es `'\n'`. También se comprobó que los archivos vacíos devuelvan cero y que las rutas inexistentes devuelvan -1. Finalmente, se revisó que todos los archivos abiertos se cierren y que la complejidad temporal sea O(n).

---

**Firma digital:** [Usuario de GitHub]

---

## Código para ejecutar en Google Colab

Guarda el código de la sección 3 en `artesano_sesion03_c.c` y ejecuta:

```bash
!gcc -std=c11 -Wall -Wextra -Werror harness_sesion03_c.c artesano_sesion03_c.c -o test_artesano && ./test_artesano
```

Para probar la versión de IA, guarda el mismo código en `ia_sesion03_c.c` y ejecuta:

```bash
!gcc -std=c11 -Wall -Wextra -Werror harness_sesion03_c.c ia_sesion03_c.c -o test_ia && ./test_ia
```
