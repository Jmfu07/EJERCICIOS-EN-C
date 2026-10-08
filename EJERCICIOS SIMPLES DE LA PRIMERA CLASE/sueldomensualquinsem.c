#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
/*  

ELABORADO 28/9/2026 -- JUAN MEDINA CI:31708027

2. Ingrese por teclado, Nombre, Edad, Monto de asignación mensuales y Monto de deducciones mensuales. Calcular el sueldo neto mensual, quincenal y semanal

*/


int main ()
{
    char nombre[100];
    int edad;
    float montasigmen, montdeducmen, snmen, snquin, snsem;

    printf ("CALCULADORA DE SUELDO NETO MENSUAL, QUINCENAL Y SEMANAL \n");
    printf ("Ingrese su nombre: ");
    scanf ("%s", &nombre);
    printf ("Ingrese su edad: ");
    scanf ("%d", &edad);
    printf ("Ingrese su Monto de Asignacion mensual: ");
    scanf ("%f", &montasigmen);
    printf ("Ingrese su Monto de Deduccion mensual: ");
    scanf ("%f", &montdeducmen);


    snmen = montasigmen - montdeducmen;
    snsem = ((snmen * 12) / 52);
    snquin= snmen/2;
    printf("EL TRABAJADOR %s, DE EDAD %d TIENE\n\n", nombre, edad);
    printf("EL SUELDO NETO MENSUAL DE %.2f\n", snmen);
    printf("EL SUELDO NETO SEMANAL DE %.2f\n", snsem);
    printf("EL SUELDO NETO QUINCENAL DE %.2f\n", snquin);
    
    getche();
}