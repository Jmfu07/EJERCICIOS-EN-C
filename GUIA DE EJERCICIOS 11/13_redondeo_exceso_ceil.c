/*
 * Guia: Estructuras Secuenciales - Ejercicio 13
 * Enunciado: Realizar un programa en C que lea un numero real y lo redondee por
 * exceso al siguiente entero mayor. Mostrar resultados con sus respectivos mensajes.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double leer_real(const char *prompt) {
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
        return valor;
    }
}

int main(void) {
    double numero, redondeado;

    printf("=== EJERCICIO 13: REDONDEO POR EXCESO (CEIL) ===\n\n");
    numero = leer_real("Ingrese un numero real: ");
    redondeado = ceil(numero);

    printf("\nResultados:\n");
    printf("Numero real ingresado                    : %.4f\n", numero);
    printf("Redondeado por exceso al entero superior : %.0f\n\n", redondeado);

    system("pause");
    return 0;
}
