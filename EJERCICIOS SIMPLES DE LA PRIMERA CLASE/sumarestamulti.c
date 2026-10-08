#include <stdio.h>
#include <conio.h>
#include <stdlib.h>

/*  

ELABORADO 28/9/2026 -- JUAN MEDINA CI:31708027

1. Ingrese por teclado los valores numéricos enteros y calcule la suma resta y multiplicación de ellos (Imprimir los resultados con los mensajes respectivos)

*/

int main ()
{
    int a, b, sum, res, mul;

    printf("CALCULADORA DE SUMA, RESTA Y MULTIPLICACION DE DOS NUMEROS \n");
    printf("INGRESE UN NUMERO: \n");
    scanf("%d", &a);
    printf("INGRESE OTRO NUMERO: \n");
    scanf("%d", &b);

    sum=a+b;
    res=a-b;
    mul=a*b;

    printf("\nLa suma de %d y %d es %d", a, b, sum);
    printf("\nLa resta de %d y %d es %d", a, b, res);
    printf("\nLa multiplicacion de %d y %d es %d", a, b, mul);
    
    
    getche();
}