# GUIA DE EJERCICIOS 17 — PUNTEROS EN C

Colección progresiva de 10 ejercicios completos sobre punteros, aritmética de punteros, memoria dinámica y estructuras enlazadas en C:

1. `01_fundamentos_y_paso_por_referencia.c`: Direcciones (`&`), desreferenciación (`*`), intercambio (`swap`), cociente/resto y mínimo/máximo por referencia.
2. `02_aritmetica_punteros_arreglos.c`: Recorrido, suma, promedio, inversión *in-place* y búsqueda usando exclusivamente aritmética de punteros (`*(p + i)`), sin notación `[]`.
3. `03_cadenas_con_punteros.c`: Implementación propia con `char *` de `mi_strlen`, `mi_strcpy`, `mi_strcat`, `mi_strcmp`, `mi_strrev` y conteo de vocales/consonantes.
4. `04_ordenamiento_y_busqueda_punteros.c`: Ordenamiento por selección con punteros y búsqueda binaria que retorna `int *` al elemento hallado o `NULL`.
5. `05_memoria_dinamica_vector_redimensionable.c`: Vector dinámico con `malloc` y `realloc` (agregar, insertar, eliminar y liberar memoria segura).
6. `06_matriz_dinamica_doble_puntero.c`: Matrices dinámicas con `int **` (`malloc`/`calloc`), transpuesta, sumas por fila y liberación ordenada.
7. `07_punteros_a_funciones_calculadora_y_orden.c`: Arreglo de punteros a funciones matemáticas y ordenamiento genérico mediante *callback* comparador.
8. `08_estructuras_con_punteros_crud.c`: Arreglo dinámico de `Estudiante *` usando operador flecha (`->`), búsqueda por cédula, actualización y ordenamiento por promedio.
9. `09_pila_y_cola_dinamicas_punteros.c`: Pila (LIFO) y Cola (FIFO) dinámicas implementadas con nodos y punteros dobles (`Nodo **`).
10. `10_lista_doblemente_enlazada_punteros.c`: Lista doblemente enlazada (`ant` y `sig`) con inserción al inicio/final, eliminación y recorrido bidireccional.
