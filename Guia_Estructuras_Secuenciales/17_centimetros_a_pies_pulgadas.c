/*
 * Guia: Estructuras Secuenciales - Ejercicio 17
 * Enunciado: Dados ciertos centimetros como entrada de tipo decimal, escribir
 * equivalente a pies (entero) y pulgadas (decimal), dando las pulgadas con una
 * precision de un lugar decimal. Considere 2.54 cm por pulgada y 12 pulgadas
 * por pie. La salida podria ser: 100 cm son 3 pies y 39.4 pulgadas.
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
            printf("Entrada invalida. Ingrese un numero decimal valido.\n");
            continue;
        }
        if (valor < 0.0) {
            printf("La cantidad de centimetros no puede ser negativa.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double cm, pulgadas_totales, pulgadas_restantes;
    int pies;

    printf("=== EJERCICIO 17: CONVERSION DE CENTIMETROS A PIES Y PULGADAS ===\n\n");
    cm = leer_real_positivo("Ingrese la longitud en centimetros: ");

    pulgadas_totales = cm / 2.54;
    pies = (int)(pulgadas_totales / 12.0);
    pulgadas_restantes = pulgadas_totales - (pies * 12.0);

    printf("\nResultados:\n");
    printf("%.2f cm son %d pies y %.1f pulgadas (en total).\n", cm, pies, pulgadas_totales);
    printf("Desglose exacto: %d pies con %.1f pulgadas restantes.\n\n", pies, pulgadas_restantes);

    system("pause");
    return 0;
}
