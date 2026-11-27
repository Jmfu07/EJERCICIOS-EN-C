/*
 * ============================================================================
 * GUIA VARIOS - EJERCICIO 4: Recursividad, Busqueda y Ordenamiento
 * ============================================================================
 * Algoritmo: Reune las funciones recursivas, algoritmos de busqueda,
 *            algoritmos de clasificacion (ordenamiento) y calculadora
 *            modular solicitados en la guia:
 *              1. Factorial recursivo de un numero entero (0 a 20).
 *              2. Secuencia de Fibonacci recursiva hasta N terminos.
 *              3. Ordenamiento de un arreglo por Seleccion Directa y Burbuja.
 *              4. Busqueda Lineal recursiva y Busqueda Binaria recursiva.
 *              5. Calculadora modular (suma, resta, multiplicacion, division).
 * Funciones:
 *   - factorial_rec(), fibonacci_rec()
 *   - ordenar_seleccion(), ordenar_burbuja()
 *   - busqueda_lineal_rec(), busqueda_binaria_rec()
 *   - operar_numeros()
 * Punteros y Recursividad:
 *   - Funciones recursivas puras y manipulacion de arreglos con punteros.
 * Tipos de datos: unsigned long long, int, double.
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

double leer_double(const char *prompt) {
    double valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &valor) != 1) {
            printf("  [Error] Ingrese un numero valido.\n");
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

unsigned long long factorial_rec(int n) {
    if (n <= 1) return 1ULL;
    return (unsigned long long)n * factorial_rec(n - 1);
}

unsigned long long fibonacci_rec(int n) {
    if (n <= 0) return 0ULL;
    if (n == 1) return 1ULL;
    return fibonacci_rec(n - 1) + fibonacci_rec(n - 2);
}

void ordenar_seleccion(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        int min_idx = i;
        for (int j = i + 1; j < n; j++) {
            if (*(arr + j) < *(arr + min_idx)) {
                min_idx = j;
            }
        }
        if (min_idx != i) {
            int temp = *(arr + i);
            *(arr + i) = *(arr + min_idx);
            *(arr + min_idx) = temp;
        }
    }
}

void ordenar_burbuja(int *arr, int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - 1 - i; j++) {
            if (*(arr + j) > *(arr + j + 1)) {
                int temp = *(arr + j);
                *(arr + j) = *(arr + j + 1);
                *(arr + j + 1) = temp;
            }
        }
    }
}

int busqueda_lineal_rec(const int *arr, int n, int idx, int objetivo) {
    if (idx >= n) return -1;
    if (*(arr + idx) == objetivo) return idx;
    return busqueda_lineal_rec(arr, n, idx + 1, objetivo);
}

int busqueda_binaria_rec(const int *arr, int izq, int der, int objetivo) {
    if (izq > der) return -1;
    int med = izq + (der - izq) / 2;
    if (*(arr + med) == objetivo) return med;
    if (*(arr + med) > objetivo) return busqueda_binaria_rec(arr, izq, med - 1, objetivo);
    return busqueda_binaria_rec(arr, med + 1, der, objetivo);
}

double operar_numeros(double a, double b, int op, int *error) {
    *error = 0;
    switch (op) {
        case 1: return a + b;
        case 2: return a - b;
        case 3: return a * b;
        case 4:
            if (b == 0.0) {
                *error = 1;
                return 0.0;
            }
            return a / b;
        default:
            return 0.0;
    }
}

void imprimir_arreglo(const int *arr, int n, const char *titulo) {
    printf("  %s: [ ", titulo);
    for (int i = 0; i < n; i++) {
        printf("%d ", *(arr + i));
    }
    printf("]\n");
}

int main(void) {
    printf("=================================================================\n");
    printf("   RECURSIVIDAD, BUSQUEDA, ORDENAMIENTO Y FUNCIONES\n");
    printf("=================================================================\n");

    int opcion = 0;
    do {
        printf("\n------------------ MENU PRINCIPAL ------------------\n");
        printf("  1. Calcular Factorial recursivo\n");
        printf("  2. Generar secuencia de Fibonacci recursiva\n");
        printf("  3. Ordenar arreglo (Seleccion o Burbuja) y Buscar (Lineal y Binaria)\n");
        printf("  4. Calculadora con funciones (Suma, Resta, Multiplicacion, Division)\n");
        printf("  5. Salir\n");
        opcion = leer_entero("Seleccione una opcion (1 - 5): ", 1, 5);

        if (opcion == 1) {
            int n = leer_entero("  Ingrese un entero n (0 - 20): ", 0, 20);
            printf("  * El factorial de %d! es: %llu\n", n, factorial_rec(n));
        } else if (opcion == 2) {
            int n = leer_entero("  Cantidad de terminos de Fibonacci a generar (1 - 35): ", 1, 35);
            printf("  * Secuencia Fibonacci (%d terminos):\n    ", n);
            for (int i = 0; i < n; i++) {
                printf("%llu ", fibonacci_rec(i));
            }
            printf("\n");
        } else if (opcion == 3) {
            int n = leer_entero("  Cantidad de elementos del arreglo (2 - 30): ", 2, 30);
            int *arr = (int *)malloc(n * sizeof(int));
            if (!arr) {
                printf("  [Error] Memoria insuficiente.\n");
                continue;
            }
            for (int i = 0; i < n; i++) {
                char msg[64];
                snprintf(msg, sizeof(msg), "    Elemento [%d] (-1000 a 1000): ", i);
                *(arr + i) = leer_entero(msg, -1000, 1000);
            }

            imprimir_arreglo(arr, n, "Arreglo ingresado");
            int val_lineal = leer_entero("  Valor a buscar con Busqueda Lineal Recursiva: ", -1000, 1000);
            int idx_lin = busqueda_lineal_rec(arr, n, 0, val_lineal);
            printf("  -> Indice en Busqueda Lineal: %d\n", idx_lin);

            int metodo = leer_entero("  Metodo de clasificacion (1 = Seleccion Directa, 2 = Burbuja): ", 1, 2);
            if (metodo == 1) {
                ordenar_seleccion(arr, n);
                imprimir_arreglo(arr, n, "Arreglo ordenado por Seleccion Directa");
            } else {
                ordenar_burbuja(arr, n);
                imprimir_arreglo(arr, n, "Arreglo ordenado por Burbuja");
            }

            int val_bin = leer_entero("  Valor a buscar con Busqueda Binaria Recursiva: ", -1000, 1000);
            int idx_bin = busqueda_binaria_rec(arr, 0, n - 1, val_bin);
            printf("  -> Indice en Busqueda Binaria: %d\n", idx_bin);

            free(arr);
        } else if (opcion == 4) {
            double a = leer_double("  Ingrese el primer numero: ");
            double b = leer_double("  Ingrese el segundo numero: ");
            int op = leer_entero("  Operacion (1=Suma, 2=Resta, 3=Multiplicacion, 4=Division): ", 1, 4);
            int err = 0;
            double res = operar_numeros(a, b, op, &err);
            if (err) {
                printf("  [Error] No es posible dividir entre cero.\n");
            } else {
                printf("  * Resultado = %.4f\n", res);
            }
        }
    } while (opcion != 5);

    printf("\nPrograma finalizado.\n");
    system("pause");
    return 0;
}
