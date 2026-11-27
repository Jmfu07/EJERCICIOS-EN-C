/*
 * Guia: Estructuras Secuenciales - Ejercicio 2
 * Enunciado: Disenar un programa en C que calcule el area de un triangulo a
 * partir del ingreso de su base y altura, imprimiendo los resultados con sus
 * respectivos mensajes (area = 1/2(base x altura)).
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
    double base, altura, area;

    printf("=== EJERCICIO 2: AREA DE UN TRIANGULO ===\n\n");
    base = leer_real_positivo("Ingrese la base del triangulo: ");
    altura = leer_real_positivo("Ingrese la altura del triangulo: ");

    area = 0.5 * (base * altura);

    printf("\nResultados:\n");
    printf("Base   : %.2f\n", base);
    printf("Altura : %.2f\n", altura);
    printf("Area del triangulo (1/2 * base * altura) = %.2f\n\n", area);

    system("pause");
    return 0;
}
