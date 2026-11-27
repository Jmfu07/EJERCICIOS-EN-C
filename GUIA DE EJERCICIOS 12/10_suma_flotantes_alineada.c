/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 10
 * Enunciado: Escribe un programa que lea del teclado dos numeros en coma
 * flotante cuya parte entera se asume que no supera 3 digitos, y muestre en
 * pantalla su suma ajustada a la derecha. Por ejemplo, si los numeros son 23.6
 * y 187.54 el programa debe mostrar:
 *      23.60
 *     187.54
 *  ----------
 *     211.14
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double leer_flotante_3_digitos(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un numero en coma flotante.\n");
            continue;
        }
        if (fabs(valor) >= 1000.0) {
            printf("La parte entera no debe superar 3 digitos (-999.99 a 999.99).\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double n1, n2, suma;

    printf("=== PROGRESION I - EJERCICIO 10: SUMA DE FLOTANTES ALINEADA A LA DERECHA ===\n\n");
    n1 = leer_flotante_3_digitos("Ingrese el primer numero (parte entera <= 3 digitos): ");
    n2 = leer_flotante_3_digitos("Ingrese el segundo numero (parte entera <= 3 digitos): ");

    suma = n1 + n2;

    printf("\nSuma ajustada a la derecha:\n\n");
    printf("%10.2f\n", n1);
    printf("%10.2f\n", n2);
    printf("----------\n");
    printf("%10.2f\n\n", suma);

    system("pause");
    return 0;
}
