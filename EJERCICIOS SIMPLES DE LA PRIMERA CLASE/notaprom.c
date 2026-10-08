#include <stdio.h>
#include <conio.h>
#include <stdlib.h>
#include <math.h>
/*  

ELABORADO 28/9/2026 -- JUAN MEDINA CI:31708027

3. Ingrese por teclado los siguientes datos de un alumno: Nombre, Edad, Nota uno, Nota dos, Nota. Se pide calcular, el promedio del estudiante e imprimirlo con su nombre

*/

int main ()
{
    char nombre[100];
    int edad;
    float n1, n2, n3, prom;

    printf ("CALCULADORA DE PROMEDIO DE 3 NOTAS\n");
    printf ("Ingrese su nombre: \n");
    scanf ("%s", &nombre);
    printf ("Ingrese su edad: \n");
    scanf ("%d", &edad);
    printf ("INGRESE SU PRIMERA NOTA: \n");
    scanf ("%f", &n1);
    printf ("INGRESE SU SEGUNDA NOTA: \n");
    scanf ("%f", &n2);
    printf ("INGRESE SU TERCERA NOTA: \n");
    scanf ("%f", &n3);

    prom = (n1+n2+n3)/3;
    printf ("El estudiante %s, de edad %d tiene un promedio de %.2f", nombre, edad, prom);
    getche();
}