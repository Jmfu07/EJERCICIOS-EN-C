/*
 * Ejercicio 37:
 * Del registro de partes meteorologico por cada dia se registra la fecha,
 * temperatura maxima y temperatura minima. Disenar en Pseudocodigo un
 * algoritmo que permita informar:
 * - el dia mas frio y cual fue esa temperatura
 * - el dia mas calido y cual fue esa temperatura
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
        printf("La fecha no puede estar vacia.\n");
    }
}

static double leer_temperatura(const char *mensaje) {
    double temp;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &temp, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (temp >= -90.0 && temp <= 70.0) return temp;
            printf("Ingrese una temperatura razonable (-90 a 70 C).\n");
        } else {
            printf("Entrada invalida. Ingrese un numero.\n");
            limpiar_buffer();
        }
    }
}

static int desea_continuar(void) {
    char resp, extra;
    while (1) {
        printf("Desea registrar otro parte diario? (s/n): ");
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
    char fecha[64], fecha_mas_fria[64] = "", fecha_mas_calida[64] = "";
    double temp_max, temp_min;
    double record_frio = 0.0, record_calor = 0.0;
    int dias = 0;

    printf("=== EJERCICIO 37: PARTES METEOROLOGICOS (DIA MAS FRIO Y MAS CALIDO) ===\n\n");

    do {
        printf("--- Parte Meteorologico #%d ---\n", dias + 1);
        leer_texto("Ingrese la fecha (ej. 15/07/2026): ", fecha, sizeof(fecha));
        while (1) {
            temp_min = leer_temperatura("Temperatura minima (C): ");
            temp_max = leer_temperatura("Temperatura maxima (C): ");
            if (temp_max >= temp_min) {
                break;
            }
            printf("Error: la temperatura maxima no puede ser menor que la minima.\n");
        }

        if (dias == 0 || temp_min < record_frio) {
            record_frio = temp_min;
            strncpy(fecha_mas_fria, fecha, sizeof(fecha_mas_fria) - 1);
            fecha_mas_fria[sizeof(fecha_mas_fria) - 1] = '\0';
        }
        if (dias == 0 || temp_max > record_calor) {
            record_calor = temp_max;
            strncpy(fecha_mas_calida, fecha, sizeof(fecha_mas_calida) - 1);
            fecha_mas_calida[sizeof(fecha_mas_calida) - 1] = '\0';
        }

        dias++;
        printf("\n");
    } while (desea_continuar());

    printf("\n=== INFORME METEOROLOGICO (%d DIA(S) REGISTRADOS) ===\n", dias);
    printf("- Dia mas frio  : %s con %.2f C\n", fecha_mas_fria, record_frio);
    printf("- Dia mas calido: %s con %.2f C\n\n", fecha_mas_calida, record_calor);

    system("pause");
    return 0;
}
