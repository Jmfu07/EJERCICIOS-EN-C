/*
 * Ejercicio 4:
 * Disenar un programa en C que calcule la longitud de la circunferencia y el
 * area del circulo de radio dado.
 */

#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static double leer_double_positivo(const char *mensaje) {
    double valor;
    char extra;
    while (1) {
        printf("%s", mensaje);
        if (scanf("%lf%c", &valor, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (valor > 0.0) {
                return valor;
            }
            printf("El radio debe ser mayor que cero.\n");
        } else {
            printf("Entrada invalida. Por favor ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    double radio, longitud, area;

    printf("=== EJERCICIO 4: LONGITUD Y AREA DE UN CIRCULO ===\n\n");

    radio = leer_double_positivo("Ingrese el radio del circulo (> 0): ");
    longitud = 2.0 * PI * radio;
    area = PI * radio * radio;

    printf("\nPara un circulo de radio %.2f:\n", radio);
    printf("- Longitud de la circunferencia: %.4f\n", longitud);
    printf("- Area del circulo:              %.4f\n\n", area);

    system("pause");
    return 0;
}
