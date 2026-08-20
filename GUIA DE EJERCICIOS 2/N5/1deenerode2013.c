#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/*

El programa debe aceptar una fecha por teclado en el dia y mes correspondiente al 2013
el programa debe decir desde ese dia cuantos dias han pasado desde el primero de enero 
de 2013 (1 de enero de 2013)


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


int main() {
    int dia, mes, i;
    int dias_por_mes[] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31};
    int dias_transcurridos = 0;
    do {
        dia = validacion("Ingrese el dia: ");
        if (dia > 31 ) printf ("Numero invalido."); sleep (2);
    } while (dia > 31);
    
    do {
        mes = validacion("Ingrese el mes: ");
        if (mes > 12 ) printf ("Numero invalido."); sleep (2);
    } while (mes > 12);
    
    // Sumar los días de los meses anteriores
    for (i = 0; i < mes - 1; i++) {
        dias_transcurridos += dias_por_mes[i];
    }

    // Sumar los días del mes actual
    dias_transcurridos += dia;

    system ("cls");
    printf("Han pasado %d dias desde el 1 de enero de 2013.\n", dias_transcurridos);
    sleep(10);
    return 0;
}
