#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

Dado el numero del mes y el año (considerando si es bisiesto) saque
por pantalla el numero de dias del mes

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}


int main()
{
    int mes, anio, dias;

    printf("Ingrese el numero del mes (1-12): ");
    scanf("%d", &mes);
    limpiarBuffer();

    printf("Ingrese el año: ");
    scanf("%d", &anio);
    limpiarBuffer();

    if (mes < 1 || mes > 12) {
        printf("Mes invalido. Debe estar entre 1 y 12.\n");
        return 1;
    }

    switch (mes) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            dias = 31;
            break;
        case 4: case 6: case 9: case 11:
            dias = 30;
            break;
        case 2:
            if ((anio % 4 == 0 && anio % 100 != 0) || (anio % 400 == 0)) {
                dias = 29; // Año bisiesto
            } else {
                dias = 28; // Año no bisiesto
            }
            break;
        default:
            dias = -1; // Esto nunca debería ocurrir debido a la validación anterior
            break;
    }

    printf("El mes %d del año %d tiene %d días.\n", mes, anio, dias);

    return 0;
    sleep(5);
}