/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicios 6 y 7
 * Enunciado:
 * 6. Una temperatura en grados centigrados C, puede ser convertida en su valor
 *    equivalente de la escala Fahrenheit de acuerdo a la siguiente formula:
 *    F = (9 / 5) C + 32
 * 7. Escribe un programa C que solicite una temperatura en grados centigrados
 *    por teclado y presente en pantalla la temperatura Fahrenheit equivalente.
 *    (ATENCION a la division 9/5).
 */

#include <stdio.h>
#include <stdlib.h>

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
            printf("Entrada invalida. Ingrese una temperatura numerica valida.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double C, F;

    printf("=== PROGRESION I - EJERCICIOS 6 Y 7: CENTIGRADOS A FAHRENHEIT ===\n\n");
    C = leer_real("Ingrese la temperatura en grados centigrados (C): ");

    /* Se utiliza 9.0 / 5.0 para realizar division en punto flotante y no entera */
    F = (9.0 / 5.0) * C + 32.0;

    printf("\nResultados:\n");
    printf("Temperatura en grados Centigrados : %.2f C\n", C);
    printf("Temperatura en grados Fahrenheit  : %.2f F\n\n", F);

    system("pause");
    return 0;
}
