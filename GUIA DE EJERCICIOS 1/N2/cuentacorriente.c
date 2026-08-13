#include <stdio.h>
#include <stdlib.h>

// ELABORADO POR JUAN MEDINA 13/08/2026

/* 
El programa debe pedir una clave no mayor a 4 digitos, el usuario debe poder ingresar que opcion 
desea realizar como pedir su saldo y este ser impreso en la pantalla, todo debe estar validado para:

- consulta de saldo
- deposito
- retiro

No puedes retirar mas de lo que tienes
No puedes depositar mas de lo que tienes

*/

void menu ()
{
    printf ("BIENVENIDO AL BFC");
    printf ("1. Consulta de saldo");
    printf ("2. Deposito ");
    printf ("3. Retiro");
}

// Función para vaciar letras del búfer
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validacionmenu(int op1)
{
    return (op1 > 0 && op1 < 3);
}



int main 
{

    int saldo=0, deposito=0, retiro=0; op1=0, op2=0; 

}