#include <stdio.h>
#include <stdlib.h>

// ELABORADO POR JUAN MEDINA 13/08/2026

/* 
El programa debe pedir una clave no mayor a 4 digitos, el usuario debe poder ingresar que opcion 
desea realizar como pedir su saldo y este ser impreso en la pantalla, todo debe estar validado para:

1- consulta de saldo
2- deposito
3- retiro

No puedes retirar mas de lo que tienes
No puedes depositar mas de lo que tienes

*/
void menu ()
{
    printf ("BIENVENIDO AL BFC\n");
    printf ("1. Consulta de saldo\n");
    printf ("2. Deposito \n");
    printf ("3. Retiro\n");
}

// Función para vaciar letras del búfer
void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validacionmenu(int op1)
{
    return (op1 > 0 && op1 <= 3 );
}

int validacioncontr (int op2)
{

    
}

int main()
{

    int saldo=0, deposito=0, retiro=0, op1=0, op2=0; 
    menu();
    do  
    {
        printf("\nIngrese su opcion: ");
   
        // Si scanf devuelve 0, el usuario escribió una letra
        if (scanf("%d", &op1) != 1) 
            {
                printf("Error: Solo se permiten numeros.\n");
                limpiarBuffer(); // Limpia la letra atrapada para evitar el bucle infinito
                op1 = -1;     // Asignamos un valor inválido para forzar a repetirse
            } 
            else  
            {
                limpiarBuffer(); // Limpia el 'Enter' sobrante
            }

    } while (!validacionmenu(op1));




    if (op1 == 1)
    {
        printf ("Su saldo es de: %d ", saldo);
    }
}