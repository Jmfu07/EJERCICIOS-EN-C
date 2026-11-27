#include <stdio.h>
#include <conio.h>

/* Ejemplo 1: Modificacion indirecta de una variable mediante un puntero */
int main(void)
{
    /* Se reserva espacio en memoria para un entero y se inicializa en 10 */
    int Var1 = 10;

    /* Se declara un puntero a entero y se le asigna la direccion de memoria de Var1 (&) */
    int *Puntero = &Var1;

    /* Imprime el valor original almacenado en Var1 (10) */
    printf("Valor original de la variable Var1: %d\n", Var1);
    printf("Direccion de memoria de Var1:       %p\n", (void *)Puntero);

    /* Operador de indireccion (*): accede a la direccion a la que apunta Puntero (Var1) y cambia su valor a 30 */
    *Puntero = 30;

    /* Imprime nuevamente Var1 para comprobar que su valor fue modificado a 30 */
    printf("\nNuevo valor de la variable Var1 tras *Puntero = 30: %d\n", Var1);

    printf("\nPresione cualquier tecla para salir...");
    _getch();
    return 0;
}
