/*
 * ============================================================================
 * GUIA VARIOS - EJERCICIO 5: Punteros (Intercambio, Copia y Memoria Dinamica)
 * ============================================================================
 * Algoritmo: Implementa los ejercicios de punteros especificados en la guia:
 *              1. Funcion para intercambiar el valor de dos variables usando
 *                 punteros.
 *              2. Funcion para copiar un arreglo hacia otro bloque de memoria
 *                 utilizando exclusivamente aritmetica de punteros.
 *              3. Funcion segura para liberar la memoria asignada
 *                 dinamicamente y poner el puntero en NULL.
 * Funciones:
 *   - intercambiar_valores(): Intercambia dos enteros por referencia.
 *   - copiar_arreglo_punteros(): Copia n elementos usando *(dest + i) = *(orig + i).
 *   - liberar_memoria_segura(): Libera un bloque dinamico y anula su puntero.
 * Punteros y Recursividad:
 *   - Punteros simples (int *) y puntero doble (int **) para liberacion segura.
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

void intercambiar_valores(int *a, int *b) {
    int temp = *a;
    *a = *b;
    *b = temp;
}

void copiar_arreglo_punteros(const int *origen, int *destino, int n) {
    for (int i = 0; i < n; i++) {
        *(destino + i) = *(origen + i);
    }
}

void liberar_memoria_segura(int **ptr) {
    if (ptr != NULL && *ptr != NULL) {
        free(*ptr);
        *ptr = NULL;
    }
}

void mostrar_arreglo(const int *arr, int n, const char *titulo) {
    printf("  %s (Direccion base: %p): [ ", titulo, (const void *)arr);
    for (int i = 0; i < n; i++) {
        printf("%d ", *(arr + i));
    }
    printf("]\n");
}

int main(void) {
    printf("=================================================================\n");
    printf("   PUNTEROS: INTERCAMBIO, COPIA DE ARREGLOS Y MEMORIA DINAMICA\n");
    printf("=================================================================\n\n");

    printf("--- 1. INTERCAMBIO DE DOS VARIABLES USANDO PUNTEROS ---\n");
    int x = leer_entero("  Ingrese el valor de la variable X (-10000 a 10000): ", -10000, 10000);
    int y = leer_entero("  Ingrese el valor de la variable Y (-10000 a 10000): ", -10000, 10000);
    printf("  Antes del intercambio : X = %d (dir %p), Y = %d (dir %p)\n",
           x, (void *)&x, y, (void *)&y);
    intercambiar_valores(&x, &y);
    printf("  Despues del intercambio: X = %d (dir %p), Y = %d (dir %p)\n\n",
           x, (void *)&x, y, (void *)&y);

    printf("--- 2. COPIA DE UN ARREGLO CON ARITMETICA DE PUNTEROS ---\n");
    int n = leer_entero("  Cantidad de elementos del arreglo dinamico (1 - 30): ", 1, 30);

    int *origen = (int *)malloc(n * sizeof(int));
    int *copia  = (int *)malloc(n * sizeof(int));
    if (!origen || !copia) {
        printf("  [Error] Fallo la asignacion de memoria.\n");
        system("pause");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        char msg[64];
        snprintf(msg, sizeof(msg), "    Elemento [%d] (-10000 a 10000): ", i);
        *(origen + i) = leer_entero(msg, -10000, 10000);
    }

    copiar_arreglo_punteros(origen, copia, n);
    printf("\n");
    mostrar_arreglo(origen, n, "Arreglo Origen");
    mostrar_arreglo(copia, n,  "Arreglo Copia ");

    printf("\n--- 3. LIBERACION DE MEMORIA ASIGNADA DINAMICAMENTE ---\n");
    liberar_memoria_segura(&origen);
    liberar_memoria_segura(&copia);
    printf("  * Puntero 'origen' tras liberar: %p\n", (void *)origen);
    printf("  * Puntero 'copia'  tras liberar: %p\n\n", (void *)copia);

    system("pause");
    return 0;
}
