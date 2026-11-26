/*
 * ============================================================================
 * GUIA VARIOS - EJERCICIO 1: Operaciones con Arreglos
 * ============================================================================
 * Algoritmo: Implementa las operaciones fundamentales sobre un arreglo de
 *            numeros enteros usando punteros y funciones modulares:
 *              1. Buscar el elemento mas grande y el mas pequeno del arreglo.
 *              2.Calcular la suma de todos los elementos del arreglo.
 *              3. Contar la frecuencia de ocurrencias de un elemento dado
 *                 y devolver su primer indice (o -1 si no existe).
 *              4. Revertir el arreglo in-place (intercambiando de principio
 *                 a fin mediante punteros).
 * Funciones:
 *   - buscar_mayor_menor()
 *   - sumar_arreglo()
 *   - contar_frecuencia_y_buscar()
 *   - revertir_arreglo_inplace()
 * Punteros y Recursividad:
 *   - Uso de punteros int * para recorrer e invertir el arreglo in-place.
 * Tipos de datos: int, int *.
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

void mostrar_arreglo(const int *arr, int n, const char *titulo) {
    printf("  %s: [ ", titulo);
    for (int i = 0; i < n; i++) {
        printf("%d ", *(arr + i));
    }
    printf("]\n");
}

void buscar_mayor_menor(const int *arr, int n, int *mayor, int *menor) {
    *mayor = *arr;
    *menor = *arr;
    for (int i = 1; i < n; i++) {
        if (*(arr + i) > *mayor) *mayor = *(arr + i);
        if (*(arr + i) < *menor) *menor = *(arr + i);
    }
}

int sumar_arreglo(const int *arr, int n) {
    int suma = 0;
    for (int i = 0; i < n; i++) {
        suma += *(arr + i);
    }
    return suma;
}

int contar_frecuencia_y_buscar(const int *arr, int n, int objetivo, int *primer_indice) {
    int frecuencia = 0;
    *primer_indice = -1;
    for (int i = 0; i < n; i++) {
        if (*(arr + i) == objetivo) {
            if (*primer_indice == -1) {
                *primer_indice = i;
            }
            frecuencia++;
        }
    }
    return frecuencia;
}

void revertir_arreglo_inplace(int *arr, int n) {
    int *izq = arr;
    int *der = arr + n - 1;
    while (izq < der) {
        int temp = *izq;
        *izq = *der;
        *der = temp;
        izq++;
        der--;
    }
}

int main(void) {
    printf("=================================================================\n");
    printf("   OPERACIONES CON ARREGLOS (MAYOR/MENOR, FRECUENCIA, REVERTIR)\n");
    printf("=================================================================\n\n");

    int n = leer_entero("Ingrese la cantidad de elementos del arreglo (1 - 50): ", 1, 50);
    int *arr = (int *)malloc(n * sizeof(int));
    if (!arr) {
        printf("Error al reservar memoria.\n");
        system("pause");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        char msg[64];
        snprintf(msg, sizeof(msg), "  Elemento [%d] (-10000 a 10000): ", i);
        *(arr + i) = leer_entero(msg, -10000, 10000);
    }

    printf("\n--- RESULTADOS ---\n");
    mostrar_arreglo(arr, n, "Arreglo Original");

    int mayor = 0, menor = 0;
    buscar_mayor_menor(arr, n, &mayor, &menor);
    int suma = sumar_arreglo(arr, n);

    printf("  * Elemento mas grande (Maximo): %d\n", mayor);
    printf("  * Elemento mas pequeno (Minimo): %d\n", menor);
    printf("  * Suma total de elementos     : %d\n\n", suma);

    int buscado = leer_entero("Ingrese un numero para buscar y contar su frecuencia: ", -10000, 10000);
    int primer_idx = -1;
    int frec = contar_frecuencia_y_buscar(arr, n, buscado, &primer_idx);

    printf("  * Frecuencia de %d en el arreglo: %d aparicion(es)\n", buscado, frec);
    printf("  * Primer indice encontrado      : %d\n\n", primer_idx);

    revertir_arreglo_inplace(arr, n);
    mostrar_arreglo(arr, n, "Arreglo Revertido In-Place");
    printf("\n");

    free(arr);
    system("pause");
    return 0;
}
