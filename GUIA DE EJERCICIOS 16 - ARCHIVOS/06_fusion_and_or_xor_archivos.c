/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 06:
 * Operaciones de conjuntos AND (interseccion), OR (union) y XOR (diferencia
 * simetrica) entre dos archivos de enteros ordenados, guardando resultado
 * ordenado de mayor a menor sin duplicados.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static void crear_ejemplos(void) {
    FILE *f;
    if ((f = fopen("conjunto_a.txt", "r")) != NULL) fclose(f);
    else if ((f = fopen("conjunto_a.txt", "w")) != NULL) { fprintf(f, "1 3 5 7 9 12 15\n"); fclose(f); }
    if ((f = fopen("conjunto_b.txt", "r")) != NULL) fclose(f);
    else if ((f = fopen("conjunto_b.txt", "w")) != NULL) { fprintf(f, "2 3 5 8 12 20\n"); fclose(f); }
}

static int cargar_enteros(const char *nom, int *v, int max) {
    FILE *f = fopen(nom, "r");
    int n = 0;
    if (!f) return -1;
    while (n < max && fscanf(f, "%d", &v[n]) == 1) n++;
    fclose(f);
    return n;
}

static void ordenar_asc(int *v, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1 - i; j++)
            if (v[j] > v[j + 1]) { int t = v[j]; v[j] = v[j + 1]; v[j + 1] = t; }
}

static void agregar_unico(int *out, int *n, int val) {
    if (*n == 0 || out[*n - 1] != val) out[(*n)++] = val;
}

static int operar_conjuntos(const int *a, int na, const int *b, int nb, int *out, int op) {
    int i = 0, j = 0, k = 0;
    while (i < na && j < nb) {
        if (a[i] < b[j]) {
            if (op == 2 || op == 3) agregar_unico(out, &k, a[i]);
            i++;
        } else if (b[j] < a[i]) {
            if (op == 2 || op == 3) agregar_unico(out, &k, b[j]);
            j++;
        } else {
            if (op == 1 || op == 2) agregar_unico(out, &k, a[i]);
            i++; j++;
        }
    }
    while (i < na) { if (op == 2 || op == 3) agregar_unico(out, &k, a[i]); i++; }
    while (j < nb) { if (op == 2 || op == 3) agregar_unico(out, &k, b[j]); j++; }
    return k;
}

int main(void) {
    int a[256], b[256], out[512], na, nb, nout, op, i;
    FILE *f;

    crear_ejemplos();
    na = cargar_enteros("conjunto_a.txt", a, 256);
    nb = cargar_enteros("conjunto_b.txt", b, 256);
    if (na < 0 || nb < 0) { printf("[Error] No se pudieron abrir los archivos.\n"); system("pause"); return 1; }
    ordenar_asc(a, na);
    ordenar_asc(b, nb);

    for (;;) {
        printf("\n=== ARCHIVOS 06: AND / OR / XOR DE FICHEROS ===\n");
        printf("1. AND (en ambos)\n2. OR (en cualquiera, sin repetidos)\n3. XOR (solo en uno)\n0. Salir\nOpcion: ");
        do { op = _getch(); if (op == 0 || op == 0xE0) { _getch(); op = 0; } } while (op < '0' || op > '3');
        printf("%c\n", op);
        op -= '0';
        if (op == 0) break;

        nout = operar_conjuntos(a, na, b, nb, out, op);
        f = fopen("conjunto_salida.txt", "w");
        if (!f) { printf("[Error] No se pudo escribir 'conjunto_salida.txt'.\n"); continue; }
        printf("Resultado (mayor a menor, %d elementos):", nout);
        for (i = nout - 1; i >= 0; i--) {
            printf(" %d", out[i]);
            fprintf(f, "%d\n", out[i]);
        }
        printf("\n[OK] Guardado en 'conjunto_salida.txt'.\n");
        fclose(f);
    }
    return 0;
}
