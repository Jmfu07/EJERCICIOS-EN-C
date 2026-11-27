/*
 * Ejercicio 36:
 * Disenar un programa en C que permita calcular el promedio de un alumno
 * sabiendo que se ingresa por alumno la nota de 6 materias y su nombre. No se
 * sabe la cantidad de alumnos. Se pide mostrar:
 * NOMBRE DEL ALUMNO:        PROMEDIO:
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static void leer_texto(const char *mensaje, char *destino, size_t tam) {
    while (1) {
        printf("%s", mensaje);
        if (fgets(destino, (int)tam, stdin) != NULL) {
            size_t len = strlen(destino);
            while (len > 0 && (destino[len - 1] == '\n' || destino[len - 1] == '\r')) {
                destino[--len] = '\0';
            }
            if (len > 0) return;
        }
        printf("El nombre no puede estar vacio.\n");
    }
}

static double leer_nota(int num_materia) {
    double nota;
    char extra;
    while (1) {
        printf("  Nota de la materia %d (0 a 10): ", num_materia);
        if (scanf("%lf%c", &nota, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (nota >= 0.0 && nota <= 10.0) return nota;
            printf("  La nota debe estar entre 0 y 10.\n");
        } else {
            printf("  Entrada invalida. Ingrese un numero.\n");
            limpiar_buffer();
        }
    }
}

static int desea_continuar(void) {
    char resp, extra;
    while (1) {
        printf("Desea ingresar otro alumno? (s/n): ");
        if (scanf(" %c%c", &resp, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            resp = (char)tolower((unsigned char)resp);
            if (resp == 's') return 1;
            if (resp == 'n') return 0;
        } else {
            limpiar_buffer();
        }
        printf("Respuesta invalida. Ingrese 's' o 'n'.\n");
    }
}

int main(void) {
    char nombre[100];
    int i, cant_alumnos = 0;

    printf("=== EJERCICIO 36: PROMEDIO DE 6 MATERIAS POR ALUMNO ===\n\n");

    do {
        double suma = 0.0;
        cant_alumnos++;
        printf("--- Alumno #%d ---\n", cant_alumnos);
        leer_texto("Ingrese el nombre del alumno: ", nombre, sizeof(nombre));
        for (i = 1; i <= 6; i++) {
            suma += leer_nota(i);
        }
        printf("\nNOMBRE DEL ALUMNO: %-25s PROMEDIO: %.2f\n\n", nombre, suma / 6.0);
    } while (desea_continuar());

    printf("\nTotal de alumnos procesados: %d\n\n", cant_alumnos);
    system("pause");
    return 0;
}
