/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 05:
 * Vector dinamico redimensionable con malloc y realloc: agregar al final,
 * insertar en posicion, eliminar por indice, mostrar capacidad y liberar.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

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

static int asegurar_capacidad(int **v, int *cap, int necesario) {
    if (necesario <= *cap) return 1;
    {
        int nueva_cap = (*cap == 0) ? 4 : (*cap * 2);
        int *tmp = (int *)realloc(*v, (size_t)nueva_cap * sizeof(int));
        if (!tmp) return 0;
        *v = tmp;
        *cap = nueva_cap;
        return 1;
    }
}

static void mostrar_vec(const int *v, int n, int cap) {
    int i;
    printf("\nVector (%d/%d elementos): [ ", n, cap);
    for (i = 0; i < n; i++) printf("%d ", *(v + i));
    printf("]\n");
}

int main(void) {
    int *vec = NULL, n = 0, cap = 0, op, val, pos, i;

    for (;;) {
        printf("\n=== PUNTEROS 05: VECTOR DINAMICO (malloc / realloc) ===\n");
        printf("1. Agregar al final\n2. Insertar en posicion\n3. Eliminar por indice\n4. Mostrar\n0. Salir\nOpcion: ");
        do { op = _getch(); if (op == 0 || op == 0xE0) { _getch(); op = 0; } } while (op < '0' || op > '4');
        printf("%c\n", op);
        op -= '0';
        if (op == 0) break;

        if (op == 1) {
            val = leer_int_rango("  Valor a agregar: ", -100000, 100000);
            if (!asegurar_capacidad(&vec, &cap, n + 1)) { printf("  [Error] Sin memoria.\n"); continue; }
            *(vec + n) = val; n++;
            mostrar_vec(vec, n, cap);
        } else if (op == 2) {
            pos = leer_int_rango("  Indice de insercion [0 a N]: ", 0, n);
            val = leer_int_rango("  Valor a insertar: ", -100000, 100000);
            if (!asegurar_capacidad(&vec, &cap, n + 1)) { printf("  [Error] Sin memoria.\n"); continue; }
            for (i = n; i > pos; i--) *(vec + i) = *(vec + i - 1);
            *(vec + pos) = val; n++;
            mostrar_vec(vec, n, cap);
        } else if (op == 3) {
            if (n == 0) { printf("  Vector vacio.\n"); continue; }
            pos = leer_int_rango("  Indice a eliminar: ", 0, n - 1);
            for (i = pos; i < n - 1; i++) *(vec + i) = *(vec + i + 1);
            n--;
            mostrar_vec(vec, n, cap);
        } else if (op == 4) {
            mostrar_vec(vec, n, cap);
        }
    }

    free(vec);
    return 0;
}
