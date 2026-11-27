/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 1
 * Enunciado: Ingrese por teclado dos numeros y calcular la suma, resta y
 * multiplicacion entre ellos. Imprimir cada uno de los resultados acompanado
 * de un mensaje orientado al usuario.
 */

#include <stdio.h>
#include <stdlib.h>

double leer_numero(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un numero valido.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double n1, n2, suma, resta, multiplicacion;

    printf("=== PROGRESION I - EJERCICIO 1: SUMA, RESTA Y MULTIPLICACION ===\n\n");
    n1 = leer_numero("Ingrese el primer numero: ");
    n2 = leer_numero("Ingrese el segundo numero: ");

    suma = n1 + n2;
    resta = n1 - n2;
    multiplicacion = n1 * n2;

    printf("\nResultados de las operaciones:\n");
    printf("-> La suma de %.2f mas %.2f es: %.2f\n", n1, n2, suma);
    printf("-> La resta de %.2f menos %.2f es: %.2f\n", n1, n2, resta);
    printf("-> La multiplicacion de %.2f por %.2f es: %.2f\n\n", n1, n2, multiplicacion);

    system("pause");
    return 0;
}
