#include <stdio.h>
#include <conio.h>

/*  

ELABORADO 8/10/2026 -- JUAN MEDINA CI:31708027

    Una persona tiene una cuenta corriente en un banco nacional con un monto inicial de 520000,00 bs soberanos. La persona 
ingresa por teclado su clave de acceso compuesta de cuatro digitos y luego una opcion numerica para realizar las siguientes 
operaciones:

1- consulta de saldo
2- deposito
3- retiro

EFECTUAR UN PROGRAMA EN LENGUAJE C PARA:
A- Ingresar la clave de acceso y validar.
B- Ingresar una opcion del usuario y realizar la transaccion solicitada (imprimir resultado).
C- Utilizar el ciclo while con respuesta del operador. 
D- Validar todas las opciones e incluso cuando esta sea incorrecta.


No puedes retirar mas de lo que tienes
No puedes depositar mas de lo que tienes


*/

main ()
{
	int  op1=0, clave=1234;
	double saldo = 520000, deposito = 0, retiro = 0;
	do
	{
		printf("\nIngrese su clave secreta: ");
		scanf("%d", &clave);
		if (clave == 1234) printf ("\nCLAVE CORRECTA\n");
		else  printf ("\nERROR: CLAVE INCORRECTA\n\n");
	} while (clave != 1234);
	do
	{
		printf("\n--- BIENVENIDO AL BFC ---\n");
    	printf("1. Consulta de saldo\n");
    	printf("2. Deposito\n");
    	printf("3. Retiro\n");
    	printf("4. Salir\n");
		scanf("%d", &op1); 
        if (op1 < 1 || op1 > 4) 
		{
            printf("\n[ERROR] Opcion invalida (Debe ser del 1 al 4).\n\n");
            op1 = 0;
		}        
        if (op1==1) printf("\nSu saldo actual es de: %.2f $\n", saldo);
		if (op1==2)
		{
			do
			{
                printf("\nIngrese el monto a depositar: ");
                scanf("%lf", &deposito); 
                if (deposito <= 0) printf("Error: Ingrese un monto valido.\n");
            } while (deposito <= 0);
            
            saldo += deposito;
            printf("Deposito exitoso. Nuevo saldo: %.2f $\n", saldo);
		}
		if (op1==3)
		{
			do
			{
				printf("\nIngrese el monto a retirar: ");
		        scanf("%lf", &retiro); 
		        if (retiro <= 0 || retiro > saldo) printf("Error: Ingrese un monto valido.\n");
		    } while (retiro <= 0 || retiro > saldo);

        	saldo -= retiro;
    	    printf("Retiro exitoso. Nuevo saldo: %.2f $\n", saldo);
		}
		if (op1==4)
		{
			printf("\nGRACIAS POR USAR EL CAJERO\n");
		}
	} while (op1 != 4);
	getche();
}