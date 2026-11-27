/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 04:
 * Ordenamiento por seleccion con punteros y busqueda binaria devolviendo un
 * puntero (int *) al elemento encontrado o NULL si no existe.
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

static void ordenar_seleccion_ptr(int *v, int n) {
    int *i, *j, *min_p, *fin = v + n;
    for (i = v; i < fin - 1; i++) {
        min_p = i;
        for (j = i + 1; j < fin; j++)
            if (*j < *min_p) min_p = j;
        if (min_p != i) {
            int tmp = *i; *i = *min_p; *min_p = tmp;
        }
    }
}

static int *busqueda_binaria_ptr(int *v, int n, int objetivo) {
    int *izq = v, *der = v + n - 1;
    while (izq <= der) {
        int *mid = izq + (der - izq) / 2;
        if (*mid == objetivo) return mid;
        if (*mid < objetivo) izq = mid + 1;
        else der = mid - 1;
    }
    return NULL;
}

int main(void) {
    int n, i, objetivo, *v, *res;

    printf("=== PUNTEROS 04: ORDENAMIENTO Y BUSQUEDA BINARIA CON PUNTEROS ===\n\n");
    n = leer_int_rango("Cantidad de elementos [2 a 40]: ", 2, 40);
    v = (int *)malloc((size_t)n * sizeof(int));
    if (!v) { printf("Error de memoria.\n"); return 1; }

    for (i = 0; i < n; i++) {
        char msg[40]; sprintf(msg, "  Elemento [%d]: ", i);
        *(v + i) = leer_int_rango(msg, -100000, 100000);
    }

    ordenar_seleccion_ptr(v, n);
    printf("\nArreglo ordenado: ");
    for (i = 0; i < n; i++) printf("%d ", *(v + i));
    printf("\n");

    objetivo = leer_int_rango("\nValor a buscar (busqueda binaria): ", -100000, 100000);
    res = busqueda_binaria_ptr(v, n, objetivo);
    if (res)
        printf("Encontrado: *res = %d en posicion %ld (direccion %p).\n\n", *res, (long)(res - v), (void *)res);
    else
        printf("No encontrado (NULL).\n\n");

    free(v);
    system("pause");
    return 0;
}
