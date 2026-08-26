#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

Programa para calcular el indice de masa corporal (IMC) de una persona, 
para ello se debe solicitar al usuario su peso y estatura, posteriormente 
se debe calcular el IMC y mostrarlo en pantalla.

formula: IMC = peso / (altura cuadrada)
*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main ()
{
    long double peso, altura, imc;

    do 
    {
        system("cls");
        printf("Ingrese el peso (kg): ");
        if (scanf("%Lf", &peso) != 1 || peso <= 0) 
        {
            printf("Error: Dato invalido, solo numeros mayores a cero.\n");
            limpiarBuffer();
            sleep(3);
            continue;
        }
        limpiarBuffer();
        break;  

    } while (1);

    do 
    {
        system("cls");
        printf("Ingrese la altura (m): ");
        if (scanf("%Lf", &altura) != 1 || altura <= 0) 
        {
            printf("Error: Dato invalido, solo numeros mayores a cero.\n");
            limpiarBuffer();
            sleep(3);
            continue;
        }
        limpiarBuffer();
        break;  
        
    } while (1);

    imc = peso / pow(altura, 2);

    printf("Su IMC es de %.2LF", imc);
    sleep(10);
}

