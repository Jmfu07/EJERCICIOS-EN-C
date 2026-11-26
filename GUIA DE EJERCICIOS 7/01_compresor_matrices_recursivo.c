/*
 * ============================================================================
 * EJERCICIO 1: Compresor de Matrices por Segmentos con Recursividad y Punteros
 * ============================================================================
 * Algoritmo: Run-Length Encoding (RLE) por filas usando recursividad y punteros.
 *            Por cada fila de la matriz dinamica, una funcion recursiva recorre
 *            los elementos consecutivos iguales y genera pares (valor, cantidad)
 *            almacenados dinamicamente. Luego reconstruye la matriz original
 *            a partir de la estructura comprimida y verifica su exactitud.
 * Funciones:
 *   - leer_entero(): Valida entradas enteras dentro de un rango.
 *   - contar_iguales_rec(): Cuenta recursivamente elementos consecutivos iguales.
 *   - comprimir_fila_rec(): Comprime recursivamente una fila en pares (valor, cant).
 *   - reconstruir_fila_rec(): Reconstruye recursivamente una fila desde sus pares.
 * Punteros y Recursividad:
 *   - Doble puntero (int **) para matriz original y reconstruida.
 *   - Arreglo de punteros a estructuras (Segmento **) para la compresion.
 *   - Aritmetica de punteros (*(*(mat + i) + j)) en todas las operaciones.
 * Tipos de datos: int, struct Segmento, size_t.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int valor;
    int cantidad;
} Segmento;

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leer_entero(const char *prompt, int min, int max) {
    int valor;
    int valido = 0;
    while (!valido) {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1) {
            printf("  [Error] Entrada invalida. Ingrese un numero entero.\n");
            limpiar_buffer();
        } else if (valor < min || valor > max) {
            printf("  [Error] El valor debe estar entre %d y %d.\n", min, max);
            limpiar_buffer();
        } else {
            limpiar_buffer();
            valido = 1;
        }
    }
    return valor;
}

int contar_iguales_rec(const int *fila, int col_actual, int cols, int valor_ref) {
    if (col_actual >= cols || *(fila + col_actual) != valor_ref) {
        return 0;
    }
    return 1 + contar_iguales_rec(fila, col_actual + 1, cols, valor_ref);
}

void comprimir_fila_rec(const int *fila, int col_actual, int cols,
                        Segmento *dest, int *num_seg) {
    if (col_actual >= cols) {
        return;
    }
    int val = *(fila + col_actual);
    int cant = contar_iguales_rec(fila, col_actual, cols, val);

    (dest + *num_seg)->valor = val;
    (dest + *num_seg)->cantidad = cant;
    (*num_seg)++;

    comprimir_fila_rec(fila, col_actual + cant, cols, dest, num_seg);
}

void reconstruir_fila_rec(const Segmento *segs, int seg_idx, int total_segs,
                          int *fila_dest, int *col_dest) {
    if (seg_idx >= total_segs) {
        return;
    }
    int val = (segs + seg_idx)->valor;
    int cant = (segs + seg_idx)->cantidad;
    for (int k = 0; k < cant; k++) {
        *(fila_dest + *col_dest) = val;
        (*col_dest)++;
    }
    reconstruir_fila_rec(segs, seg_idx + 1, total_segs, fila_dest, col_dest);
}

void imprimir_matriz(int **mat, int filas, int cols, const char *titulo) {
    printf("\n--- %s (%dx%d) ---\n", titulo, filas, cols);
    for (int i = 0; i < filas; i++) {
        printf("  Fila %2d: ", i);
        for (int j = 0; j < cols; j++) {
            printf("%4d ", *(*(mat + i) + j));
        }
        printf("\n");
    }
}

int main(void) {
    printf("=================================================================\n");
    printf("   COMPRESOR DE MATRICES POR SEGMENTOS (RLE RECURSIVO)\n");
    printf("=================================================================\n\n");

    int filas = leer_entero("Ingrese el numero de filas (1 - 15): ", 1, 15);
    int cols  = leer_entero("Ingrese el numero de columnas (1 - 15): ", 1, 15);
    int modo  = leer_entero("Modo de llenado (1 = Manual, 2 = Ejemplo con segmentos): ", 1, 2);

    int **matriz = (int **)malloc(filas * sizeof(int *));
    if (!matriz) {
        printf("Error de memoria.\n");
        system("pause");
        return 1;
    }

    for (int i = 0; i < filas; i++) {
        *(matriz + i) = (int *)malloc(cols * sizeof(int));
        for (int j = 0; j < cols; j++) {
            if (modo == 1) {
                char msg[64];
                snprintf(msg, sizeof(msg), "  Elemento [%d][%d] (-999 a 999): ", i, j);
                *(*(matriz + i) + j) = leer_entero(msg, -999, 999);
            } else {
                *(*(matriz + i) + j) = (i + (j / 2)) % 5 + 1;
            }
        }
    }

    imprimir_matriz(matriz, filas, cols, "MATRIZ ORIGINAL");

    Segmento **comprimida = (Segmento **)malloc(filas * sizeof(Segmento *));
    int *conteo_segs = (int *)malloc(filas * sizeof(int));

    for (int i = 0; i < filas; i++) {
        *(comprimida + i) = (Segmento *)malloc(cols * sizeof(Segmento));
        *(conteo_segs + i) = 0;
        comprimir_fila_rec(*(matriz + i), 0, cols, *(comprimida + i), conteo_segs + i);
    }

    printf("\n--- ESTRUCTURA COMPRIMIDA (Valor, Cantidad) ---\n");
    for (int i = 0; i < filas; i++) {
        printf("  Fila %2d (%d segmentos): ", i, *(conteo_segs + i));
        for (int s = 0; s < *(conteo_segs + i); s++) {
            Segmento *seg = *(comprimida + i) + s;
            printf("(%d, %d) ", seg->valor, seg->cantidad);
        }
        printf("\n");
    }

    int **reconstruida = (int **)malloc(filas * sizeof(int *));
    for (int i = 0; i < filas; i++) {
        *(reconstruida + i) = (int *)malloc(cols * sizeof(int));
        int col_dest = 0;
        reconstruir_fila_rec(*(comprimida + i), 0, *(conteo_segs + i),
                             *(reconstruida + i), &col_dest);
    }

    imprimir_matriz(reconstruida, filas, cols, "MATRIZ RECONSTRUIDA");

    int iguales = 1;
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            if (*(*(matriz + i) + j) != *(*(reconstruida + i) + j)) {
                iguales = 0;
            }
        }
    }

    printf("\n[VALIDACION]: La matriz reconstruida es %s a la original.\n\n",
           iguales ? "IDENTICA (Proceso exitoso)" : "DIFERENTE (Error)");

    for (int i = 0; i < filas; i++) {
        free(*(matriz + i));
        free(*(comprimida + i));
        free(*(reconstruida + i));
    }
    free(matriz);
    free(comprimida);
    free(conteo_segs);
    free(reconstruida);

    system("pause");
    return 0;
}
