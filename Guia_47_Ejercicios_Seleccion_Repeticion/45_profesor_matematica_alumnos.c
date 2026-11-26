/*
 * Ejercicio 45:
 * Un profesor de matematica de un establecimiento educativo registra de cada
 * alumno Nro de legajo, nombre y promedio. Segun el promedio desea saber
 * cuantos alumnos aprobaron (promedio mayor o igual a 7), cuantos rinden en
 * diciembre (promedio menor a 7 y mayor o igual a 4) y cuantos rinden examen
 * en marzo (promedio menor a 4). Ademas desea conocer el Nro de legajo y
 * nombre del alumno con mejor promedio (y tambien el de menor promedio).
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_legajo(void) {
    int leg;
    char extra;
    while (1) {
        printf("Nro de legajo (> 0): ");
        if (scanf("%d%c", &leg, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (leg > 0) return leg;
            printf("El legajo debe ser mayor que 0.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            limpiar_buffer();
        }
    }
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

static double leer_promedio(void) {
    double prom;
    char extra;
    while (1) {
        printf("Promedio del alumno (0 a 10): ");
        if (scanf("%lf%c", &prom, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (prom >= 0.0 && prom <= 10.0) return prom;
            printf("El promedio debe estar entre 0 y 10.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero.\n");
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
    int aprobaron = 0, diciembre = 0, marzo = 0, total = 0;
    int mejor_legajo = 0, menor_legajo = 0;
    char nombre[100], mejor_nombre[100] = "", menor_nombre[100] = "";
    double mejor_prom = -1.0, menor_prom = 11.0;

    printf("=== EJERCICIO 45: ESTADISTICA DE ALUMNOS DE MATEMATICA ===\n\n");

    do {
        int legajo;
        double prom;

        printf("--- Alumno #%d ---\n", total + 1);
        legajo = leer_legajo();
        leer_texto("Nombre del alumno: ", nombre, sizeof(nombre));
        prom = leer_promedio();

        if (prom >= 7.0) {
            aprobaron++;
        } else if (prom >= 4.0) {
            diciembre++;
        } else {
            marzo++;
        }

        if (total == 0 || prom > mejor_prom) {
            mejor_prom = prom;
            mejor_legajo = legajo;
            strncpy(mejor_nombre, nombre, sizeof(mejor_nombre) - 1);
            mejor_nombre[sizeof(mejor_nombre) - 1] = '\0';
        }
        if (total == 0 || prom < menor_prom) {
            menor_prom = prom;
            menor_legajo = legajo;
            strncpy(menor_nombre, nombre, sizeof(menor_nombre) - 1);
            menor_nombre[sizeof(menor_nombre) - 1] = '\0';
        }

        total++;
        printf("\n");
    } while (desea_continuar());

    printf("\n======================================================\n");
    printf("           INFORME DE ALUMNOS DE MATEMATICA           \n");
    printf("======================================================\n");
    printf("- Alumnos que aprobaron (promedio >= 7)        : %d\n", aprobaron);
    printf("- Alumnos que rinden en diciembre (4 <= p < 7) : %d\n", diciembre);
    printf("- Alumnos que rinden en marzo (promedio < 4)   : %d\n", marzo);
    printf("------------------------------------------------------\n");
    printf("Alumno con MEJOR promedio:\n");
    printf("  Legajo #%d - %s (Promedio: %.2f)\n", mejor_legajo, mejor_nombre, mejor_prom);
    printf("Alumno con MENOR promedio:\n");
    printf("  Legajo #%d - %s (Promedio: %.2f)\n", menor_legajo, menor_nombre, menor_prom);
    printf("======================================================\n\n");

    system("pause");
    return 0;
}
