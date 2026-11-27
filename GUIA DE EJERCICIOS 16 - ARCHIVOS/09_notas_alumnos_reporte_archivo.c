/* ============================================================================
 * GUIA DE EJERCICIOS 16 (ARCHIVOS) - EJERCICIO 09:
 * Leer registros de alumnos desde archivo (nombre;n1;n2;n3), calcular
 * promedios, aprobados/reprobados, mejor y peor alumno, y generar reporte .txt.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define ARCH_IN  "alumnos_notas.txt"
#define ARCH_OUT "reporte_alumnos.txt"

static void crear_datos_ejemplo(void) {
    FILE *f = fopen(ARCH_IN, "r");
    if (f) { fclose(f); return; }
    f = fopen(ARCH_IN, "w");
    if (!f) return;
    fprintf(f, "Ana Perez;18.5;17.0;19.0\n");
    fprintf(f, "Luis Gomez;8.0;9.5;7.0\n");
    fprintf(f, "Maria Silva;14.0;15.5;16.0\n");
    fprintf(f, "Carlos Ruiz;11.0;9.0;10.0\n");
    fprintf(f, "Sofia Diaz;19.5;20.0;19.0\n");
    fclose(f);
}

int main(void) {
    FILE *fin, *fout;
    char linea[200], nom[64], mejor_nom[64] = "", peor_nom[64] = "";
    double n1, n2, n3, prom, suma_gen = 0.0, max_p = -1.0, min_p = 21.0;
    int total = 0, aprob = 0, reprob = 0;

    crear_datos_ejemplo();
    fin = fopen(ARCH_IN, "r");
    fout = fopen(ARCH_OUT, "w");
    if (!fin || !fout) {
        if (fin) fclose(fin);
        if (fout) fclose(fout);
        printf("[Error] No se pudieron abrir los archivos.\n");
        system("pause");
        return 1;
    }

    fprintf(fout, "=====================================================================\n");
    fprintf(fout, "| %-22s | %5s | %5s | %5s | %6s | %-10s |\n", "ALUMNO", "N1", "N2", "N3", "PROM", "ESTADO");
    fprintf(fout, "=====================================================================\n");

    printf("=== ARCHIVOS 09: REPORTE DE NOTAS DESDE ARCHIVO ===\n\n");
    while (fgets(linea, sizeof linea, fin)) {
        if (sscanf(linea, "%63[^;];%lf;%lf;%lf", nom, &n1, &n2, &n3) == 4) {
            const char *est;
            prom = (n1 + n2 + n3) / 3.0;
            est = (prom >= 10.0) ? "APROBADO" : "REPROBADO";
            if (prom >= 10.0) aprob++; else reprob++;
            suma_gen += prom;
            if (prom > max_p) { max_p = prom; strcpy(mejor_nom, nom); }
            if (prom < min_p) { min_p = prom; strcpy(peor_nom, nom); }
            total++;
            fprintf(fout, "| %-22s | %5.2f | %5.2f | %5.2f | %6.2f | %-10s |\n", nom, n1, n2, n3, prom, est);
            printf("  %-22s -> Promedio: %5.2f (%s)\n", nom, prom, est);
        }
    }

    fprintf(fout, "=====================================================================\n");
    if (total > 0) {
        fprintf(fout, "Total alumnos: %d | Aprobados: %d | Reprobados: %d | Promedio general: %.2f\n",
                total, aprob, reprob, suma_gen / total);
        fprintf(fout, "Mejor alumno : %s (%.2f)\nPeor alumno  : %s (%.2f)\n",
                mejor_nom, max_p, peor_nom, min_p);
        printf("\nPromedio general: %.2f | Mejor: %s (%.2f) | Peor: %s (%.2f)\n",
               suma_gen / total, mejor_nom, max_p, peor_nom, min_p);
    }
    fclose(fin);
    fclose(fout);
    printf("[OK] Tabla exportada a '%s'.\n\n", ARCH_OUT);
    system("pause");
    return 0;
}
