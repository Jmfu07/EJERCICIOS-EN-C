#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escriba un programa en el que se declaren dos variables, a y b, se pida un valor para 
cada una de ellas y se intercambien dichos valores.

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}

double validacion (const char *mensaje)
{
    double num;
    do 
    {
        system("cls");
        printf("%s", mensaje);
        if (scanf("%lf", &num) != 1 || num <=0) 
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

int main ()
{

    double a, b, temp;

    a = validacion("Ingrese un numero: ");
    b = validacion("Ingrese otro numero:");

    system("cls");
    printf ("a = %.2lf\n", a);
    printf ("b = %.2lf\n", b);
    sleep(3);
    
    temp = a;
    a=b;
    b=temp;
    
    system("cls");
    printf ("- a = %.2lf\n", a);
    printf ("- b = %.2lf\n", b);
    sleep(3);
    


}
