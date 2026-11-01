# Guias resueltas en C

Cada ejercicio está separado en su propio archivo `.c`. Al principio de cada archivo se incluye:

- Qué hace el algoritmo.
- Cómo funciona la solución.
- Qué tipos de datos utiliza.

## Guia de recursividad

| Archivo | Ejercicio | Tipos principales |
|---|---|---|
| `01_factorial.c` | Factorial | `int`, `long long` |
| `02_fibonacci.c` | Fibonacci | `int`, `long long` |
| `03_suma_digitos.c` | Suma de dígitos | `int` |
| `04_invertir_cadena.c` | Inversión de cadena | `char[]`, `int` |
| `05_contar_elementos_arreglo.c` | Conteo de arreglo | `int[]`, `int` |
| `06_evaluar_expresion.c` | Expresión aritmética | `char[]`, `int` |
| `07_busqueda_binaria.c` | Búsqueda binaria | `int[]`, `int` |
| `08_palindromo.c` | Palíndromo | `char[]`, `int` |
| `09_combinaciones_binarias.c` | Combinaciones binarias | `char[]`, `int` |
| `10_torres_hanoi.c` | Torres de Hanoi | `int`, `char` |
| `11_contar_vocales.c` | Conteo de vocales | `char[]`, `char`, `int` |
| `12_decimal_a_binario.c` | Decimal a binario | `unsigned int` |
| `13_mcd.c` | Máximo común divisor | `int` |
| `14_numero_primo.c` | Número primo | `int` |
| `15_suma_matriz.c` | Suma de matriz | `int[][]`, `int` |
| `16_ocurrencias_arreglo.c` | Ocurrencias | `int[]`, `int` |
| `17_multiplicacion_sin_asterisco.c` | Multiplicación recursiva | `int` |
| `18_arreglo_inverso.c` | Arreglo inverso | `int[]`, `int` |
| `19_potencia.c` | Potencia | `int`, `long long` |
| `20_eliminar_espacios.c` | Eliminar espacios | `char[]`, `int` |

## Guia de switch y funciones

| Archivo | Ejercicio | Tipos principales |
|---|---|---|
| `01_intervalo.c` | Intervalo de 100 a 200 | `double` |
| `02_divisores.c` | Divisores | `int` |
| `03_ternas_pitagoricas.c` | Ternas pitagóricas | `int` |
| `04_e_x.c` | Aproximación de e^x | `double`, `int` |
| `05_valor_absoluto.c` | Valor absoluto | `double` |
| `06_promocion_peliculas.c` | Promoción de películas | `double` |
| `07_calificaciones.c` | Calificaciones | `double`, `int` |
| `08_collatz.c` | Conjetura de Collatz | `long long` |
| `09_craps.c` | Juego de craps | `int`, `time_t` |
| `10_menu_funciones.c` | Pares, perfectos e inversión | `int` |
| `11_mcd_recursivo.c` | MCD recursivo | `int` |
| `12_ackermann.c` | Función de Ackermann | `unsigned int`, `unsigned long long` |

## Guia de matrices

| Archivo | Ejercicio | Tipos principales |
|---|---|---|
| `01_copiar_multiplicar.c` | Copiar y multiplicar por 2 | `int`, `int[]` |
| `02_copiar_multiplicar_ordenado.c` | Copiar, multiplicar y ordenar | `int`, `int[]` |
| `03_media_de_10_numeros.c` | Media de 10 números | `double`, `int[]` |
| `04_ordenar_positivos_negativos.c` | Ordenar positivos y negativos | `int`, `int[]` |
| `05_tablero_ajedrez.c` | Tablero de ajedrez | `char`, `char[][]` |
| `06_notas_clase.c` | Gestión de notas de una clase | `char[][]`, `double[]`, `int` |
| `07_notas_cursos.c` | Notas por curso y asignatura | `double[][][]`, `int` |
| `08_primos_matriz_vector.c` | Primos en matriz y vector | `int[][]`, `int[]` |
| `09_suma_dos_diagonales.c` | Suma de diagonales | `int[][]`, `int` |
| `10_primeros_pares.c` | Primeros 20 pares | `int[]` |
| `11_contar_positivos_negativos_ceros.c` | Clasificar valores | `int[]`, `int` |
| `12_maximo_y_posicion.c` | Máximo y posición | `double[]`, `int` |
| `13_cuadrado_y_cubo.c` | Cuadrado y cubo | `int[]`, `long long` |
| `14_suma_filas_columnas.c` | Suma de filas y columnas | `int[][]`, `int` |
| `15_copiar_arrays.c` | Copiar arrays con una función | `int[]`, `int` |
| `16_ordenar_pares_impares.c` | Ordenar pares e impares | `int[]`, `time_t` |
| `17_vendedores.c` | Ingresos de vendedores | `double[][]`, `int` |
| `18_reserva_asientos.c` | Reserva de asientos | `int[][]`, `int` |
| `19_contar_letras.c` | Contar letras repetidas | `char[]`, `int[]` |

## Compilación

Desde una terminal ubicada dentro de cualquiera de las carpetas:

```text
gcc archivo.c -o programa
```

Por ejemplo:

```text
gcc 01_factorial.c -o factorial
```

Todos los archivos fueron compilados con GCC usando `-std=c11 -Wall -Wextra -pedantic`.
