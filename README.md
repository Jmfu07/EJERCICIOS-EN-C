# Repositorio de Ejercicios y Guías Resueltas en C

Colección organizada de ejercicios, prácticas, parciales y plantillas de validación en **Lenguaje C**, lista para compilar y ejecutar en **Visual Studio Code (MSYS2 / TDM-GCC)** y **Dev-C++**.

Cada guía incluye sus ejercicios resueltos (`.c` y `.exe`) junto con su **PDF original de enunciados** dentro de la misma carpeta.

> **Índice temático rápido:** consulta [`INDICE_MATRICES_ARCHIVOS_PUNTEROS.md`](INDICE_MATRICES_ARCHIVOS_PUNTEROS.md) para ver listados en un solo lugar todos los ejercicios de **Matrices (100)**, **Archivos (14)** y **Punteros (37)**.

---

## 1. Plantillas de Validación y Bloqueo de Teclado (`PLANTILLAS_VALIDACIONES/`)

| Archivo | Descripción |
|---|---|
| `PLANTILLA_VALIDACIONES.c` | **Plantilla maestra unificada (sin `struct`)**: combina el bloqueo de atajos de Windows (`Win+L`, `Ctrl+Alt+Supr`, `Alt+Tab`, `Alt+F4`, `Ctrl+C/V/X`, `F1–F24`, `Esc`, `QuickEdit`) ejecutándose en una **sola consola**, con soporte para símbolos y `AltGr` (`PERMITIR_ALTGR 1`) y restauración automática al cerrar. Incluye un motor unificado tecla por tecla (`_getch`) con límites configurables mediante `#define` (`MAX_ENT_POS`, `MAX_ENT_NEG`, `MAX_ENT`, `MAX_DEC_POS`, `MAX_DEC_NEG`, `MAX_DEC`, `MAX_LETRAS`, `MAX_TEXTO`, `MAX_CUALQ`) donde `-`, `.` y `,` **no cuentan como dígito**, además de validación de rangos y menús instantáneos sin Enter (`leer_opcion_num`, `leer_opcion_letra`, `desea_repetir`). |
| `VALIDACIONDETECLADOSIMPLE.c` | **Bloqueo ligero en modo usuario**: bloquea atajos de teclado (`Alt+Tab`, `Alt+F4`, tecla `Win`, `Ctrl+C/V/X`, `Esc`, `F1–F24`) sin modificar el registro de Windows ni solicitar permisos de administrador. |

---

## 2. Entorno Portable para VS Code (`.vscode/`)

El repositorio se **auto-configura al abrir la carpeta en cualquier PC** con Visual Studio Code:

- **Detección automática (`.vscode/auto_config.ps1` + `.vscode/compilar.cmd`):** busca automáticamente **MSYS2** (`ucrt64`/`mingw64`), **TDM-GCC** (incluido Dev-C++) y `gdb.exe` en este orden:
  1. Carpeta portable local `tools/msys2` o `tools/tdm-gcc` (ideal para llevar compiladores en USB; `tools/` está ignorado en `.gitignore`).
  2. Rutas estándar de Windows (`C:\LENGUAJES\`, `C:\msys64\`, `C:\TDM-GCC-64\`, `Dev-Cpp`).
  3. El `PATH` del sistema.
- **Perfiles F5 en Consola Externa (`.vscode/launch.json`):** abre cualquier `.c` en cualquier subcarpeta, elige el perfil en *Run & Debug* (`Ctrl+Shift+D`) y pulsa **F5**:
  - `Ejecutar (Auto - Consola Externa)` *(MSYS2 si existe, si no TDM-GCC, si no `PATH`)*.
  - `Ejecutar con MSYS2 (Consola Externa)`.
  - `Ejecutar con TDM-GCC (Consola Externa)`.
- **Tarea sin GDB (`.vscode/tasks.json`):** `Ejecutar en Consola Externa (Sin GDB)` para PCs que solo tengan `gcc` instalado.
- **Configuración manual / instalación asistida:** ejecuta `.vscode/auto_config.cmd` si deseas reconfigurar o instalar MSYS2 vía `winget`.

---

## 3. Guías y Carpetas del Repositorio

### Guías Numeradas (`GUIA DE EJERCICIOS 01` a `17`)

| Carpeta | `.c` | PDF / Guía en carpeta | De qué trata |
|---|---:|---|---|
| `GUIA DE EJERCICIOS 01/` | 5 | `ejercicios 1.pdf` | Condicionales y ciclos con menú: elecciones municipales (Pedro/María/Juan y nulos), cuenta corriente BFC con clave (dos variantes), selección de atletas de maratón y cálculo de jornal diario (diurno/nocturno y domingos). |
| `GUIA DE EJERCICIOS 02/` | 10 | `ejercicios 2.pdf` | Selección y cálculos prácticos: área de triángulo, oferta por docenas con descuento y obsequios, IMC, salario con horas extras (+50 %), días transcurridos en 2013, par/impar, clasificación de ángulos, tipos de triángulo, calificaciones (0–10) y días del mes con año bisiesto. |
| `GUIA DE EJERCICIOS 03/` | 12 | `ejercicios 3.pdf` | Elementos básicos de C: variables, tipos de datos, operadores aritméticos y relacionales, cálculo de edad, interés simple/compuesto, área de triángulo rectángulo, perímetro, división/módulo, segundos a minutos, intercambio de variables y conversión de divisas. |
| `GUIA DE EJERCICIOS 04/` | 19 | `ejercicios de matrices.pdf` | Vectores y matrices (N1–N19) con validaciones tecla por tecla: multiplicación y ordenamiento de arreglos, tablero de ajedrez, gestión de notas de 20 alumnos (con archivo), cursos/asignaturas, conteo de primos, diagonales 20×20, pares/impares, vendedores 18×10, reserva de asientos 25×4 y frecuencia de letras. |
| `GUIA DE EJERCICIOS 05/` | 9 | `ejercicios matriecs 2.pdf` | Práctica de arreglos, matrices 2D y cadenas: operaciones numéricas sobre vectores, ordenamiento, tablero de ajedrez, matriz transpuesta, conteo de vocales e inversión de cadenas. |
| `GUIA DE EJERCICIOS 06/` | 55 | `ejercicios que pedira en clases ... algunos.pdf` | Guía extensa para elaborar en clase dividida en 4 bloques: matrices y álgebra matricial (`34`–`57`), programación modular con funciones y menús (`F1`–`F21`), recursividad (`R23`–`R26`) y estructuras de datos (`E27`–`E34`: países, tablas `f(x)` y triángulos). |
| `GUIA DE EJERCICIOS 07/` | 12 | `ejecricios en c avanzados.pdf` | Proyectos avanzados integrando punteros, memoria dinámica, recursividad y matrices: compresión RLE recursiva, rotación de anillos, árbol de decisiones, permutaciones con restricciones, evaluador de expresiones con matriz de tokens, simulación de ecosistema, ortogonalidad, laberinto con backtracking, determinantes, mapa de calor y cifrado zigzag. |
| `GUIA DE EJERCICIOS 08/` | 20 | `Ejercicios de Recursividad.pdf` | 20 ejercicios dedicados a recursividad: factorial, Fibonacci, suma de dígitos, inversión de cadenas, palíndromos, búsqueda binaria, combinaciones binarias, Torres de Hanói, vocales, decimal a binario, MCD, primos, matriz recursiva, multiplicación sin `*`, potencia y evaluación de expresiones. |
| `GUIA DE EJERCICIOS 09/` | 12 | `Ejercicios de Switch y funciones en lenguaje C.pdf` | Programación modular con `switch` y funciones: intervalos, divisores, ternas pitagóricas, serie de Taylor (`e^x`), valor absoluto, promoción de películas, calificaciones, conjetura de Collatz, juego Craps, menú de funciones, MCD recursivo y función de Ackermann. |
| `GUIA DE EJERCICIOS 10/` | 5 | `Ejercicios de punteros con funciones y matrices (5).pdf` | Matrices dinámicas manipuladas exclusivamente con aritmética de punteros y punteros a funciones: suma recursiva de diagonales, búsqueda en matrices, recorrido recursivo de caminos, operaciones elemento a elemento con callbacks y conteo recursivo de primos. |
| `GUIA DE EJERCICIOS 11/` | 20 | `Ejercicios de Estructuras Secuenciales.pdf` | Estructuras secuenciales: fórmulas geométricas (círculo, triángulo, rectángulo, cilindro), sueldos, promedios, conversión de mayúsculas/minúsculas y cadena a entero/largo, redondeo (`ceil`/`floor`), llamadas telefónicas, conversión cm→pies/pulgadas, Celsius→Fahrenheit, recibo de sueldo y desplazamiento de carácter en consola. |
| `GUIA DE EJERCICIOS 12/` | 20 | `Ejercicios Lenguaje C (No resueltos. Prog I).pdf` | Progresión I completa: desde aritmética básica, códigos ASCII, formato numérico vertical y fecha de Pascua (algoritmo Gregoriano completo), hasta menús, ciclos `while`/centinela, estadísticas de empleados, notas ponderadas, banco, elecciones, vectores de alumnos y matriz 4×4 transpuesta. |
| `GUIA DE EJERCICIOS 13/` | 44 | `47 Ejercicio de Lenguaje C (No Resueltos).pdf`, `Ejercicios de Estructuras de Selección y de Repetición (Guia 1).pdf` | 47 ejercicios de selección y repetición: condicionales, series numéricas, primos, tablas, cálculos estadísticos (lluvias, empleados, alumnos, clubes, concursos), triángulos, año bisiesto, menú de 4 operaciones básicas y fecha de Pascua. |
| `GUIA DE EJERCICIOS 14/` | 8 | `Ejercicios Varios de Lenguaje C.pdf` | Estructuras de datos y temas intermedios/avanzados: arreglos con punteros, estructuras con punteros, lectura/escritura/búsqueda dinámica en archivos de texto, recursividad/ordenamiento/búsqueda, memoria dinámica segura, listas enlazadas simples y árboles binarios de búsqueda (ABB). |
| `GUIA DE EJERCICIOS 15 - MATRICES/` | 20 | `Ejercicios de Matrices en Lenguaje C sin Resolver.pdf`, `rrays arreglos multidimensionales en C.pdf` | **Matrices y Arreglos Multidimensionales**: 19 ejercicios de arreglos 1D/2D/3D (versión ligera sin bloqueo Win32) + ejercicio 20 de arreglo tridimensional `6×10×2` de ventas por empleado/oficina/edificio. |
| `GUIA DE EJERCICIOS 16 - ARCHIVOS/` | 10 | `README_GUIA_16_ARCHIVOS.md` | **Manejo completo de Archivos en C**: modos `r`/`w`/`a`, estadísticas de texto, buscar y reemplazar, copiar/mayúsculas/invertir líneas, ordenamiento numérico con reporte, fusión AND/OR/XOR de ficheros, agenda CSV con borrado físico, inventario CRUD **binario** (`fwrite`/`fread`/`fseek`/`ftell`), reporte de notas en tabla y cifrado César. |
| `GUIA DE EJERCICIOS 17 - PUNTEROS/` | 10 | `README_GUIA_17_PUNTEROS.md` | **Punteros y Memoria Dinámica en C**: fundamentos y paso por referencia (`&`, `*`), aritmética de punteros sin `[]`, implementación de cadenas con `char *`, selección + búsqueda binaria devolviendo `int *`, vector redimensionable (`malloc`/`realloc`), matrices dinámicas `int **`, callbacks/punteros a funciones, estructuras dinámicas (`->`), pila/cola con `Nodo **` y lista doblemente enlazada. |

### Otras Carpetas

| Carpeta | `.c` | Contenido |
|---|---:|---|
| `EJERCICIOS SIMPLES DE LA PRIMERA CLASE/` | 5 | Primeros 5 ejercicios básicos de entrada/salida: suma/resta/multiplicación (`long long`), sueldo mensual/quincenal/semanal, promedio de notas, costo de llamada y área/longitud de circunferencia. |
| `EJERCICIOS_DE_CLASE/` | 5 | Prácticas realizadas en clase: `10 NUMEROS REALES.c`, `EJERCICIO DE PUNTEROS 1.c` y `2.c`, `LABORATORIO DE LENGUAJES.c` y `programa de vendedores.c`. |
| `LENGUAJES/` | 1 | `LENGUAJES DE PROGRAMACION ARCHIVOS.c` junto con sus archivos de prueba (`a.txt`, `b.txt`, `c.txt`): operaciones **AND**, **OR** y **XOR** entre dos ficheros de enteros ordenados generando la salida ordenada de mayor a menor. |
| `PARCIALES DE LABORATORIO RAMIREZ/` | 6 | Exámenes parciales originales conservados intactos (`PARCIAL N1/` y `PARCIAL N2/`) junto a `Ramirez Jose Parciales.pdf`. |

---

## Compilación manual por consola

```bash
gcc -Wall -Wextra -static-libgcc archivo.c -o archivo.exe -lm
```
