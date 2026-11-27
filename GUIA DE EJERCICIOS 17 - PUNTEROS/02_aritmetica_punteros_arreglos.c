/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 02:
 * Aritmetica de punteros sobre arreglos (sin notacion []): carga, suma,
 * promedio, inversion in-place con dos punteros y busqueda devolviendo puntero.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>

#define MAX_N 50

static int leer_int_rango(const char *msg, int min, int max) {
    char buf[32]; long v; char *fin;
    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtol(buf, &fin, 10);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0') && v >= min && v <= max)
            return (int)v;
        printf("  Error: valor fuera de rango [%d a %d].\n", min, max);
    }
}

static void mostrar_ptr(const int *v, int n, const char *titulo) {
    const int *p = v, *fin = v + n;
    printf("%s: ", titulo);
    while (p < fin) { printf("%d ", *p); p++; }
    printf("\n");
}

static long long sumar_ptr(const int *v, int n) {
    long long s = 0;
    const int *p;
    for (p = v; p < v + n; p++) s += *p;
    return s;
}

static void invertir_ptr(int *v, int n) {
    int *izq = v, *der = v + n - 1;
    while (izq < der) {
        int tmp = *izq;
        *izq = *der;
        *der = tmp;
        izq++; der--;
    }
}

static const int *buscar_ptr(const int *v, int n, int objetivo) {
    const int *p;
    for (p = v; p < v + n; p++)
        if (*p == objetivo) return p;
    return NULL;
}

int main(void) {
    int datos[MAX_N], n, i, objetivo;
    long long suma;
    const int *hallado;

    printf("=== PUNTEROS 02: ARITMETICA DE PUNTEROS EN ARREGLOS ===\n\n");
    n = leer_int_rango("Cantidad de elementos [2 a 50]: ", 2, MAX_N);
    for (i = 0; i < n; i++) {
        char msg[40];
        sprintf(msg, "  Elemento [%d]: ", i);
        *(datos + i) = leer_int_rango(msg, -100000, 100000);
    }

    mostrar_ptr(datos, n, "\nOriginal ");
    suma = sumar_ptr(datos, n);
    printf("Suma: %lld | Promedio: %.2f\n", suma, (double)suma / n);

    invertir_ptr(datos, n);
    mostrar_ptr(datos, n, "Invertido");

    objetivo = leer_int_rango("\nValor a buscar en el arreglo invertido: ", -100000, 100000);
    hallado = buscar_ptr(datos, n, objetivo);
    if (hallado)
        printf("Encontrado en indice %ld (direccion %p).\n\n", (long)(hallado - datos), (const void *)hallado);
    else
        printf("Valor no encontrado (puntero NULL).\n\n");

    system("pause");
    return 0;
}
