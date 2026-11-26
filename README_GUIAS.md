# Repositorio de Ejercicios y Guias Resueltas en C

Cada ejercicio está separado en su propio archivo `.c` junto con su ejecutable `.exe`, compatible con **Dev-C++ (TDM-GCC)** y **Visual Studio / VS Code (MSYS2/MinGW)**.

## Plantillas Maestras de Validación y Bloqueo de Teclado
- `VALIDACIONESPORTECLADO.c`: Plantilla maestra unificada que combina el bloqueo de atajos de Windows (`Win+L`, `Ctrl+Alt+Supr`, `Alt+Tab`, `Alt+F4`, `Ctrl+C/V/X`, `F1-F24`, `Esc`, `QuickEdit`) sin falsos positivos de antivirus + validaciones tecla por tecla (`_getch`) + validaciones por línea con rango (`fgets`) + menús instantáneos.
- `VALIDACIONES DE TIPO DE DATO.c`: Menú interactivo de pruebas de validación tecla por tecla (`_getch`) para enteros (positivos, negativos, con signo), decimales, letras, operaciones y vectores.
- `VALIDACIONES DE HOY.c`: Calculadora validada con bloqueo de teclado.
- `VALIDACIONDETECLADOSIMPLE.c`: Versión ligera del hook de teclado en modo usuario.

---

## Estructura de Carpetas y Guías Resueltas

| Carpeta | Contenido / Guía correspondiente | Archivos `.c` |
|---|---|---|
| `EJERCICIOS SIMPLES DE LA PRIMERA CLASE/` | 5 ejercicios básicos de entrada/salida y cálculos | 5 |
| `GUIA DE EJERCICIOS 1/` | `1eraGuia.pdf` (Elecciones, Cuenta corriente BFC, Maratón, Jornal diario) | 5 |
| `GUIA DE EJERCICIOS 2/` | `2daGuia.pdf` / `Ejercicios en Lenguaje C (No Resueltos).pdf` (N1 a N10) | 10 |
| `GUIA DE EJERCICIOS 3/` | `Ejercicios. Elementos Básicos del Lenguaje C.pdf` (N1 a N12) | 12 |
| `GUIA DE EJERCICIOS 4/` | `Ejercicios de Matrices en Lenguaje C sin Resolver.pdf` (N1 a N19 con validaciones completas) | 19 |
| `GUIA DE EJERCICIOS 5/` | `Ejercicios de Matrices.pdf` (`ejercicios matriecs 2.pdf` - 9 ejercicios) | 9 |
| `GUIA DE EJERCICIOS 6/` | `Ejercicios para Elaborar en clase.pdf` (Matrices 34-57, Funciones 1-21, Recursividad 23-26, Estructuras 27-34) | 55 |
| `GUIA DE EJERCICIOS 7/` | `ejecricios en c avanzados.pdf` (12 requerimientos avanzados de examen con recursividad, matrices dinámicas y punteros) | 12 |
| `Guia_Matrices/` | 19 ejercicios de la guía de matrices y vectores (`01` a `19`) | 19 |
| `Guia_Recursividad/` | `Ejercicios de Recursividad.pdf` (`01` a `20`) | 20 |
| `Guia_Switch_Funciones/` | `Ejercicios de Switch y funciones en lenguaje C.pdf` (`01` a `12`) | 12 |
| `Guia_Punteros_Funciones_Matrices/` | `Ejercicios de punteros con funciones y matrices (5).pdf` (`01` a `05`) | 5 |
| `Guia_Estructuras_Secuenciales/` | `Ejercicios de Estructuras Secuenciales.pdf` (`01` a `20`) | 20 |
| `Guia_Progresion_I/` | `Ejercicios Lenguaje C (No resueltos. Prog I).pdf` (`01` a `21`) | 20 |
| `Guia_47_Ejercicios_Seleccion_Repeticion/` | `47 Ejercicio de Lenguaje C.pdf` y `Guia 1 Selección y Repetición` (`01` a `47`) | 44 |
| `Guia_Varios/` | `Ejercicios Varios de Lenguaje C.pdf` (Arreglos, Estructuras, Archivos, Recursividad, Punteros, Listas enlazadas, Árboles binarios) | 8 |
| `PARCIALES DE LABORATORIO RAMIREZ/` | Parcial N1 (`PARCIAL N1.1.c` y `PARCIAL N1.2.c`) y parciales en raíz (`PARCIAL N2 EJERCICIO N1/N2`, `LABORATORIO N1/N2`) | 6+ |

---

## Compilación

Con GCC (TDM-GCC en Dev-C++ o MSYS2 en VS Code):

```bash
gcc -Wall -Wextra archivo.c -o archivo.exe
```
