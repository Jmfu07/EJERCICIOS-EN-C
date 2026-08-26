#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>

// ELABORADO POR JUAN MEDINA 16/08/2026

/* 
    En un supermercado hay una oferta al mayor para un producto donde ofrecen un 15% de descuento por la compra de mas de 3
3 docenas y 10% en caso contrario. Ademas por la compra de mas de 3 docenas se obsequia una unidad del producto por cada
docena en exceso sobre 3

Desarrolle:
- monto total de la compra
- monto del descuento
- monto a pagar
- numero de unidades de obsequio por la compra de cierta cantidad de docenas del producto

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main ()
{
    
    long double precio, descuento, total, montoPagar;
    int cantidadDocenas, unidadesObsequio;

    do
    {
        system("cls");
        printf("Ingrese el precio del producto: ");
        if (scanf("%Lf", &precio) != 1 || precio <= 0) 
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
        printf("Ingrese la cantidad de docenas del producto: ");
        if (scanf("%d", &cantidadDocenas) != 1 || cantidadDocenas <= 0 || cantidadDocenas < 3) 
        {
            printf("Error: Dato invalido, solo numeros mayores a cero y mayores o iguales a 3.\n");
            limpiarBuffer();
            sleep(3);
            continue;
        }
        limpiarBuffer();
        break;

    }while (1);

    if (cantidadDocenas > 3)
    {
        descuento = precio * cantidadDocenas * 0.15;
        unidadesObsequio = cantidadDocenas - 3;
    }
    else
    {
        descuento = precio * cantidadDocenas * 0.10;
        unidadesObsequio = 0;
    }

    system("cls");
    total = precio * cantidadDocenas;
    printf("Monto total de la compra: %.2Lf\n", total);
    printf("Monto del descuento: %.2Lf\n", descuento);
    montoPagar = total - descuento;
    printf("Monto a pagar: %.2Lf\n", montoPagar);
    printf("Numero de unidades de obsequio: %d\n", unidadesObsequio);
    sleep(10);
}
