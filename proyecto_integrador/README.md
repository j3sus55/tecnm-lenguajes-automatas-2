# Lenguajito — compilador C11
Equipo: **Jesus Manuel Ramgel Aviles** y **Rafael Antonio Castillo Basto**.
Curso: Lenguajes y Autómatas II, TecNM Cancún.

## Ejecutar
Requiere Linux x86-64, GCC, GNU as, Make y Python 3.10 o posterior.
Puede ejecutarse en Google Colab o WSL. En Windows nativo el backend emite
ensamblador ELF, pero `--run` requiere Linux.

```bash
cd proyecto_integrador
make
make test
python3 pruebas_propias/run.py
./lenguajito tests/validos/08_anidado.lj --run
```

En Colab, clona el fork y selecciona la rama de implementación:
```python
!git clone --branch codex/lenguajito-integrador https://github.com/j3sus55/tecnm-lenguajes-automatas-2.git
%cd tecnm-lenguajes-automatas-2/proyecto_integrador
!make
!make test
!python3 pruebas_propias/run.py
```

## Qué funciona
Los seis modos: `--tokens`, `--ast`, `--check` (predeterminado),
`--tac`, `--asm` y `--run`. Gramática base completa, enteros de 32 bits,
ámbitos, shadowing, inicialización definida, diagnósticos múltiples (máximo 20),
plegado de constantes y eliminación de temporales sin usos.
`--run` ejecuta un binario real ensamblado con `as` y enlazado con `gcc`.
No hay intérprete oculto.

## Arquitectura
| Archivo | Responsabilidad |
| --- | --- |
| src/lexer.c | Tokens, comentarios, línea y columna |
| src/parser.c | Parser descendente recursivo y recuperación |
| src/ast.c | Memoria, diagnósticos y representación del árbol |
| src/semantica.c | Símbolos, ámbitos y análisis de inicialización |
| src/tac.c | IR, temporales, etiquetas y optimizaciones |
| src/codegen.c | ABI System V, x86-64 AT&T, as/gcc y ejecución |
| src/compilador.c | Coordinación y liberación de recursos |
| src/main.c | CLI original del curso |
| pruebas_propias/run.py | Casos adicionales separados de las pruebas doradas |

## Pruebas y evidencia
`make test-hito1`, `make test-hito2`, `make test-hito3`,
`make test-hito4` y `make test` mantienen el contrato original.
Las pruebas de `tests/` se copiaron del fork sin alterar su contenido.
Por la regla de no modificar esa carpeta, las pruebas nuevas están en
`pruebas_propias/`.
El workflow `Lenguajito C11` conserva logs, comprueba ELF con `file` y
`readelf` y publica un artefacto Linux con compilador, ensamblador y binario
de ejemplo. También ejecuta AddressSanitizer y UndefinedBehaviorSanitizer.
Consultar [evidencia](docs/validacion.md).

## Equipo y reparto propuesto para revisión y defensa
| Integrante | Archivos y revisión asignada |
| --- | --- |
| Jesus Manuel Ramgel Aviles | lexer.c, parser.c, ast.c, semantica.c; demostrar tokens, AST y errores |
| Rafael Antonio Castillo Basto | tac.c, codegen.c, pruebas_propias/run.py; demostrar TAC, ABI y ELF |
| Ambos | compilador.c, documentación y ejecución completa |

La implementación inicial fue generada con asistencia de Codex en esta sesión.
El reparto anterior es una asignación de revisión, **no una afirmación de autoría
manual ni de revisiones ya realizadas**. Cada integrante debe revisar, explicar
y registrar sus cambios reales con su propia cuenta; no se simulan commits de
personas que no los hicieron.

## Qué falta para la entrega académica
- La guía exige 3–4 integrantes; aquí se registran los dos nombres proporcionados.
  Debe resolverse con el docente.
- Revisión humana del código y bitácoras por ambos integrantes.
- Demos presenciales y avances históricos: se proporciona guion, pero no se
  afirma que hayan ocurrido.
- Los tags académicos deben publicarse sobre la revisión aprobada; consultar
  [entregas](docs/entregas.md).
- No incluye extensiones ajenas a la gramática base.
- No se exige controlar división dinámica entre cero; los tests válidos no la usan.

Diseño: [decisiones](docs/diseno.md). Exposición: [guion](docs/demo.md).
