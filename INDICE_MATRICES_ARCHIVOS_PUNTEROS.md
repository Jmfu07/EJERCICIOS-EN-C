# Índice Completo de Ejercicios: Matrices, Archivos y Punteros

Este documento reúne **todos** los ejercicios del repositorio correspondientes a los tres temas clave (**Matrices**, **Archivos** y **Punteros**), indicando en qué carpeta está cada uno para que puedas localizarlos y practicarlos al instante.

---

## 1. ARCHIVOS (`FILE *`, texto, CSV y binarios) — 14 ejercicios

### `GUIA DE EJERCICIOS 16 - ARCHIVOS/` (10 ejercicios dedicados)
1. `01_crear_leer_agregar_texto.c` — Crear/sobrescribir (`"w"`), agregar (`"a"`) y leer (`"r"`) líneas de texto.
2. `02_contar_lineas_palabras_caracteres.c` — Contar líneas, palabras, caracteres, vocales, dígitos y línea más larga.
3. `03_buscar_reemplazar_palabra.c` — Buscar ocurrencias por línea y reemplazar palabras generando un archivo nuevo.
4. `04_copiar_invertir_y_mayusculas.c` — Copiar carácter a carácter (`fgetc`/`fputc`), convertir a mayúsculas e invertir líneas.
5. `05_numeros_estadisticas_y_ordenar.c` — Leer números dinámicamente, calcular suma/promedio/mín/máx, ordenar y exportar reporte.
6. `06_fusion_and_or_xor_archivos.c` — Operaciones AND, OR y XOR entre dos archivos de enteros ordenados (salida mayor a menor sin duplicados).
7. `07_agenda_contactos_csv.c` — Agenda de contactos CSV (`nombre;telefono;correo`) con alta, listado, búsqueda y borrado físico con archivo temporal.
8. `08_inventario_binario_crud.c` — Inventario CRUD **binario** completo (`fwrite`, `fread`, `fseek`, `ftell`) con edición *in-place*.
9. `09_notas_alumnos_reporte_archivo.c` — Lectura de notas desde archivo, estadísticas y exportación de tabla formateada `.txt`.
10. `10_cifrado_cesar_archivos.c` — Cifrado y descifrado César carácter a carácter sobre archivos de texto.

### Otros ejercicios con manejo de archivos en el repositorio (4 ejercicios)
11. `LENGUAJES/LENGUAJES DE PROGRAMACION ARCHIVOS.c` — Fusión AND/OR/XOR de dos ficheros de enteros con validación y bloqueo Win32 (`a.txt`, `b.txt`, `c.txt`).
12. `GUIA DE EJERCICIOS 14/03_manejo_archivos_texto.c` — Lectura a arreglo dinámico `char **`, escritura, conteo de líneas y búsqueda de palabras.
13. `GUIA DE EJERCICIOS 7/08_laberinto_backtracking.c` — Carga/generación de laberinto desde `laberinto.txt` y resolución con backtracking.
14. `GUIA DE EJERCICIOS 4/N6/notade20alumnosmodificables.c` — Gestión de 20 alumnos con exportación de tabla formateada a `alumnos.txt`.

---

## 2. PUNTEROS Y MEMORIA DINÁMICA — 37 ejercicios

### `GUIA DE EJERCICIOS 17 - PUNTEROS/` (10 ejercicios dedicados)
1. `01_fundamentos_y_paso_por_referencia.c` — Direcciones (`&`), desreferenciación (`*`), `swap`, cociente/resto y mín/máx por referencia.
2. `02_aritmetica_punteros_arreglos.c` — Recorrido, suma, promedio, inversión *in-place* y búsqueda usando solo `*(p + i)` (sin `[]`).
3. `03_cadenas_con_punteros.c` — `mi_strlen`, `mi_strcpy`, `mi_strcat`, `mi_strcmp`, `mi_strrev` y vocales/consonantes con `char *`.
4. `04_ordenamiento_y_busqueda_punteros.c` — Ordenamiento por selección con punteros y búsqueda binaria devolviendo `int *`.
5. `05_memoria_dinamica_vector_redimensionable.c` — Vector dinámico con `malloc` y `realloc` (agregar, insertar, eliminar y liberar).
6. `06_matriz_dinamica_doble_puntero.c` — Matrices dinámicas con `int **`, transpuesta, suma por fila y liberación ordenada.
7. `07_punteros_a_funciones_calculadora_y_orden.c` — Arreglo de punteros a función y ordenamiento genérico con *callback*.
8. `08_estructuras_con_punteros_crud.c` — Arreglo dinámico de `Estudiante *` con operador flecha (`->`), búsqueda y ordenamiento.
9. `09_pila_y_cola_dinamicas_punteros.c` — Pila (LIFO) y Cola (FIFO) dinámicas con nodos y punteros dobles (`Nodo **`).
10. `10_lista_doblemente_enlazada_punteros.c` — Lista doblemente enlazada (`ant` / `sig`) con inserción, eliminación y recorrido bidireccional.

### `GUIA DE EJERCICIOS 10/` — Punteros con funciones y matrices (5 ejercicios)
11. `01_recursividad_diagonales_punteros.c` — Suma recursiva de diagonales usando exclusivamente punteros.
12. `02_punteros_funciones_busqueda_matriz.c` — Búsqueda en matrices dinámicas con punteros a funciones.
13. `03_recorrido_recursivo_caminos_punteros.c` — Recorrido recursivo de caminos en matrices con punteros.
14. `04_operaciones_punteros_a_funciones_matriz.c` — Operaciones elemento a elemento con *callbacks*.
15. `05_recursividad_conteo_primos_punteros.c` — Conteo recursivo de números primos en matriz con punteros.

### `GUIA DE EJERCICIOS 14/` — Estructuras de datos y punteros (8 ejercicios)
16. `01_operaciones_arreglos.c` — Arreglos dinámicos con punteros.
17. `02_estructuras_punteros_personas.c` — CRUD de `Persona *` con punteros.
18. `03_manejo_archivos_texto.c` — Arreglo dinámico `char **` con `malloc`/`realloc`.
19. `04_recursividad_busqueda_ordenamiento.c` — Arreglos dinámicos, búsqueda binaria y ordenamiento.
20. `05_punteros_intercambio_copia_memoria.c` — Intercambio, copia con punteros y liberación segura (`int **`).
21. `06_estructuras_punto_y_libro.c` — Estructuras `Punto` y `Libro` manipuladas por puntero.
22. `07_lista_enlazada_simple.c` — Lista enlazada simple con `Nodo *`.
23. `08_arbol_binario_busqueda.c` — Árbol binario de búsqueda (ABB) recursivo con punteros.

### `GUIA DE EJERCICIOS 7/` — Proyectos avanzados con punteros y matrices dinámicas (12 ejercicios)
24. `01_compresor_matrices_recursivo.c`
25. `02_rotacion_multinivel_anillos.c`
26. `03_arbol_decisiones_matriz.c`
27. `04_permutaciones_restricciones_matriciales.c`
28. `05_evaluador_expresiones_tokens.c`
29. `06_simulacion_ecosistema.c`
30. `07_analizador_matriz_transpuesta_ortogonal.c`
31. `08_laberinto_backtracking.c`
32. `09_gestion_datos_estructuras_busqueda.c`
33. `10_submatrices_determinantes_recursivo.c`
34. `11_procesamiento_numerico_mapa_calor.c`
35. `12_cifrado_descifrado_bloques_zigzag.c`

### `EJERCICIOS_DE_CLASE/` (2 ejercicios)
36. `EJERCICIO DE PUNTEROS 1.c`
37. `EJERCICIO DE PUNTEROS 2.c`

---

## 3. MATRICES Y ARREGLOS MULTIDIMENSIONALES — 100 ejercicios

### `GUIA DE EJERCICIOS 15 - MATRICES/` (20 ejercicios — versión ligera + PDFs)
1. `01_copiar_multiplicar.c`
2. `02_copiar_multiplicar_ordenado.c`
3. `03_media_de_10_numeros.c`
4. `04_ordenar_positivos_negativos.c`
5. `05_tablero_ajedrez.c`
6. `06_notas_clase.c`
7. `07_notas_cursos.c`
8. `08_primos_matriz_vector.c`
9. `09_suma_dos_diagonales.c`
10. `10_primeros_pares.c`
11. `11_contar_positivos_negativos_ceros.c`
12. `12_maximo_y_posicion.c`
13. `13_cuadrado_y_cubo.c`
14. `14_suma_filas_columnas.c`
15. `15_copiar_arrays.c`
16. `16_ordenar_pares_impares.c`
17. `17_vendedores.c`
18. `18_reserva_asientos.c`
19. `19_contar_letras.c`
20. `20_array_3d_ventas_oficinas.c` *(array tridimensional `6×10×2`)*

### `GUIA DE EJERCICIOS 4/` (19 ejercicios — con validaciones `_getch` y bloqueo Win32)
21. `N1/5numsporteclado.c` a `N19/contarletrasrepetidas.c` (19 archivos, equivalentes a los 19 ejercicios de `ejercicios de matrices.pdf`).

### `GUIA DE EJERCICIOS 5/` (9 ejercicios — `ejercicios matriecs 2.pdf`)
40. `01_operaciones_10_numeros.c`
41. `02_ordenar_positivos_negativos.c`
42. `03_tablero_ajedrez.c`
43. `04_primeros_100_matriz.c`
44. `05_primeros_100_suma_columnas.c`
45. `06_matriz_pares_consultar_posicion.c`
46. `07_matriz_3x3_traspuesta.c`
47. `08_cadena_al_reves.c`
48. `09_contar_vocales_cadena.c`

### `GUIA DE EJERCICIOS 6/` — Bloque de Matrices (32 ejercicios)
49–72. `34_matriz_3x3_azar.c` hasta `57_operaciones_matriz_A_vector_X.c` (24 ejercicios: matrices al azar, vocales, sumatorias, promedios, máx/mín y posiciones, producto escalar, suma de matrices, diagonales, bordes, filas impares, columnas pares, descomposición en vectores, transpuesta y álgebra `A`/`B`/`X`).
73–80. `F14_matriz_4x4_dado_filas_impares.c` hasta `F21_menu_escalar_matriz_5x5.c` (8 ejercicios modulares con menús sobre matrices).

### Otras guías con ejercicios de matrices (20 ejercicios)
81–92. `GUIA DE EJERCICIOS 7/` (`01` a `12`, matrices dinámicas avanzadas).
93–97. `GUIA DE EJERCICIOS 10/` (`01` a `05`, matrices dinámicas con punteros).
98. `GUIA DE EJERCICIOS 8/15_suma_matriz.c` (suma recursiva de matriz).
99. `GUIA DE EJERCICIOS 12/21_matriz_4x4_transpuesta_diagonal.c` (matriz 4×4, transpuesta y diagonal).
100. `GUIA DE EJERCICIOS 17 - PUNTEROS/06_matriz_dinamica_doble_puntero.c` (matriz dinámica `int **`).
