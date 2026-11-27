/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 04:
 * Copiar un archivo caracter a caracter, generar una copia en MAYUSCULAS y
 * otra copia con las lineas en orden invertido.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ORIG "original_ej04.txt"

static void crear_ejemplo(const char *nom) {
    FILE *f = fopen(nom, "r");
    if (f) { fclose(f); return; }
    f = fopen(nom, "w");
    if (!f) return;
    fprintf(f, "Primera linea del archivo de prueba.\n");
    fprintf(f, "Segunda linea con texto en minusculas.\n");
    fprintf(f, "Tercera linea para probar inversion.\n");
    fprintf(f, "Cuarta y ultima linea.\n");
    fclose(f);
}

static int copiar_transformado(const char *src, const char *dst, int mayus) {
    FILE *in = fopen(src, "r"), *out = fopen(dst, "w");
    int c;
    if (!in || !out) { if (in) fclose(in); if (out) fclose(out); return 0; }
    while ((c = fgetc(in)) != EOF)
        fputc(mayus ? toupper((unsigned char)c) : c, out);
    fclose(in); fclose(out);
    return 1;
}

static int copiar_lineas_invertidas(const char *src, const char *dst) {
    FILE *in = fopen(src, "r"), *out;
    char **lineas = NULL, buf[512];
    int n = 0, cap = 8, i;
    if (!in) return 0;
    lineas = (char **)malloc((size_t)cap * sizeof(char *));
    if (!lineas) { fclose(in); return 0; }

    while (fgets(buf, sizeof buf, in)) {
        if (n >= cap) {
            char **tmp;
            cap *= 2;
            tmp = (char **)realloc(lineas, (size_t)cap * sizeof(char *));
            if (!tmp) { for (i = 0; i < n; i++) free(lineas[i]); free(lineas); fclose(in); return 0; }
            lineas = tmp;
        }
        buf[strcspn(buf, "\r\n")] = '\0';
        lineas[n] = (char *)malloc(strlen(buf) + 1);
        if (!lineas[n]) { for (i = 0; i < n; i++) free(lineas[i]); free(lineas); fclose(in); return 0; }
        strcpy(lineas[n++], buf);
    }
    fclose(in);

    out = fopen(dst, "w");
    if (!out) { for (i = 0; i < n; i++) free(lineas[i]); free(lineas); return 0; }
    for (i = n - 1; i >= 0; i--) fprintf(out, "%s\n", lineas[i]);
    fclose(out);
    for (i = 0; i < n; i++) free(lineas[i]);
    free(lineas);
    return 1;
}

int main(void) {
    crear_ejemplo(ORIG);
    printf("=== ARCHIVOS 04: COPIAR, MAYUSCULAS E INVERTIR LINEAS ===\n\n");
    printf("1) Copia exacta      ('copia_exacta.txt')     : %s\n", copiar_transformado(ORIG, "copia_exacta.txt", 0) ? "OK" : "ERROR");
    printf("2) Copia MAYUSCULAS  ('copia_mayusculas.txt') : %s\n", copiar_transformado(ORIG, "copia_mayusculas.txt", 1) ? "OK" : "ERROR");
    printf("3) Lineas invertidas ('copia_invertida.txt')  : %s\n\n", copiar_lineas_invertidas(ORIG, "copia_invertida.txt") ? "OK" : "ERROR");
    system("pause");
    return 0;
}
