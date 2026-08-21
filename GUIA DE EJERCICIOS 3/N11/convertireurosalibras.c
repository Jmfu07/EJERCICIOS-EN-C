#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Escriba un programa que covierta euros a dolares 
1 euro = 1,286$
y euros a libras esterlinas 
1 euro = 0,865 libras

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
        if (scanf("%d", &num) != 1 || num <=0) 
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
    double n1, n2, n3;
    double r1, r2;
    do
    {
        n1 = validacion("Que desea hacer?\n Convertir euros a dolares ----> 1 \n Convertir euros a libras -----> 2 \n ");
        if (n1 != 1 && n1 != 2) {printf ("Error: Solo numeros 1 y 2"); sleep(3);}
        else; continue;

    } while (n1 != 1 && n1 != 2); 

    if (n1 == 1) {
        n2 = validacion("Ingrese la cantidad de euros: ");
        r1 = n2/1.286;
        printf ("La conversion de euros a dolares es de %.2lf", r1);
        sleep(5);
    }
    
    else if (n1 == 2) {
        n3 = validacion("ingrese la cantidad de euros: ");
        r2 = n3/0.865;
        printf ("La conversion de euros a libras es de %.2lf", r2);
        sleep(5);
    } 
    
}
