/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 02:
 * Estadisticas de un archivo de texto: lineas, palabras, caracteres, vocales,
 * digitos y linea mas larga.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define ARCHIVO_DEF "texto_estadisticas.txt"

static void crear_ejemplo_si_no_existe(const char *nom) {
    FILE *f = fopen(nom, "r");
    if (f) { fclose(f); return; }
    f = fopen(nom, "w");
    if (!f) return;
    fprintf(f, "El lenguaje C fue creado en 1972 por Dennis Ritchie.\n");
    fprintf(f, "Con archivos en C podemos guardar datos de forma permanente.\n");
    fprintf(f, "Practicar con punteros y ficheros mejora la logica de programacion.\n");
    fclose(f);
}

static int es_vocal(int c) {
    c = tolower((unsigned char)c);
    return c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u';
}

int main(void) {
    char nombre[128], linea[512], mas_larga[512] = "";
    FILE *f;
    long lineas = 0, palabras = 0, chars = 0, vocales = 0, digitos = 0;
    size_t max_len = 0;

    crear_ejemplo_si_no_existe(ARCHIVO_DEF);
    printf("=== ARCHIVOS 02: ESTADISTICAS DE ARCHIVO DE TEXTO ===\n\n");
    printf("Nombre del archivo (Enter para usar '%s'): ", ARCHIVO_DEF);
    if (!fgets(nombre, sizeof nombre, stdin)) return 0;
    nombre[strcspn(nombre, "\r\n")] = '\0';
    if (nombre[0] == '\0') strcpy(nombre, ARCHIVO_DEF);

    f = fopen(nombre, "r");
    if (!f) {
        printf("[Error] No se pudo abrir '%s'.\n", nombre);
        system("pause");
        return 1;
    }

    while (fgets(linea, sizeof linea, f)) {
        size_t i, len;
        int en_palabra = 0;
        linea[strcspn(linea, "\r\n")] = '\0';
        len = strlen(linea);
        lineas++;
        chars += (long)len;
        if (len > max_len) {
            max_len = len;
            strcpy(mas_larga, linea);
        }
        for (i = 0; i < len; i++) {
            unsigned char ch = (unsigned char)linea[i];
            if (es_vocal(ch)) vocales++;
            if (isdigit(ch)) digitos++;
            if (isspace(ch)) en_palabra = 0;
            else if (!en_palabra) { en_palabra = 1; palabras++; }
        }
    }
    fclose(f);

    printf("\n--- ESTADISTICAS DE '%s' ---\n", nombre);
    printf("  Total de lineas     : %ld\n", lineas);
    printf("  Total de palabras   : %ld\n", palabras);
    printf("  Total de caracteres : %ld (sin saltos de linea)\n", chars);
    printf("  Vocales             : %ld\n", vocales);
    printf("  Digitos             : %ld\n", digitos);
    printf("  Linea mas larga (%u chars): \"%s\"\n\n", (unsigned)max_len, mas_larga);

    system("pause");
    return 0;
}
