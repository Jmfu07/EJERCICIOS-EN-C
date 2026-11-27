/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 20
 * Enunciado: Cargar en un vector de dimension 6 los nombres de unos alumnos.
 * Calcular:
 * 1) El promedio de los alumnos teniendo presente que ingresaron tres
 *    calificaciones diferentes en una sola variable de memoria.
 * 2) Almacenar este promedio en un vector.
 * 3) Averiguar cual alumno obtuvo el mayor promedio.
 * 4) Imprimir los requerimientos anteriores.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define NUM_ALUMNOS 6

void leer_cadena_no_vacia(const char *prompt, char *destino, size_t tam) {
    while (1) {
        size_t i, len;
        int tiene_texto = 0;

        printf("%s", prompt);
        if (!fgets(destino, (int)tam, stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        len = strlen(destino);
        while (len > 0 && (destino[len - 1] == '\n' || destino[len - 1] == '\r')) {
            destino[--len] = '\0';
        }
        for (i = 0; i < len; i++) {
            if (!isspace((unsigned char)destino[i])) {
                tiene_texto = 1;
                break;
            }
        }
        if (!tiene_texto) {
            printf("El nombre no puede estar vacio.\n");
            continue;
        }
        return;
    }
}

double leer_nota(const char *prompt) {
    char buffer[128];
    double valor;
    char *fin;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        valor = strtod(buffer, &fin);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == buffer || (*fin != '\n' && *fin != '\r' && *fin != '\0')) {
            printf("Entrada invalida. Ingrese una calificacion numerica.\n");
            continue;
        }
        if (valor < 0.0 || valor > 20.0) {
            printf("La calificacion debe estar entre 0 y 20.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    char NOM[NUM_ALUMNOS][80];
    double P[NUM_ALUMNOS];
    double nota; /* Una sola variable de memoria para leer las 3 calificaciones */
    int i, j, indice_mayor = 0;

    printf("=== PROGRESION I - EJERCICIO 20: VECTOR DE 6 ALUMNOS Y MAYOR PROMEDIO ===\n\n");

    for (i = 0; i < NUM_ALUMNOS; i++) {
        char prompt[120];
        P[i] = 0.0;

        printf("--- Alumno #%d de %d ---\n", i + 1, NUM_ALUMNOS);
        snprintf(prompt, sizeof(prompt), "Ingrese el nombre del alumno %d: ", i + 1);
        leer_cadena_no_vacia(prompt, NOM[i], sizeof(NOM[i]));

        for (j = 1; j <= 3; j++) {
            snprintf(prompt, sizeof(prompt), "  Ingrese la calificacion %d (0 a 20): ", j);
            nota = leer_nota(prompt);
            P[i] += nota;
        }
        P[i] /= 3.0;
        printf("\n");

        if (i == 0 || P[i] > P[indice_mayor]) {
            indice_mayor = i;
        }
    }

    printf("====================================================\n");
    printf("  Nro.  %-30s  Promedio(P)\n", "Nombre del Alumno (NOM)");
    printf("====================================================\n");
    for (i = 0; i < NUM_ALUMNOS; i++) {
        printf("   %d    %-30s     %6.2f\n", i + 1, NOM[i], P[i]);
    }
    printf("====================================================\n");
    printf("Alumno con mayor promedio: %s (Promedio: %.2f)\n\n",
           NOM[indice_mayor], P[indice_mayor]);

    system("pause");
    return 0;
}
