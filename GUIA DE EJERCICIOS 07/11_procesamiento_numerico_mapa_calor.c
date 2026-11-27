/*
 * ============================================================================
 * EJERCICIO 11: Simulador de Procesamiento Numerico con Mapa de Calor
 * ============================================================================
 * Algoritmo: Crea dinamicamente un arreglo de tamano F * C usando punteros y
 *            lo transforma logicamente en una matriz de F x C mediante
 *            aritmetica de punteros.
 *            Mediante funciones recursivas calcula la suma de divisores propios
 *            y verifica primalidad para clasificar cada numero en:
 *              - Categoria 1: Primo (y Subtipo: Deficiente)
 *              - Categoria 2: Compuesto Especial Perfecto (suma_div == n)
 *              - Categoria 3: Compuesto Especial Abundante (suma_div > n)
 *              - Categoria 4: Compuesto Deficiente / Unidad (suma_div < n)
 *            Genera un mapa de calor textual con simbolos distintos para cada
 *            categoria y muestra frecuencia por categoria, promedio por fila
 *            y columna, y el elemento mas repetido (moda).
 * Funciones:
 *   - es_primo_rec(): Determina recursivamente si un entero es primo.
 *   - suma_divisores_rec(): Suma recursivamente los divisores propios.
 *   - clasificar_matriz_rec(): Clasifica recursivamente cada celda.
 * Punteros y Recursividad:
 *   - Arreglo 1D accedido como matriz 2D con *(arr + i * cols + j).
 * Tipos de datos: int, double, char.
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

int es_primo_rec(int n, int div) {
    if (n < 2) return 0;
    if (div * div > n) return 1;
    if (n % div == 0) return 0;
    return es_primo_rec(n, div + 1);
}

int suma_divisores_rec(int n, int div) {
    if (n <= 1 || div > n / 2) return 0;
    int actual = (n % div == 0) ? div : 0;
    return actual + suma_divisores_rec(n, div + 1);
}

/*
 * Codigos de clasificacion:
 *   1 = Primo [Simbolo: '*']
 *   2 = Especial Perfecto [Simbolo: '#']
 *   3 = Especial Abundante (Compuesto) [Simbolo: '+']
 *   4 = Compuesto Deficiente / Unidad [Simbolo: '.']
 */
void clasificar_matriz_rec(const int *arr, int *cat, int idx, int total) {
    if (idx >= total) return;

    int val = *(arr + idx);
    int s_div = suma_divisores_rec(val, 1);

    if (s_div == val && val > 1) {
        *(cat + idx) = 2;
    } else if (es_primo_rec(val, 2)) {
        *(cat + idx) = 1;
    } else if (s_div > val) {
        *(cat + idx) = 3;
    } else {
        *(cat + idx) = 4;
    }

    clasificar_matriz_rec(arr, cat, idx + 1, total);
}

char simbolo_categoria(int c) {
    switch (c) {
        case 1: return '*';
        case 2: return '#';
        case 3: return '+';
        default: return '.';
    }
}

int main(void) {
    printf("=================================================================\n");
    printf("   PROCESAMIENTO NUMERICO CON PUNTEROS Y MAPA DE CALOR TEXTUAL\n");
    printf("=================================================================\n\n");

    int filas = leer_entero("Numero de filas de la matriz (2 - 10): ", 2, 10);
    int cols  = leer_entero("Numero de columnas de la matriz (2 - 10): ", 2, 10);
    int total = filas * cols;
    int modo  = leer_entero("Modo (1 = Datos de demostracion variados, 2 = Manual): ", 1, 2);

    int *arr = (int *)malloc(total * sizeof(int));
    int *cat = (int *)malloc(total * sizeof(int));
    if (!arr || !cat) {
        printf("Error de memoria.\n");
        system("pause");
        return 1;
    }

    const int demo[] = {6, 7, 12, 28, 15, 18, 6, 11, 20, 24, 9, 13, 6, 30, 17, 28};
    for (int i = 0; i < total; i++) {
        if (modo == 1) {
            *(arr + i) = demo[i % 16];
        } else {
            char msg[64];
            snprintf(msg, sizeof(msg), "  Elemento #%d (1 - 500): ", i + 1);
            *(arr + i) = leer_entero(msg, 1, 500);
        }
    }

    clasificar_matriz_rec(arr, cat, 0, total);

    printf("\n--- MATRIZ TRANSFORMADA DESDE ARREGLO 1D (%dx%d) ---\n", filas, cols);
    for (int i = 0; i < filas; i++) {
        printf("  Fila %2d: ", i);
        for (int j = 0; j < cols; j++) {
            printf("%4d ", *(arr + i * cols + j));
        }
        printf("\n");
    }

    printf("\n--- MAPA DE CALOR TEXTUAL POR CATEGORIA ---\n");
    printf("Leyenda: [*] Primo | [#] Especial Perfecto | [+] Especial Abundante | [.] Compuesto Deficiente\n\n");
    for (int i = 0; i < filas; i++) {
        printf("  Fila %2d: ", i);
        for (int j = 0; j < cols; j++) {
            int idx = i * cols + j;
            printf(" [%c:%3d] ", simbolo_categoria(*(cat + idx)), *(arr + idx));
        }
        printf("\n");
    }

    int c_primos = 0, c_perfectos = 0, c_abundantes = 0, c_deficientes = 0;
    for (int i = 0; i < total; i++) {
        if (*(cat + i) == 1) c_primos++;
        else if (*(cat + i) == 2) c_perfectos++;
        else if (*(cat + i) == 3) c_abundantes++;
        else c_deficientes++;
    }

    printf("\n--- ESTADISTICAS AVANZADAS ---\n");
    printf("  1) Frecuencia por Categoria:\n");
    printf("     * Primos              [*]: %d\n", c_primos);
    printf("     * Especiales Perfectos[#]: %d\n", c_perfectos);
    printf("     * Especiales Abund.   [+]: %d\n", c_abundantes);
    printf("     * Compuestos Defic.   [.]: %d\n", c_deficientes);
    printf("     * Total Compuestos       : %d\n", c_perfectos + c_abundantes + c_deficientes);

    printf("\n  2) Promedio por Fila:\n");
    for (int i = 0; i < filas; i++) {
        double suma_f = 0.0;
        for (int j = 0; j < cols; j++) {
            suma_f += *(arr + i * cols + j);
        }
        printf("     * Fila %2d: %8.2f\n", i, suma_f / cols);
    }

    printf("\n  3) Promedio por Columna:\n");
    for (int j = 0; j < cols; j++) {
        double suma_c = 0.0;
        for (int i = 0; i < filas; i++) {
            suma_c += *(arr + i * cols + j);
        }
        printf("     * Columna %2d: %8.2f\n", j, suma_c / filas);
    }

    int mas_repetido = *arr;
    int max_frec = 0;
    for (int i = 0; i < total; i++) {
        int frec = 0;
        for (int j = 0; j < total; j++) {
            if (*(arr + j) == *(arr + i)) frec++;
        }
        if (frec > max_frec) {
            max_frec = frec;
            mas_repetido = *(arr + i);
        }
    }
    printf("\n  4) Elemento mas repetido (Moda): %d (aparece %d veces)\n\n",
           mas_repetido, max_frec);

    free(arr);
    free(cat);
    system("pause");
    return 0;
}
