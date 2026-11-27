/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 01:
 * Crear, escribir, leer y agregar lineas a un archivo de texto (modos w, r, a)
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

#define ARCHIVO_DEF "notas_texto.txt"

static int leer_opcion(char min, char max) {
    for (;;) {
        int c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c >= min && c <= max) { printf("%c\n", c); return c - '0'; }
    }
}

static int leer_entero_rango(const char *msg, int min, int max) {
    char buf[16]; int len, c, v;
    for (;;) {
        len = 0; printf("%s [%d a %d]: ", msg, min, max);
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (len > 0) break; continue; }
            if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
            if (c >= '0' && c <= '9' && len < 4) { buf[len++] = (char)c; putchar(c); }
        }
        buf[len] = '\0'; putchar('\n');
        v = (int)strtol(buf, NULL, 10);
        if (v >= min && v <= max) return v;
        printf("  Error: fuera de rango (%d a %d).\n", min, max);
    }
}

static void leer_linea(const char *msg, char *dest, int tam) {
    printf("%s", msg);
    if (!fgets(dest, tam, stdin)) { dest[0] = '\0'; return; }
    dest[strcspn(dest, "\r\n")] = '\0';
}

static void escribir_lineas(const char *nombre, const char *modo) {
    FILE *f = fopen(nombre, modo);
    int n, i;
    char linea[256];
    if (!f) { printf("  [Error] No se pudo abrir '%s'.\n", nombre); return; }
    n = leer_entero_rango("Cantidad de lineas a escribir", 1, 20);
    for (i = 0; i < n; i++) {
        printf("  Linea %d: ", i + 1);
        leer_linea("", linea, (int)sizeof linea);
        fprintf(f, "%s\n", linea);
    }
    fclose(f);
    printf("  [OK] %d linea(s) guardadas en '%s'.\n", n, nombre);
}

static void mostrar_archivo(const char *nombre) {
    FILE *f = fopen(nombre, "r");
    char linea[256];
    int num = 0;
    if (!f) { printf("  [Aviso] El archivo '%s' aun no existe.\n", nombre); return; }
    printf("\n--- CONTENIDO DE '%s' ---\n", nombre);
    while (fgets(linea, sizeof linea, f)) {
        printf("%3d | %s", ++num, linea);
    }
    if (num == 0) printf("  (Archivo vacio)\n");
    printf("-----------------------------\n");
    fclose(f);
}

int main(void) {
    int op;
    do {
        printf("\n=== ARCHIVOS 01: CREAR, LEER Y AGREGAR TEXTO (%s) ===\n", ARCHIVO_DEF);
        printf("1. Crear / Sobrescribir archivo (modo \"w\")\n");
        printf("2. Agregar lineas al final (modo \"a\")\n");
        printf("3. Leer y mostrar contenido (modo \"r\")\n");
        printf("0. Salir\nOpcion: ");
        op = leer_opcion('0', '3');
        if (op == 1) escribir_lineas(ARCHIVO_DEF, "w");
        else if (op == 2) escribir_lineas(ARCHIVO_DEF, "a");
        else if (op == 3) mostrar_archivo(ARCHIVO_DEF);
    } while (op != 0);
    return 0;
}
