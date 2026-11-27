#include <stdio.h>
#include <conio.h>

/* Ejemplo 2: Lectura de direccion de memoria y contenido mediante un puntero */
int main(void)
{
    /* Declaracion multiple: dos enteros normales (P y P2) en 50, y un puntero a entero (*PP) */
    int P = 50, P2 = 50, *PP;
    (void)P2;

    /* Se guarda la direccion de memoria de la variable P dentro del puntero PP */
    PP = &P;

    /* Imprime la direccion de memoria almacenada en PP en formato hexadecimal (%p) */
    printf("Direccion de P: %p\n", (void *)PP);

    /* Desreferencia el puntero (*PP) para acceder y mostrar el valor guardado en esa direccion (50) */
    printf("Valor de P:     %d\n", *PP);

    printf("\nPresione cualquier tecla para salir...");
    _getch();
    return 0;
}
