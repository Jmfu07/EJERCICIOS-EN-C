#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escribe un programa que calcule el interes que se obtiene por un determinado dinero
depositado en un banco, el programa debe preguntar por el capital inicial y por el 
tipo de interes

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

double tipotiempo(double tiptemp)
{
    double tiempo;
    double t;
    do{
        if (tiptemp == 1){ 
            t = validacion ("Ingrese el tiempo a cobrar: ");
            return tiempo = t;
            continue;
        }
        else if (tiptemp == 2){
            t = validacion ("Ingrese el tiempo a cobrar: ");
            return tiempo = t/12;
            continue;
        }
        else if(tiptemp == 3){
            t = validacion ("Ingrese el tiempo a cobrar: ");
            return tiempo = t/365;
            continue;
        }
        else; printf ("Formato invalido"); sleep(10); continue;
    } while (tiptemp != 1 && tiptemp != 2 && tiptemp != 3);
            
}

int main ()
{
    double capital, op, interes, tiptemp, tiempo, t, r1, r2; 

    capital = validacion ("Ingrese el capital: ");
    do {
        op = validacion ("Ingrese el tipo de interes: \n - Simple ----> 1\n - Compuesto ----> 2\n");

        if (op == 1) 
        { 
            interes = validacion ("Ingrese el porcentaje de interes: ");
            tiptemp = validacion ("Ingrese el tipo de tiempo: \n 1- Anos\n 2- Meses\n 3- Dias\n");
            tiempo = tipotiempo(tiptemp);
            r1 = capital * (interes / 100) * tiempo;
            system ("cls");
            printf ("RESULTADO: \n");
            printf ("El interes es de: %.2lf$\n", r1);
            printf ("Monto total acumulado: %.2lf$\n", r1+capital);
            sleep(10);
            break;
        }
        else if (op == 2) 
        { 
            interes = validacion ("Ingrese el porcentaje de interes: ");
            tiptemp = validacion ("Ingrese el tipo de tiempo: \n 1- Anos\n 2- Meses\n 3-Dias\n");
            tiempo = tipotiempo(tiptemp);
            r2 = capital * (pow(1+(interes/100),tiempo));
            system ("cls");
            printf ("RESULTADO: \n");
            printf ("El interes es de: %.2lf$\n", r2-capital);
            printf ("Monto total acumulado: %.2lf$\n", r2);
            sleep(10);
            break;
        }
    } while (op != 1 && op != 2);
    
}

