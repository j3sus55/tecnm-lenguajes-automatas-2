# Bitácora de Requerimientos y Auditoría IA — Sesión 03
**Materia:** Lenguajes y Autómatas II (SCD-1016)  
**Tema:** Google Colab, Mágicos y Flujo de Entrega  
**Docente Titular:** MCC. Iván Márquez Larios (ijmarquezl)  
**Estudiante:** Jesus Manuel Rangel Aviles

**Matrícula:** 24531356

**Track:** C  

---

## 1. Fase 1: El Artesano (Dificultad Manual)
**Reto:** Manipulación de archivos y llamadas OS  

**Descripción del problema y restricciones identificadas:**
Durante el codificado manual en C (C99), se identificaron severas restricciones asociadas con la manipulación del sistema de archivos y el manejo de memoria dinámica:
* **Manejo de Buffer y Desbordamientos:** En llamadas al sistema como `read()` o funciones de lectura de archivos como `fgets()` / `fread()`, la falta de verificación estricta de límites (`boundary check`) puede generar buffer overflows o escrituras fuera de rango.
* **Fugas de Memoria (Memory Leaks):** El ciclo de vida de los punteros devueltos por `malloc()` o `calloc()` debe ser gestionado explícitamente. Cualquier retorno temprano o interrupción del flujo antes de invocar `free()` ocasiona fugas de memoria.
* **Control de Archivos y POSIX Call-sites:** La manipulación de descriptores de archivo (`open`, `read`, `close`) exige la comprobación constante de valores de retorno devueltos por el kernel para prevenir lecturas nulas o accesos colgados en punteros `FILE*`.
* **Sintaxis estricta C99:** La necesidad de declarar punteros explicitados y evitar bibliotecas no estándar dificulta la portabilidad rápida y obliga a construir abstracciones personalizadas para la gestión de cadenas y arreglos dinámicos.

---

## 2. Fase 2: El Científico (Ingeniería de Requerimientos C-R-E-O)

### 2.1 Matriz de Especificación Formal

| Elemento C-R-E-O | Definición Técnica |
| :--- | :--- |
| **Contexto (C)** | Entorno de compilación C99 / GCC sin bibliotecas no estándar. Manejo explícito de llamadas del sistema/archivos POSIX. |
| **Rol (R)** | Ingeniero de software de sistemas y compiladores especializado en optimización C99 y bajo nivel. |
| **Especificación (E)** | Función/programa modular para lectura, procesamiento y auditoría de archivos de texto. Manejo riguroso de errores, códigos de retorno ABI (0 para éxito, `EXIT_FAILURE` para errores) y limpieza de recursos. |
| **Output (O)** | Únicamente código modular en C (C99) completamente ejecutable, autodocumentado con análisis de complejidad $O(n)$ en tiempo y $O(1)$ en memoria auxiliar. |

### 2.2 Prompt Estructurado

```text
[CONTEXTO]
Estás trabajando en un entorno C99 utilizando el compilador GCC de GNU/Linux. Queda estrictamente prohibido el uso de bibliotecas no estándar externas o código no portable.

[ROL]
Actúa como un Ingeniero de Software de Sistemas y Compiladores experto en C de bajo nivel, manipulación de archivos POSIX y análisis de complejidad de algoritmos.

[ESPECIFICACIÓN]
Escribe un programa modular en C99 que:
1. Abra y lea un archivo de entrada pasado como argumento o buffer de prueba.
2. Procese el contenido línea por línea o bloque por bloque asegurando una gestión de memoria estricta (sin accesos fuera de límites ni fugas de memoria).
3. Gestione los códigos de retorno ABI del sistema operativo (0 en éxito, distinto de 0 en error).
4. Libere todos los descriptores de archivo y punteros dinámicos antes de terminar.

[OUTPUT]
Proporciona únicamente el código C modular compilable, seguido de un breve comentario indicando la complejidad temporal y espacial.
