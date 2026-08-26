#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// ELABORADO POR JUAN MEDINA 13/08/2026

/* 

El programa debe seleccionar a un atleta que terminara una maraton en un determinado tiempo

Estos tiempos son:
HOMBRES
150 MINUTOS PARA MENORES A 40 AÑOS
175 MINUTOS PARA MAYORES A 40 AÑOS

MUJERES 180 MINUTOS 

El usuario debe ingresar
sexo (H/M), edad y tiempo

el programa dira si esta seleccionado o no

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() 
{
    char sexo;
    int edad, tiempo;
    
    // 1. Validacion del Sexo (Solo 'H' o 'M')
    do 
    {
        system("cls");
        printf("Ingrese sexo del atleta (H/M): ");
        if (scanf(" %c", &sexo) != 1) 
        {
            limpiarBuffer();
            printf("Error: Ingrese 'H' para Hombre o 'M' para Mujer.\n");
            sleep (3);
            continue;
        }
        limpiarBuffer(); // Limpia sobrantes si el usuario escribió más de un carácter
        sexo = toupper(sexo);

        if (sexo != 'H' && sexo != 'M') 
        {
            printf("Error: Ingrese 'H' para Hombre o 'M' para Mujer.\n");
            sleep (3);
        }
    } while (sexo != 'H' && sexo != 'M');

    // 2. Validacion de la Edad (Entero positivo)
    do 
    {
        system("cls");
        printf("Ingrese la edad: ");
        if (scanf("%d", &edad) != 1 || edad <= 0) 
        {
            printf("Error: Ingrese un entero positivo.\n");
            limpiarBuffer();
            sleep (3);
        } 
        else 
        {
            limpiarBuffer();
            break;
        }
    } while (1);

    // 3. Validacion del Tiempo (Entero positivo)
    do 
    {
        system("cls");
        printf("Ingrese el tiempo en minutos: ");
        if (scanf("%d", &tiempo) != 1 || tiempo <= 0) 
        {
            printf("Error: Ingrese un tiempo válido en minutos.\n");
            limpiarBuffer();
            sleep (3);
        } 
        else 
        {
            limpiarBuffer();
            break;
        }
    } while (1);

    // Evaluación de la selección
    if (sexo == 'H') 
    {
        if ((edad < 40 && tiempo <= 150) || (edad >= 40 && tiempo <= 175)) 
        {
            system("cls");
            printf("\n>>> RESULTADO: Atleta SELECCIONADO <<<\n");
            sleep(10);    
        }
        else 
        {
            system("cls");
            printf("\n>>> RESULTADO: NO SELECCIONADO <<<\n");
            sleep(10);
        }
    } 
    else if (sexo == 'M') 
    {
        if (tiempo <= 180) 
        {
            system("cls");
            printf("\n>>> RESULTADO: Atleta SELECCIONADO <<<\n");
            sleep(10);
        }
        else 
        {
            system("cls");
            printf("\n>>> RESULTADO: NO SELECCIONADO <<<\n");
            sleep(10);
        }
    }
    

    
}
