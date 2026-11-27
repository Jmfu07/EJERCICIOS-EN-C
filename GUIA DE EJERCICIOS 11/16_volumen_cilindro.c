/*
 * Guia: Estructuras Secuenciales - Ejercicio 16
 * Enunciado: Hacer un programa en C que calcule el volumen de un cilindro, dado
 * el radio y su altura, mostrar resultado en pantalla con sus respectivos
 * mensajes ( volumen = pi * r^2 * h ).
 */

#include <stdio.h>
#include <stdlib.h>

#define PI 3.14159265358979323846

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
    double radio, altura, volumen;

    printf("=== EJERCICIO 16: VOLUMEN DE UN CILINDRO ===\n\n");
    radio = leer_real_positivo("Ingrese el radio del cilindro: ");
    altura = leer_real_positivo("Ingrese la altura del cilindro: ");

    volumen = PI * radio * radio * altura;

    printf("\nResultados:\n");
    printf("Radio del cilindro   : %.2f\n", radio);
    printf("Altura del cilindro  : %.2f\n", altura);
    printf("Volumen del cilindro : %.4f\n\n", volumen);

    system("pause");
    return 0;
}
