# Entrega — Hito 3: TAC y optimizaciones
**Equipo:** Jesus Manuel Ramgel Aviles y Rafael Antonio Castillo Basto.
**Repositorio:** https://github.com/j3sus55/tecnm-lenguajes-automatas-2
**Fecha de preparación:** 2026-11-05.
**Estado:** implementación y pruebas listas; revisión humana y demo pendientes.
**Tag previsto:** v0.3-hito3, todavía no publicado.

## Qué funciona
`make` y `make test-hito3`. TAC y optimizaciones implementados.
Consultar README y docs/diseno.md para comandos, interfaces y decisiones.

## Qué falta
Revisión de ambos integrantes, exposición y cierre de entrega con tag.
El equipo tiene dos integrantes; la guía exige tres o cuatro.
No se atribuyen revisiones ni commits humanos que no ocurrieron.

## Pruebas
Resultado real en Linux: **Hito 3: 15/15 pruebas superadas**.
Suite completa: 20/20. Casos adicionales: 24/24.
También pasan con AddressSanitizer y UndefinedBehaviorSanitizer.
La evidencia está en docs/validacion.md; las pruebas propias están en
pruebas_propias/run.py sin alterar las doradas.

## Reparto de revisión
Jesus Manuel Ramgel Aviles: lexer, parser, AST y semántica.
Rafael Antonio Castillo Basto: TAC, backend y pruebas.
Ambos: documentación y pipeline completo.
Es una propuesta de revisión pendiente; el código inicial fue generado por Codex.

## Bitácora C-R-E-O de esta sesión
| Contexto | Rol | Especificación | Output | Auditoría |
| --- | --- | --- | --- | --- |
| Completar el proyecto del fork | Asistente de implementación | C11, parser manual, gramática y tests del curso intactos | Módulos y pruebas automatizadas | Aceptación técnica condicionada a ejecución real |
| Validar los cuatro hitos | Auditor automático | Compilar sin warnings, ejecutar pruebas y comprobar ELF | Suites oficiales, propias y sanitizadores | Se verificaron logs exitosos |
| Revisar recuperación de errores | Depurador | Dos declaraciones inválidas deben reportar dos errores | Prueba propia encontró que se omitía la segunda | Se corrigió el parser; se rechazó dejar el primer resultado como entrega |
| Preparar informe | Documentador | Registrar evidencia sin inventar participación ni demos | README, diseño, guion y bitácoras | Revisión y aceptación humana aún pendientes |

No hubo cuatro consultas históricas separadas: los cuatro hitos se implementaron
en una sesión integrada. Esta tabla documenta el uso real de IA.
El equipo debe añadir qué aceptó o rechazó después de leer el código.

## Demo
Guion disponible en docs/demo.md.
- [x] Guion preparado.
- [ ] Ensayada por ambos.
- [ ] Ambos explican cualquier módulo.
- [ ] Presentada al docente.
