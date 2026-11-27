/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 06:
 * Matriz dinamica con puntero doble (int **): reservar filas/columnas,
 * mostrar, calcular suma de filas/columnas, matriz transpuesta y liberar todo.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>

static int leer_int_rango(const char *msg, int min, int max) {
    char buf[32]; long v; char *fin;
    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtol(buf, &fin, 10);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0') && v >= min && v <= max)
            return (int)v;
        printf("  Error: fuera de rango [%d a %d].\n", min, max);
    }
}

static int **crear_matriz(int filas, int cols) {
    int **m = (int **)malloc((size_t)filas * sizeof(int *));
    int i, j;
    if (!m) return NULL;
    for (i = 0; i < filas; i++) {
        *(m + i) = (int *)calloc((size_t)cols, sizeof(int));
        if (!*(m + i)) {
            for (j = 0; j < i; j++) free(*(m + j));
            free(m);
            return NULL;
        }
    }
    return m;
}

static void liberar_matriz(int **m, int filas) {
    int i;
    if (!m) return;
    for (i = 0; i < filas; i++) free(*(m + i));
    free(m);
}

static void mostrar_matriz(int **m, int filas, int cols, const char *titulo) {
    int i, j;
    printf("\n--- %s (%dx%d) ---\n", titulo, filas, cols);
    for (i = 0; i < filas; i++) {
        for (j = 0; j < cols; j++) printf("%6d ", *(*(m + i) + j));
        printf("\n");
    }
}

int main(void) {
    int f, c, i, j, **mat, **trans;

    printf("=== PUNTEROS 06: MATRIZ DINAMICA CON PUNTERO DOBLE (int **) ===\n\n");
    f = leer_int_rango("Numero de filas [2 a 8]: ", 2, 8);
    c = leer_int_rango("Numero de columnas [2 a 8]: ", 2, 8);

    mat = crear_matriz(f, c);
    trans = crear_matriz(c, f);
    if (!mat || !trans) {
        liberar_matriz(mat, f);
        liberar_matriz(trans, c);
        printf("Error de memoria.\n");
        return 1;
    }

    for (i = 0; i < f; i++) {
        for (j = 0; j < c; j++) {
            char msg[40]; sprintf(msg, "  M[%d][%d]: ", i, j);
            *(*(mat + i) + j) = leer_int_rango(msg, -10000, 10000);
            *(*(trans + j) + i) = *(*(mat + i) + j);
        }
    }

    mostrar_matriz(mat, f, c, "MATRIZ ORIGINAL");
    mostrar_matriz(trans, c, f, "MATRIZ TRANSPUESTA");

    printf("\nSuma por fila:\n");
    for (i = 0; i < f; i++) {
        int s = 0;
        for (j = 0; j < c; j++) s += *(*(mat + i) + j);
        printf("  Fila %d: %d\n", i, s);
    }

    liberar_matriz(mat, f);
    liberar_matriz(trans, c);
    printf("\nMemoria liberada correctamente.\n\n");
    system("pause");
    return 0;
}
