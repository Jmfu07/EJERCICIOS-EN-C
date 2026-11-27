/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 5
 * Enunciado: Sean dos cuadrados de lados L1 y L2 inscritos uno en otro.
 * Calcula el area de la zona comprendida entre ambos, utilizando para ello una
 * funcion (que se llamara AreaCuadrado) que devuelve el area de un cuadrado
 * cuyo lado se pasa como argumento.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

double AreaCuadrado(double lado) {
    return lado * lado;
}

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
            printf("El lado del cuadrado debe ser mayor que cero.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double L1, L2, area1, area2, area_entre_ambos;

    printf("=== PROGRESION I - EJERCICIO 5: AREA ENTRE DOS CUADRADOS ===\n\n");
    L1 = leer_real_positivo("Ingrese el lado L1 del primer cuadrado: ");
    L2 = leer_real_positivo("Ingrese el lado L2 del segundo cuadrado: ");

    area1 = AreaCuadrado(L1);
    area2 = AreaCuadrado(L2);
    area_entre_ambos = fabs(area1 - area2);

    printf("\nResultados:\n");
    printf("Area del cuadrado de lado L1 (%.2f) : %.2f\n", L1, area1);
    printf("Area del cuadrado de lado L2 (%.2f) : %.2f\n", L2, area2);
    printf("Area comprendida entre ambos         : %.2f\n\n", area_entre_ambos);

    system("pause");
    return 0;
}
