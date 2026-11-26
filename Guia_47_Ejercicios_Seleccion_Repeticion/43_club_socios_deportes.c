/*
 * Ejercicio 43:
 * En un club se registran de uno en uno los siguientes datos de sus socios:
 * - Numero de socio
 * - Apellido y nombre
 * - Edad
 * - Tipo de deporte que practica (1 tenis, 2 rugby/paddle, 3 voley, 4 hockey, 5 futbol)
 * Disenar en Pseudocodigo y luego un programa en C que permita emitir por
 * pantalla cuantos socios practican tenis y cuantos paddle (y cada deporte) y el
 * promedio de edad de los jugadores de cada deporte.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_entero_rango(const char *mensaje, int min, int max) {
    int valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%d%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor >= min && valor <= max) return valor;
            printf("El valor debe estar entre %d y %d.\n", min, max);
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
        printf("El texto no puede estar vacio.\n");
    }
}

static int desea_continuar(void) {
    char resp, extra;
    while (1) {
        printf("Desea registrar otro socio? (s/n): ");
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
    const char *nombres_deportes[5] = {
        "Tenis", "Rugby / Paddle", "Voley", "Hockey", "Futbol"
    };
    int cant_por_deporte[5] = {0, 0, 0, 0, 0};
    int suma_edad_deporte[5] = {0, 0, 0, 0, 0};
    char nombre[100];
    int i;

    printf("=== EJERCICIO 43: REGISTRO DE SOCIOS Y DEPORTES DEL CLUB ===\n\n");

    do {
        int num_socio = leer_entero_rango("Numero de socio (1 a 999999): ", 1, 999999);
        int edad, deporte;
        (void)num_socio;

        leer_texto("Apellido y Nombre: ", nombre, sizeof(nombre));
        edad = leer_entero_rango("Edad (1 a 120): ", 1, 120);
        printf("Deportes: 1=Tenis, 2=Rugby/Paddle, 3=Voley, 4=Hockey, 5=Futbol\n");
        deporte = leer_entero_rango("Tipo de deporte (1 a 5): ", 1, 5);

        cant_por_deporte[deporte - 1]++;
        suma_edad_deporte[deporte - 1] += edad;
        printf("\n");
    } while (desea_continuar());

    printf("\n=== INFORME DEL CLUB POR DEPORTE ===\n");
    printf("- Socios que practican Tenis          : %d\n", cant_por_deporte[0]);
    printf("- Socios que practican Rugby / Paddle : %d\n\n", cant_por_deporte[1]);

    printf("%-18s %-12s %-18s\n", "DEPORTE", "CANTIDAD", "PROMEDIO DE EDAD");
    printf("--------------------------------------------------\n");
    for (i = 0; i < 5; i++) {
        if (cant_por_deporte[i] > 0) {
            double prom = (double)suma_edad_deporte[i] / cant_por_deporte[i];
            printf("%-18s %-12d %.2f anos\n", nombres_deportes[i], cant_por_deporte[i], prom);
        } else {
            printf("%-18s %-12d Sin inscriptos\n", nombres_deportes[i], 0);
        }
    }
    printf("\n");

    system("pause");
    return 0;
}
