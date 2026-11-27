/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 05:
 * Leer numeros desde un archivo, calcular estadisticas (suma, promedio, min,
 * max), ordenarlos de menor a mayor y exportar reporte a otro archivo.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>

#define ARCH_NUMS "numeros_entrada.txt"
#define ARCH_REP  "reporte_numeros.txt"

static void crear_archivo_ejemplo(const char *nom) {
    FILE *f = fopen(nom, "r");
    if (f) { fclose(f); return; }
    f = fopen(nom, "w");
    if (!f) return;
    fprintf(f, "42.5\n-10\n18.75\n0\n99.2\n-3.5\n25\n14.1\n");
    fclose(f);
}

static void ordenar(double *v, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1 - i; j++)
            if (v[j] > v[j + 1]) { double t = v[j]; v[j] = v[j + 1]; v[j + 1] = t; }
}

int main(void) {
    FILE *fin, *fout;
    double *nums = NULL, x, suma = 0.0;
    int n = 0, cap = 16, i;

    crear_archivo_ejemplo(ARCH_NUMS);
    fin = fopen(ARCH_NUMS, "r");
    if (!fin) { printf("[Error] No se pudo abrir '%s'.\n", ARCH_NUMS); system("pause"); return 1; }

    nums = (double *)malloc((size_t)cap * sizeof(double));
    if (!nums) { fclose(fin); return 1; }

    while (fscanf(fin, "%lf", &x) == 1) {
        if (n >= cap) {
            double *tmp;
            cap *= 2;
            tmp = (double *)realloc(nums, (size_t)cap * sizeof(double));
            if (!tmp) { free(nums); fclose(fin); return 1; }
            nums = tmp;
        }
        nums[n++] = x;
        suma += x;
    }
    fclose(fin);

    if (n == 0) {
        printf("[Aviso] No se encontraron numeros validos en '%s'.\n", ARCH_NUMS);
        free(nums); system("pause"); return 0;
    }

    ordenar(nums, n);
    fout = fopen(ARCH_REP, "w");
    if (!fout) { free(nums); printf("[Error] No se pudo crear '%s'.\n", ARCH_REP); system("pause"); return 1; }

    fprintf(fout, "=== REPORTE ESTADISTICO (%s) ===\n", ARCH_NUMS);
    fprintf(fout, "Cantidad : %d\nSuma     : %.2f\nPromedio : %.2f\nMinimo   : %.2f\nMaximo   : %.2f\n",
            n, suma, suma / n, nums[0], nums[n - 1]);
    fprintf(fout, "Ordenados: ");
    for (i = 0; i < n; i++) fprintf(fout, "%.2f%s", nums[i], (i + 1 < n) ? ", " : "\n");
    fclose(fout);

    printf("=== ARCHIVOS 05: ESTADISTICAS Y ORDENAMIENTO ===\n\n");
    printf("Leidos %d numeros desde '%s'.\n", n, ARCH_NUMS);
    printf("  Suma: %.2f | Promedio: %.2f | Min: %.2f | Max: %.2f\n", suma, suma / n, nums[0], nums[n - 1]);
    printf("  Ordenados: ");
    for (i = 0; i < n; i++) printf("%.2f%s", nums[i], (i + 1 < n) ? ", " : "\n");
    printf("\n[OK] Reporte guardado en '%s'.\n\n", ARCH_REP);

    free(nums);
    system("pause");
    return 0;
}
