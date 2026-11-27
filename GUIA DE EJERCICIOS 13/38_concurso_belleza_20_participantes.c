/*
 * Ejercicio 38:
 * De las 20 participantes de un concurso de belleza se van registrando de
 * uno en uno los siguientes datos:
 * - Apellido y Nombre
 * - Puntos por inteligencia
 * - Puntos por Cultura general
 * - Puntos por Belleza
 * Se necesita informar por pantalla:
 * - Apellido y nombre de la concursante de mayor puntaje general
 * - Puntaje acumulado por todas las participantes en Cultura general, en
 *   Inteligencia y en Belleza
 * - De los tres puntajes acumulados en el item anterior cual fue el menor.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define TOTAL_PARTICIPANTES 20

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

static double leer_puntos(const char *mensaje) {
    double pts;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &pts, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (pts >= 0.0) return pts;
            printf("El puntaje no puede ser negativo.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    char nombre[100], mejor_concursante[100] = "";
    double mejor_puntaje = -1.0;
    double acum_inteligencia = 0.0, acum_cultura = 0.0, acum_belleza = 0.0;
    int i;

    printf("=== EJERCICIO 38: CONCURSO DE BELLEZA (20 PARTICIPANTES) ===\n\n");

    for (i = 1; i <= TOTAL_PARTICIPANTES; i++) {
        double p_int, p_cul, p_bel, total_part;

        printf("--- Participante %d de %d ---\n", i, TOTAL_PARTICIPANTES);
        leer_texto("Apellido y Nombre: ", nombre, sizeof(nombre));
        p_int = leer_puntos("Puntos por Inteligencia (>= 0): ");
        p_cul = leer_puntos("Puntos por Cultura general (>= 0): ");
        p_bel = leer_puntos("Puntos por Belleza (>= 0): ");

        total_part = p_int + p_cul + p_bel;
        acum_inteligencia += p_int;
        acum_cultura += p_cul;
        acum_belleza += p_bel;

        if (i == 1 || total_part > mejor_puntaje) {
            mejor_puntaje = total_part;
            strncpy(mejor_concursante, nombre, sizeof(mejor_concursante) - 1);
            mejor_concursante[sizeof(mejor_concursante) - 1] = '\0';
        }
        printf("Puntaje general de %s: %.2f\n\n", nombre, total_part);
    }

    printf("\n======================================================\n");
    printf("              RESULTADOS DEL CONCURSO                 \n");
    printf("======================================================\n");
    printf("Concursante de mayor puntaje general:\n");
    printf("-> %s (%.2f puntos)\n\n", mejor_concursante, mejor_puntaje);

    printf("Puntaje acumulado por todas las participantes:\n");
    printf("- Cultura general : %.2f\n", acum_cultura);
    printf("- Inteligencia    : %.2f\n", acum_inteligencia);
    printf("- Belleza         : %.2f\n\n", acum_belleza);

    if (acum_cultura <= acum_inteligencia && acum_cultura <= acum_belleza) {
        printf("El menor puntaje acumulado fue en: CULTURA GENERAL (%.2f)\n\n", acum_cultura);
    } else if (acum_inteligencia <= acum_cultura && acum_inteligencia <= acum_belleza) {
        printf("El menor puntaje acumulado fue en: INTELIGENCIA (%.2f)\n\n", acum_inteligencia);
    } else {
        printf("El menor puntaje acumulado fue en: BELLEZA (%.2f)\n\n", acum_belleza);
    }

    system("pause");
    return 0;
}
