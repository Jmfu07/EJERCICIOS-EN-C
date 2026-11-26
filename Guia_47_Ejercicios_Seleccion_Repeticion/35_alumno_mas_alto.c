/*
 * Ejercicio 35:
 * De los alumnos de una escuela se registra su apellido y nombre y su altura.
 * Disenar en Pseudocodigo un algoritmo que indique el nombre del alumno mas
 * alto y su altura (solo uno es el mas alto).
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

static double leer_altura(void) {
    double alt;
    char extra;
    while (1) {
        printf("Ingrese la altura en metros (ej. 1.75): ");
        if (scanf("%lf%c", &alt, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (alt > 0.3 && alt < 3.0) return alt;
            printf("Por favor ingrese una altura valida en metros (entre 0.30 y 3.00).\n");
        } else {
            printf("Entrada invalida. Ingrese un numero decimal.\n");
            limpiar_buffer();
        }
    }
}

static int desea_continuar(void) {
    char resp, extra;
    while (1) {
        printf("Desea registrar otro alumno? (s/n): ");
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
    char nombre[100], nombre_mas_alto[100] = "";
    double altura, max_altura = -1.0;
    int cantidad = 0;

    printf("=== EJERCICIO 35: ALUMNO MAS ALTO DE LA ESCUELA ===\n\n");

    do {
        printf("--- Alumno #%d ---\n", cantidad + 1);
        leer_texto("Ingrese Apellido y Nombre: ", nombre, sizeof(nombre));
        altura = leer_altura();

        if (cantidad == 0 || altura > max_altura) {
            max_altura = altura;
            strncpy(nombre_mas_alto, nombre, sizeof(nombre_mas_alto) - 1);
            nombre_mas_alto[sizeof(nombre_mas_alto) - 1] = '\0';
        }
        cantidad++;
        printf("\n");
    } while (desea_continuar());

    printf("\n=== RESULTADO ===\n");
    printf("Total de alumnos registrados: %d\n", cantidad);
    printf("Alumno mas alto: %s (Altura: %.2f m)\n\n", nombre_mas_alto, max_altura);

    system("pause");
    return 0;
}
