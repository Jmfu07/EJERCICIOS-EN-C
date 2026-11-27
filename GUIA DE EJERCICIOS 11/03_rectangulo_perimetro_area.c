/*
 * Guia: Estructuras Secuenciales - Ejercicio 3
 * Enunciado: Disenar un programa en C que calcule el perimetro y el area de un
 * rectangulo a partir del ingreso de su ancho y su largo, mostrar resultados con
 * sus respectivos mensajes (perimetro = 2 x ancho + 2 x largo, area = ancho x largo).
 */

#include <stdio.h>
#include <stdlib.h>

double leer_real_positivo(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un numero real valido.\n");
            continue;
        }
        if (valor <= 0.0) {
            printf("El valor debe ser mayor que cero.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double ancho, largo, perimetro, area;

    printf("=== EJERCICIO 3: PERIMETRO Y AREA DE UN RECTANGULO ===\n\n");
    ancho = leer_real_positivo("Ingrese el ancho del rectangulo: ");
    largo = leer_real_positivo("Ingrese el largo del rectangulo: ");

    perimetro = 2.0 * ancho + 2.0 * largo;
    area = ancho * largo;

    printf("\nResultados:\n");
    printf("Ancho     : %.2f\n", ancho);
    printf("Largo     : %.2f\n", largo);
    printf("Perimetro = %.2f\n", perimetro);
    printf("Area      = %.2f\n\n", area);

    system("pause");
    return 0;
}
