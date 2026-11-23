#include <stdio.h> // Librería estándar para entrada/salida (printf)
#include <conio.h> // Librería para manejo de consola (getche)

/* Ejemplo 1: Modificación indirecta de una variable mediante un puntero */
main () {
    // Se reserva espacio en memoria para un entero y se inicializa en 10
    int Var1 = 10;
    
    // Se declara un puntero a entero y se le asigna la dirección de memoria de Var1 (&)
    int *Puntero = &Var1;
    
    // Imprime el valor original almacenado en Var1 (10)
    printf("Valor de la variable: %d", Var1);
    
    // Operador de indirección (*): accede a la dirección a la que apunta Puntero (Var1) y cambia su valor a 30
    *Puntero = 30;
    
    // Imprime nuevamente Var1 para comprobar que su valor fue modificado a 30
    printf("\n\n Valor de la variable: %d", Var1);
    
    // Pausa la ejecución del programa hasta que el usuario presione una tecla
    getche();
}
