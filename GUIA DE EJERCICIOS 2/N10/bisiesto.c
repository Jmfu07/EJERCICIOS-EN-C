#define __USE_MINGW_ANSI_STDIO 1
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

int validacion (const char *mensaje)
{
    int num;
    do 
    {
        system("cls");
        printf("%s", mensaje);
        if (scanf("%d", &num) != 1 || num <= 0) 
        {
            printf("Error: Ingrese un numero entero positivo.\n");
            limpiarBuffer();
            sleep(3);
        } 
        else 
        {
            limpiarBuffer();
            return num;
        }
    } while (1);

}


int main()
{
    int mes, year, dias;
    do {
        mes = validacion("Ingrese el dia: ");
        if (mes > 12 ) printf ("Numero invalido."); sleep (2);
    } while (mes > 12);
    
    do {
        year = validacion("Ingrese el year: ");
        if (year > 100000 ) printf ("Numero invalido."); sleep (2);
    } while (year > 100000);
    
    

     // Determinar si el año es bisiesto
    int bisiesto = (year % 4 == 0 && year % 100 != 0) || (year % 400 == 0);

    // Determinar días del mes
    switch (mes) {
        case 1: case 3: case 5: case 7: case 8: case 10: case 12:
            dias = 31;
            break;
        case 4: case 6: case 9: case 11:
            dias = 30;
            break;
        case 2:
            dias = bisiesto ? 29 : 28;
            break;
    }

    printf("El mes %d del anio %d tiene %d dias.\n", mes, year, dias);

    sleep(10);
}