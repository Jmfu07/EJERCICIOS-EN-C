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

int main ()
{
    int dia, mes, diasTranscurridos = 0;

    printf("Ingrese el dia (1-31): ");
    scanf("%d", &dia);
    limpiarBuffer();

    printf("Ingrese el mes (1-12): ");
    scanf("%d", &mes);
    limpiarBuffer();

    if (mes < 1 || mes > 12) {
        printf("Mes invalido. Debe estar entre 1 y 12.\n");
        return 1;
    }

    // Validar el día según el mes
    if ((mes == 1 || mes == 3 || mes == 5 || mes == 7 || mes == 8 || mes == 10 || mes == 12) && (dia < 1 || dia > 31)) {
        printf("Dia invalido para el mes %d. Debe estar entre 1 y 31.\n", mes);
        return 1;
    } else if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && (dia < 1 || dia > 30)) {
        printf("Dia invalido para el mes %d. Debe estar entre 1 y 30.\n", mes);
        return 1;
    } else if (mes == 2 && (dia < 1 || dia > 28)) { // Febrero en año no bisiesto
        printf("Dia invalido para el mes %d. Debe estar entre 1 y 28.\n", mes);
        return 1;
    }

    // Calcular los días transcurridos desde el primero de enero de 2013
    switch (mes) {
        case 1:
            diasTranscurridos = dia - 1; // Enero
            break;
            sleep(5);
        case 2:
            diasTranscurridos = 31 + (dia - 1); // Enero + Febrero
            break;
            sleep(5);
        case 3:
            diasTranscurridos = 31 + 28 + (dia - 1); // Enero + Febrero + Marzo
            break;
            sleep(5);
        case 4:
            diasTranscurridos = 31 + 28 + 31 + (dia - 1); // Enero + Febrero + Marzo + Abril
            break;
            sleep(5);
        case 5:
            diasTranscurridos = 31 + 28 + 31 + 30 + (dia - 1); // Enero + Febrero + Marzo + Abril + Mayo
            break;
            sleep(5);   
        case 6:
            diasTranscurridos = 31 + 28 + 31 + 30 + 31 + (dia - 1); // Enero + Febrero + Marzo + Abril + Mayo + Junio
            break;
            sleep(5);
        case 7:
            diasTranscurridos = 31 + 28 + 31 + 30 + 31 + 30 + (dia - 1); // Enero + Febrero + Marzo + Abril + Mayo + Junio + Julio
            break;  
            sleep(5);
        case 8:
            diasTranscurridos = 31 + 28 + 31 + 30 + 31 + 30 + 31 + (dia - 1); // Enero + Febrero + Marzo + Abril + Mayo + Junio + Julio + Agosto
            break;  
            sleep(5);
        case 9:
            diasTranscurridos = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + (dia - 1); // Enero + Febrero + Marzo + Abril + Mayo + Junio + Julio + Agosto + Septiembre
            break;
            sleep(5);
        case 10:
            diasTranscurridos = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + (dia - 1); // Enero + Febrero + Marzo + Abril + Mayo + Junio + Julio + Agosto + Septiembre + Octubre
            break;
            sleep(5);
        case 11:
            diasTranscurridos = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + (dia - 1); // Enero + Febrero + Marzo + Abril + Mayo + Junio + Julio + Agosto + Septiembre + Octubre + Noviembre
            break;
            sleep(5);
        case 12:
            diasTranscurridos = 31 + 28 + 31 + 30 + 31 + 30 + 31 + 31 + 30 + 31 + 30 + (dia - 1); // Enero + Febrero + Marzo + Abril + Mayo + Junio + Julio + Agosto + Septiembre + Octubre + Noviembre + Diciembre
            break;
            sleep(5);
        default:
            printf("Mes invalido. Debe estar entre 1 y 12.\n");
            return 1;
    }
}