/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 03:
 * Buscar ocurrencias de una palabra y reemplazarla generando un nuevo archivo.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCH_IN  "origen_texto.txt"
#define ARCH_OUT "reemplazado_texto.txt"

static void crear_ejemplo_si_no_existe(const char *nom) {
    FILE *f = fopen(nom, "r");
    if (f) { fclose(f); return; }
    f = fopen(nom, "w");
    if (!f) return;
    fprintf(f, "C es rapido y C es portable.\n");
    fprintf(f, "Muchos sistemas operativos estan escritos en C.\n");
    fprintf(f, "Aprender C abre muchas puertas.\n");
    fclose(f);
}

static void leer_no_vacio(const char *msg, char *dest, int tam) {
    for (;;) {
        printf("%s", msg);
        if (!fgets(dest, tam, stdin)) exit(0);
        dest[strcspn(dest, "\r\n")] = '\0';
        if (dest[0] != '\0') return;
        printf("  Error: el texto no puede estar vacio.\n");
    }
}

int main(void) {
    char buscar[64], nuevo[64], linea[512];
    FILE *fin, *fout;
    int num_linea = 0, total = 0;
    size_t len_b;

    crear_ejemplo_si_no_existe(ARCH_IN);
    printf("=== ARCHIVOS 03: BUSCAR Y REEMPLAZAR PALABRA ===\n");
    printf("Archivo origen: %s -> Archivo salida: %s\n\n", ARCH_IN, ARCH_OUT);

    leer_no_vacio("Palabra/texto a buscar     : ", buscar, (int)sizeof buscar);
    leer_no_vacio("Reemplazar por             : ", nuevo,  (int)sizeof nuevo);
    len_b = strlen(buscar);

    fin = fopen(ARCH_IN, "r");
    fout = fopen(ARCH_OUT, "w");
    if (!fin || !fout) {
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        printf("[Error] No se pudieron abrir los archivos.\n");
        system("pause");
        return 1;
    }

    printf("\n--- Coincidencias encontradas ---\n");
    while (fgets(linea, sizeof linea, fin)) {
        const char *p = linea, *pos;
        int en_linea = 0;
        num_linea++;
        while ((pos = strstr(p, buscar)) != NULL) {
            fwrite(p, 1, (size_t)(pos - p), fout);
            fputs(nuevo, fout);
            p = pos + len_b;
            en_linea++;
            total++;
        }
        fputs(p, fout);
        if (en_linea > 0) printf("  Linea %2d: %d reemplazo(s)\n", num_linea, en_linea);
    }

    fclose(fin);
    fclose(fout);
    printf("\n[OK] Total de reemplazos: %d. Resultado guardado en '%s'.\n\n", total, ARCH_OUT);
    system("pause");
    return 0;
}
