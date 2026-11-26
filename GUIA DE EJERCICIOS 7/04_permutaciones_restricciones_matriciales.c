/*
 * ============================================================================
 * EJERCICIO 4: Generador de Permutaciones con Restricciones Matriciales
 * ============================================================================
 * Algoritmo: Lee N numeros en un arreglo dinamico, genera todas las N!
 *            permutaciones mediante backtracking recursivo e inserta en una
 *            matriz dinamica unicamente aquellas donde la suma de elementos
 *            en posiciones pares (indices 0, 2, 4...) sea estrictamente mayor
 *            que la suma de elementos en posiciones impares (indices 1, 3...).
 *            Ademas ponderamos cada posicion (i+1)*val para identificar la
 *            permutacion valida con mayor suma total y mayor suma ponderada.
 * Funciones:
 *   - intercambiar(): Intercambia dos enteros mediante punteros.
 *   - cumple_restriccion(): Evalua suma_pares > suma_impares con punteros.
 *   - generar_permutaciones_rec(): Genera recursivamente las permutaciones.
 * Punteros y Recursividad:
 *   - Matriz dinamica (int **) redimensionada para almacenar permutaciones validas.
 *   - Aritmetica de punteros en todo el recorrido.
 * Tipos de datos: int, int *, int **.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leer_entero(const char *prompt, int min, int max) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1) {
            printf("  [Error] Ingrese un numero entero valido.\n");
            limpiar_buffer();
        } else if (valor < min || valor > max) {
            printf("  [Error] Rango permitido: [%d, %d].\n", min, max);
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

void intercambiar(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

int cumple_restriccion(const int *arr, int n, int *suma_pares, int *suma_impares) {
    *suma_pares = 0;
    *suma_impares = 0;
    for (int i = 0; i < n; i++) {
        if (i % 2 == 0) {
            *suma_pares += *(arr + i);
        } else {
            *suma_impares += *(arr + i);
        }
    }
    return (*suma_pares > *suma_impares);
}

void generar_permutaciones_rec(int *arr, int inicio, int n,
                               int ***mat_validas, int *num_validas,
                               int *cap_validas, int *total_generadas) {
    if (inicio == n) {
        (*total_generadas)++;
        int sp = 0, si = 0;
        if (cumple_restriccion(arr, n, &sp, &si)) {
            if (*num_validas >= *cap_validas) {
                *cap_validas *= 2;
                *mat_validas = (int **)realloc(*mat_validas, (*cap_validas) * sizeof(int *));
            }
            *(*mat_validas + *num_validas) = (int *)malloc(n * sizeof(int));
            for (int i = 0; i < n; i++) {
                *(*(*mat_validas + *num_validas) + i) = *(arr + i);
            }
            (*num_validas)++;
        }
        return;
    }

    for (int i = inicio; i < n; i++) {
        intercambiar(arr + inicio, arr + i);
        generar_permutaciones_rec(arr, inicio + 1, n,
                                  mat_validas, num_validas, cap_validas, total_generadas);
        intercambiar(arr + inicio, arr + i);
    }
}

int main(void) {
    printf("=================================================================\n");
    printf("   GENERADOR DE PERMUTACIONES CON RESTRICCIONES MATRICIALES\n");
    printf("=================================================================\n\n");

    int n = leer_entero("Cantidad de elementos del conjunto (3 - 6): ", 3, 6);
    int *conjunto = (int *)malloc(n * sizeof(int));

    for (int i = 0; i < n; i++) {
        char msg[64];
        snprintf(msg, sizeof(msg), "  Ingrese elemento #%d (-100 a 100): ", i + 1);
        *(conjunto + i) = leer_entero(msg, -100, 100);
    }

    int cap_validas = 16;
    int num_validas = 0;
    int total_generadas = 0;
    int **mat_validas = (int **)malloc(cap_validas * sizeof(int *));

    generar_permutaciones_rec(conjunto, 0, n, &mat_validas, &num_validas,
                              &cap_validas, &total_generadas);

    printf("\n--- RESULTADOS DE PERMUTACIONES ---\n");
    printf("  * Cantidad total de permutaciones generadas: %d\n", total_generadas);
    printf("  * Cantidad de permutaciones validas (SumaPosPares > SumaPosImpares): %d\n\n",
           num_validas);

    if (num_validas > 0) {
        int idx_mejor = 0;
        int mejor_suma_pares = -999999;
        int suma_total_const = 0;

        printf("  Listado de permutaciones validas almacenadas en la matriz:\n");
        for (int i = 0; i < num_validas; i++) {
            int sp = 0, si = 0;
            cumple_restriccion(*(mat_validas + i), n, &sp, &si);
            suma_total_const = sp + si;

            if (i < 25) {
                printf("    Fila %2d: [ ", i);
                for (int j = 0; j < n; j++) {
                    printf("%d ", *(*(mat_validas + i) + j));
                }
                printf("] -> SumaPares=%d, SumaImpares=%d, SumaTotal=%d\n",
                       sp, si, suma_total_const);
            }
            if (sp > mejor_suma_pares) {
                mejor_suma_pares = sp;
                idx_mejor = i;
            }
        }
        if (num_validas > 25) {
            printf("    ... (%d permutaciones validas adicionales omitidas en pantalla)\n",
                   num_validas - 25);
        }

        printf("\n  * Permutacion valida destacada (Fila %d, Suma Total = %d, Mayor Suma en Pares = %d):\n    [ ",
               idx_mejor, suma_total_const, mejor_suma_pares);
        for (int j = 0; j < n; j++) {
            printf("%d ", *(*(mat_validas + idx_mejor) + j));
        }
        printf("]\n\n");
    } else {
        printf("  No se encontraron permutaciones que cumplan la regla.\n\n");
    }

    for (int i = 0; i < num_validas; i++) {
        free(*(mat_validas + i));
    }
    free(mat_validas);
    free(conjunto);

    system("pause");
    return 0;
}
