# Repositorio de Ejercicios y Guías Resueltas en C

Cada ejercicio está separado en su propio archivo `.c` junto con su ejecutable `.exe`, compatible con **Dev-C++ (TDM-GCC)** y **Visual Studio / VS Code (MSYS2/MinGW)**.

## Plantillas de Validación y Bloqueo de Teclado (`PLANTILLAS_VALIDACIONES/`)
- `PLANTILLAS_VALIDACIONES/PLANTILLA_VALIDACIONES.c`: Plantilla maestra unificada (sin `struct`) que combina el bloqueo de atajos de Windows (`Win+L`, `Ctrl+Alt+Supr`, `Alt+Tab`, `Alt+F4`, `Ctrl+C/V/X`, `F1-F24`, `Esc`, `QuickEdit`) en una sola consola, permitiendo símbolos y `AltGr`, junto con las validaciones tecla por tecla (`_getch`) con límites por `#define` y menús instantáneos.
- `PLANTILLAS_VALIDACIONES/VALIDACIONDETECLADOSIMPLE.c`: Versión ligera del bloqueo de teclado en modo usuario (sin tocar registro ni pedir administrador).

---

## Estructura de Carpetas y Guías

| Carpeta | Contenido / Guía correspondiente | Archivos `.c` | PDF en carpeta |
|---|---|---:|---|
| `PLANTILLAS_VALIDACIONES/` | Plantilla maestra y bloqueo simple de teclado | 2 | — |
| `EJERCICIOS SIMPLES DE LA PRIMERA CLASE/` | 5 ejercicios básicos de entrada/salida y cálculos | 5 | — |
| `EJERCICIOS_DE_CLASE/` | Ejercicios sueltos de clase, punteros y vendedores | 5 | — |
| `LENGUAJES/` | Operaciones AND/OR/XOR de archivos (`a.txt`, `b.txt`, `c.txt`) | 1 | — |
| `PARCIALES DE LABORATORIO RAMIREZ/` | `PARCIAL N1/` (2 ejercicios) y `PARCIAL N2/` (4 ejercicios) | 6 | — |
| `GUIA DE EJERCICIOS 1/` | Elecciones, Cuenta corriente BFC, Maratón, Jornal diario | 5 | `ejercicios 1.pdf` |
| `GUIA DE EJERCICIOS 2/` | Ejercicios N1 a N10 (selección y cálculos) | 10 | `ejercicios 2.pdf` |
| `GUIA DE EJERCICIOS 3/` | Elementos Básicos del Lenguaje C (N1 a N12) | 12 | `ejercicios 3.pdf` |
| `GUIA DE EJERCICIOS 4/` | Ejercicios de Matrices (N1 a N19) | 19 | `ejercicios de matrices.pdf` |
| `GUIA DE EJERCICIOS 5/` | Ejercicios de Matrices 2 (9 ejercicios) | 9 | `ejercicios matriecs 2.pdf` |
| `GUIA DE EJERCICIOS 6/` | Ejercicios para elaborar en clase (Matrices, Funciones, Recursividad, Estructuras) | 55 | `ejercicios que pedira en clases ... algunos.pdf` |
| `GUIA DE EJERCICIOS 7/` | Ejercicios avanzados en C (12 ejercicios) | 12 | `ejecricios en c avanzados.pdf` |
| `Guia_Matrices/` | 19 ejercicios de matrices y vectores (`01` a `19`) | 19 | — |
| `Guia_Recursividad/` | Ejercicios de Recursividad (`01` a `20`) | 20 | — |
| `Guia_Switch_Funciones/` | Ejercicios de Switch y funciones (`01` a `12`) | 12 | — |
| `Guia_Punteros_Funciones_Matrices/` | Punteros con funciones y matrices (`01` a `05`) | 5 | — |
| `Guia_Estructuras_Secuenciales/` | Estructuras Secuenciales (`01` a `20`) | 20 | — |
| `Guia_Progresion_I/` | Programación I (`01` a `21`) | 20 | — |
| `Guia_47_Ejercicios_Seleccion_Repeticion/` | Selección y Repetición (`01` a `47`) | 44 | — |
| `Guia_Varios/` | Arreglos, Estructuras, Archivos, Punteros, Listas y Árboles | 8 | — |

---

## Compilación

Con GCC (TDM-GCC en Dev-C++ o MSYS2 en VS Code):

```bash
gcc -Wall -Wextra archivo.c -o archivo.exe
```
