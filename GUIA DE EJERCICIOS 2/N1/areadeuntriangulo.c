#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/* 
    Escribir un programa que calcule el área de un triángulo, para ello se debe solicitar al usuario la base 
    y la altura del triángulo, posteriormente se debe calcular el área y mostrarla en pantalla.
*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() 
{
    long double base, altura, area;
    do 
    {
        system("cls");
        printf("Ingrese la base del triángulo (mayor que 0): ");
        if (scanf("%Lf", &base) != 1 || base <= 0) 
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
        printf("Ingrese la altura del triángulo (mayor que 0): ");
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
    
    area = (base * altura) / 2;

    system("cls");
    printf("El área del triángulo es: %.2Lf\n", area);

    sleep(10);
}