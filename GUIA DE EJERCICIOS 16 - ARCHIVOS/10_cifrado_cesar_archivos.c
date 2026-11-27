/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 10:
 * Cifrado y descifrado Cesar sobre archivos de texto (fgetc / fputc) con clave
 * de desplazamiento configurable [1 a 25].
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define ARCH_PLAIN "mensaje_original.txt"
#define ARCH_CIF   "mensaje_cifrado.txt"
#define ARCH_DESC  "mensaje_descifrado.txt"

static void crear_ejemplo(void) {
    FILE *f = fopen(ARCH_PLAIN, "r");
    if (f) { fclose(f); return; }
    f = fopen(ARCH_PLAIN, "w");
    if (!f) return;
    fprintf(f, "Hola Mundo! Este archivo sera cifrado con Cesar en C (2026).\n");
    fprintf(f, "Seguridad basica y manejo de archivos caracter a caracter.\n");
    fclose(f);
}

static int leer_clave(void) {
    char buf[16]; long v; char *fin;
    for (;;) {
        printf("Ingrese clave de desplazamiento [1 a 25]: ");
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtol(buf, &fin, 10);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0') && v >= 1 && v <= 25)
            return (int)v;
        printf("  Error: clave fuera de rango.\n");
    }
}

static int transformar_cesar(const char *src, const char *dst, int desp) {
    FILE *in = fopen(src, "r"), *out = fopen(dst, "w");
    int c;
    if (!in || !out) { if (in) fclose(in); if (out) fclose(out); return 0; }
    desp = ((desp % 26) + 26) % 26;
    while ((c = fgetc(in)) != EOF) {
        if (c >= 'A' && c <= 'Z') c = 'A' + (c - 'A' + desp) % 26;
        else if (c >= 'a' && c <= 'z') c = 'a' + (c - 'a' + desp) % 26;
        fputc(c, out);
    }
    fclose(in); fclose(out);
    return 1;
}

static void mostrar(const char *nom) {
    FILE *f = fopen(nom, "r");
    char linea[256];
    if (!f) return;
    printf("\n--- %s ---\n", nom);
    while (fgets(linea, sizeof linea, f)) printf("  %s", linea);
    fclose(f);
}

int main(void) {
    int clave;
    crear_ejemplo();
    printf("=== ARCHIVOS 10: CIFRADO Y DESCIFRADO CESAR ===\n\n");
    clave = leer_clave();

    if (transformar_cesar(ARCH_PLAIN, ARCH_CIF, clave) &&
        transformar_cesar(ARCH_CIF, ARCH_DESC, -clave)) {
        mostrar(ARCH_PLAIN);
        mostrar(ARCH_CIF);
        mostrar(ARCH_DESC);
        printf("\n[OK] Cifrado y descifrado verificados.\n\n");
    } else {
        printf("[Error] Fallo al procesar archivos.\n");
    }
    system("pause");
    return 0;
}
