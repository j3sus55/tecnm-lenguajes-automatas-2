# Decisiones de diseño
## Frontend
Se conserva la CLI del curso. El lexer produce categorías KW/ID/NUM/OP/DELIM/EOF.
Cada token conserva su posición. El parser sigue los niveles de la EBNF:
comparación, suma, producto, unario, primaria. Solo admite un prefijo unario y
una comparación por nivel, como indica la gramática.
Los errores se recuperan hasta punto y coma o cierre de bloque; si la sentencia
ya consumió su delimitador no se descarta la siguiente.

El AST usa nodos con hijos y lista enlazada de sentencias. Los tokens son dueños
de sus textos; el AST los referencia hasta que termina la compilación.
`--ast` verifica sintaxis, no nombres, porque la prueba oficial de asignación
contiene una variable sin declaración.

## Semántica y flujo
Cada declaración recibe un identificador interno único. La búsqueda recorre los
símbolos activos desde el más reciente; cerrar un bloque desactiva sus símbolos.
La variable recién declarada existe en su inicializador, pero sigue sin valor:
`int x=x;` produce E01, incluso con un x exterior.
Al salir de if se intersectan los estados de inicialización de ambas ramas.
Sin else se conserva también el camino que no ejecuta el bloque.
Al salir de while se restaura el estado previo: el cuerpo podría ejecutarse cero
veces. Es un análisis conservador sin evaluación de condiciones para inicialización.
E04 detecta divisores que son expresiones constantes de valor cero.

## IR y optimizaciones
Los operandos distinguen inmediatos, símbolos y temporales mediante IDs; aunque
una variable se llame _t0, no colisiona internamente con el temporal.
En la impresión TAC se conservan los nombres originales como exige el contrato;
el backend usa IDs distintos para cada declaración, incluido shadowing.
El plegado evalúa subárboles constantes antes de emitir instrucciones.
Una pasada hasta punto fijo elimina definiciones de temporales sin consumidores.
No se eliminan asignaciones de variables por suposición local: pueden ser leídas
después de un salto. No se propagan valores de variables a través de bucles.
Los temporales no se reutilizan.

## Backend y ejecución
Cada símbolo/temporal ocupa cuatro bytes en la pila. El marco se redondea a
múltiplos de 16 para respetar System V antes de llamar printf.
Se usan eax/ecx como registros de trabajo, esi/rdi para printf y eax=0 para
la llamada variádica. Comparaciones con signo devuelven 0/1.
El ensamblador usa direccionamiento relativo RIP y sección GNU-stack sin ejecución.
La división extiende a 64 bits para tratar INT_MIN/-1 sin excepción de idivl.
Suma, resta, multiplicación y negación usan aritmética modular de 32 bits;
el plegado usa operaciones unsigned para evitar overflow indefinido de C.

El entorno de ejecución es Linux x86-64. mkdtemp crea un directorio privado.
fork/execvp pasan argumentos sin shell; waitpid recupera el resultado.
Se ejecutan as, gcc y finalmente el ejecutable; después se borran solo sus
archivos temporales. POSIX se usa para el proceso de ejecución, sin dependencias
de terceros. El núcleo de análisis y generación usa libc.
