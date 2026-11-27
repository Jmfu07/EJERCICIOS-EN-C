# Ejercicios en C

Repositorio donde voy guardando todas las guías, prácticas de clase, parciales y plantillas de validación que he hecho en **C** (Lenguaje de Programación / Laboratorio).

**Contador actual:** `308` ejercicios resueltos (`310` archivos `.c` contando las 2 plantillas de validación).  
*(Nota: cada vez que agregue un ejercicio nuevo, actualizar este contador y la fila de su carpeta).*

Si buscas directamente ejercicios de **matrices**, **archivos** o **punteros**, aquí tienes el listado agrupado por tema: [INDICE_MATRICES_ARCHIVOS_PUNTEROS.md](INDICE_MATRICES_ARCHIVOS_PUNTEROS.md).

---

## Cómo compilar y ejecutar en VS Code

La carpeta `.vscode/` está configurada para que funcione en cualquier PC sin andar cambiando rutas a mano:

- Al abrir la carpeta en VS Code se ejecuta `.vscode/auto_config.ps1`, que detecta dónde están **MSYS2**, **TDM-GCC** (o Dev-C++) y `gdb.exe`.
- Si llevo el repo en un pendrive, puedo meter los compiladores en `tools/msys2` o `tools/tdm-gcc` (esa carpeta está en `.gitignore` para no subirla a GitHub).
- Para correr cualquier `.c` en su propia carpeta, abro el archivo y le doy a **F5** (siempre abre en **consola externa**). En el selector de *Run & Debug* (`Ctrl+Shift+D`) se puede alternar entre:
  - `Ejecutar (Auto - Consola Externa)`
  - `Ejecutar con MSYS2 (Consola Externa)`
  - `Ejecutar con TDM-GCC (Consola Externa)`
- Si en alguna PC solo hay `gcc` sin `gdb`, también está la tarea `Ejecutar en Consola Externa (Sin GDB)`.
- Si hace falta compilar a mano desde consola:

```bash
gcc -Wall -Wextra -static-libgcc archivo.c -o archivo.exe -lm
```

---

## Plantillas de validación (`PLANTILLAS_VALIDACIONES/`)

- **`PLANTILLA_VALIDACIONES.c`**: plantilla base que uso cuando necesito bloquear atajos de Windows (`Win+L`, `Ctrl+Alt+Supr`, `Alt+Tab`, `Alt+F4`, `Ctrl+C/V/X`, `F1-F24`, `Esc`, `QuickEdit`) en la misma consola, dejando activos los símbolos y `AltGr`, junto con lectura tecla por tecla (`_getch`) con límites por `#define` donde `-`, `.` y `,` no cuentan como dígito, validación de rangos y menús sin Enter. Sin `struct`.
- **`VALIDACIONDETECLADOSIMPLE.c`**: versión ligera del bloqueo de teclado sin tocar el registro de Windows ni pedir administrador.

---

## Guías y carpetas

| Carpeta | Ejercicios | PDF / referencia | Contenido |
|---|---:|---|---|
| `GUIA DE EJERCICIOS 01/` | 5 | `ejercicios 1.pdf` | Elecciones (Pedro, María y Juan), cuenta corriente BFC (dos versiones), clasificación de maratón y jornal diario. |
| `GUIA DE EJERCICIOS 02/` | 10 | `ejercicios 2.pdf` | Área de triángulo, venta al mayor por docenas, IMC, salario con horas extras, días transcurridos en 2013, par/impar, ángulos, tipos de triángulo, notas (0–10) y año bisiesto. |
| `GUIA DE EJERCICIOS 03/` | 12 | `ejercicios 3.pdf` | Variables, tipos de datos, operadores, cálculo de edad, interés simple/compuesto, triángulo rectángulo, perímetro, módulo, segundos a minutos, intercambio y cambio de divisas. |
| `GUIA DE EJERCICIOS 04/` | 19 | `ejercicios de matrices.pdf` | 19 ejercicios de vectores y matrices (ajedrez, notas de 20 alumnos con archivo, cursos, primos, diagonales 20×20, pares/impares, vendedores 18×10, asientos de cine 25×4, conteo de letras). |
| `GUIA DE EJERCICIOS 05/` | 9 | `ejercicios matriecs 2.pdf` | Operaciones con arreglos, ordenamiento, tablero de ajedrez, matriz 10×10, transpuesta 3×3, invertir cadena y contar vocales. |
| `GUIA DE EJERCICIOS 06/` | 55 | `ejercicios que pedira en clases ... algunos.pdf` | Guía larga de clase: matrices (`34`–`57`), funciones con menús (`F1`–`F21`), recursividad (`R23`–`R26`) y estructuras (`E27`–`E34`). |
| `GUIA DE EJERCICIOS 07/` | 12 | `ejecricios en c avanzados.pdf` | Ejercicios avanzados con punteros, recursividad y matrices dinámicas (RLE, rotación por anillos, árbol de decisiones, permutaciones, evaluador de expresiones, ecosistema, laberinto con backtracking, determinantes, mapa de calor y cifrado zigzag). |
| `GUIA DE EJERCICIOS 08/` | 20 | `Ejercicios de Recursividad.pdf` | Factorial, Fibonacci, suma de dígitos, invertir cadena, palíndromos, búsqueda binaria, combinaciones binarias, Torres de Hanói, vocales, binario, MCD, primos, matriz, multiplicación sin `*` y potencia. |
| `GUIA DE EJERCICIOS 09/` | 12 | `Ejercicios de Switch y funciones en lenguaje C.pdf` | Intervalos, divisores, ternas pitagóricas, serie de `e^x`, valor absoluto, promoción de películas, calificaciones, Collatz, Craps, menú de funciones, MCD recursivo y Ackermann. |
| `GUIA DE EJERCICIOS 10/` | 5 | `Ejercicios de punteros con funciones y matrices (5).pdf` | Matrices dinámicas usando solo punteros y punteros a funciones (diagonales, búsqueda, caminos recursivos, callbacks y conteo de primos). |
| `GUIA DE EJERCICIOS 11/` | 20 | `Ejercicios de Estructuras Secuenciales.pdf` | Fórmulas geométricas, sueldos, promedios, conversiones de texto y números, `ceil`/`floor`, llamadas, cm a pies/pulgadas, Celsius a Fahrenheit, recibo y mover carácter en consola. |
| `GUIA DE EJERCICIOS 12/` | 20 | `Ejercicios Lenguaje C (No resueltos. Prog I).pdf` | Progresión I: operaciones básicas, ASCII, multiplicación vertical, fecha de Pascua, menús, ciclos con centinela, empleados, banco, elecciones, alumnos y matriz 4×4 transpuesta. |
| `GUIA DE EJERCICIOS 13/` | 44 | `47 Ejercicio de Lenguaje C (No Resueltos).pdf`, `Ejercicios de Estructuras de Selección y de Repetición (Guia 1).pdf` | Condicionales, ciclos, series, primos, lluvias, empleados, alumnos, clubes, triángulos, año bisiesto, calculadora y fecha de Pascua. |
| `GUIA DE EJERCICIOS 14/` | 8 | `Ejercicios Varios de Lenguaje C.pdf` | Arreglos, estructuras con punteros, archivos de texto, recursividad/ordenamiento, memoria dinámica, lista enlazada simple y árbol binario de búsqueda. |
| `GUIA DE EJERCICIOS 15 - MATRICES/` | 20 | `Ejercicios de Matrices en Lenguaje C sin Resolver.pdf`, `rrays arreglos multidimensionales en C.pdf` | Versión ligera (sin bloqueo Win32) de los 19 ejercicios de matrices + ejercicio 20 de arreglo 3D (`6×10×2`). |
| `GUIA DE EJERCICIOS 16 - ARCHIVOS/` | 10 | `README_GUIA_16_ARCHIVOS.md` | Archivos de texto y binarios: modos `r`/`w`/`a`, estadísticas, buscar/reemplazar, copiar/mayúsculas/invertir, ordenar números, AND/OR/XOR, agenda CSV, inventario binario CRUD (`fseek`), reporte de notas y cifrado César. |
| `GUIA DE EJERCICIOS 17 - PUNTEROS/` | 10 | `README_GUIA_17_PUNTEROS.md` | Paso por referencia, aritmética de punteros sin `[]`, cadenas con `char *`, selección + búsqueda binaria devolviendo `int *`, vector con `malloc`/`realloc`, matriz `int **`, punteros a función, estructuras con `->`, pila/cola y lista doblemente enlazada. |
| `EJERCICIOS SIMPLES DE LA PRIMERA CLASE/` | 5 | — | Primeros 5 ejercicios básicos (suma/resta/multiplicación, sueldos, promedio, llamada y circunferencia). |
| `EJERCICIOS_DE_CLASE/` | 5 | — | Ejercicios sueltos de clase (`10 NUMEROS REALES`, `EJERCICIO DE PUNTEROS 1` y `2`, `LABORATORIO DE LENGUAJES` y `programa de vendedores`). |
| `LENGUAJES/` | 1 | `a.txt`, `b.txt`, `c.txt` | Operaciones AND, OR y XOR entre ficheros de enteros ordenados. |
| `PARCIALES DE LABORATORIO RAMIREZ/` | 6 | `Ramirez Jose Parciales.pdf` | Parciales originales de laboratorio (`PARCIAL N1` y `PARCIAL N2`), sin modificar. |
| **Subtotal de ejercicios** | **308** | | |
| `PLANTILLAS_VALIDACIONES/` | 2 | — | Plantilla maestra y versión simple de bloqueo de teclado. |
| **Total de archivos `.c`** | **310** | | |
