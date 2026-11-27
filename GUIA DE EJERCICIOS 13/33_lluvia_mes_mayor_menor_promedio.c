/*
 * Ejercicio 33:
 * Se ingresa por teclado la cantidad de agua caida, en milimetros dia a dia
 * durante un mes. Se pide determinar el dia de mayor lluvia, el de menor y el
 * promedio.
 */

#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_dias_mes(void) {
    int dias;
    char extra;
    while (1) {
        printf("Ingrese la cantidad de dias del mes (28 a 31): ");
        if (scanf("%d%c", &dias, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (dias >= 28 && dias <= 31) {
                return dias;
            }
            printf("Por favor ingrese una cantidad de dias entre 28 y 31.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            limpiar_buffer();
        }
    }
}

static double leer_lluvia_dia(int dia) {
    double mm;
    char extra;
    while (1) {
        printf("Agua caida en el dia %2d (mm >= 0): ", dia);
        if (scanf("%lf%c", &mm, &extra) == 2 && (extra == '\n' || extra == '\r')) {
            if (mm >= 0.0) {
                return mm;
            }
            printf("La cantidad de agua caida no puede ser negativa.\n");
        } else {
            printf("Entrada invalida. Ingrese un numero valido.\n");
            limpiar_buffer();
        }
    }
}

int main(void) {
    int dias, i;
    int dia_mayor = 1, dia_menor = 1;
    double max_lluvia = 0.0, min_lluvia = 0.0, suma = 0.0;

    printf("=== EJERCICIO 33: LLUVIA CAIDA EN UN MES (MAYOR, MENOR Y PROMEDIO) ===\n\n");

    dias = leer_dias_mes();
    printf("\n");

    for (i = 1; i <= dias; i++) {
        double mm = leer_lluvia_dia(i);
        suma += mm;
        if (i == 1 || mm > max_lluvia) {
            max_lluvia = mm;
            dia_mayor = i;
        }
        if (i == 1 || mm < min_lluvia) {
            min_lluvia = mm;
            dia_menor = i;
        }
    }

    printf("\n=== RESUMEN DEL MES (%d DIAS) ===\n", dias);
    printf("- Dia de mayor lluvia : Dia %d (%.2f mm)\n", dia_mayor, max_lluvia);
    printf("- Dia de menor lluvia : Dia %d (%.2f mm)\n", dia_menor, min_lluvia);
    printf("- Total acumulado     : %.2f mm\n", suma);
    printf("- Promedio diario     : %.2f mm\n\n", suma / dias);

    system("pause");
    return 0;
}
