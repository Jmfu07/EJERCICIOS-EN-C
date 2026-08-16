#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// ELABORADO POR JUAN MEDINA 13/08/2026

/* 
El programa debe pedir una clave no mayor a 4 digitos, el usuario debe poder ingresar que opcion 
desea realizar como pedir su saldo y este ser impreso en la pantalla, todo debe estar validado para:

1- consulta de saldo
2- deposito
3- retiro

No puedes retirar mas de lo que tienes
No puedes depositar mas de lo que tienes


ESTE PROGRAMA SI TIENE UNA CLAVE GUARDADA EN UNA VARIABLE, CONSTA DE 3 INTENTOS PARA PONER LA CLAVE 1234 BIEN
AL TERCERO QUE FALLES SE CIERRA EL PROGRAMA

*/

void menu() {
    printf("\n--- BIENVENIDO AL BFC ---\n");
    printf("1. Consulta de saldo\n");
    printf("2. Deposito\n");
    printf("3. Retiro\n");
    printf("4. Salir\n");
}

void limpiarBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validacionmenu(int op1) {
    return (op1 > 0 && op1 <= 4);
}

int main() {

    int saldo = 500, deposito = 0, retiro = 0, op1 = 0;
    int clave = 0;
    
    // CONTROL DE INTENTOS Y CLAVE GUARDADA
    int claveCorrecta = 1234;     // La clave que otorga el acceso
    int intentos = 0;             // Contador de fallos actual
    int maxIntentos = 3;          // Limite maximo permitido

    // 1. VALIDACIÓN DE LA CLAVE CON LÍMITE DE 3 INTENTOS
    do 
    {
        system("cls");
        printf("--- CONTROL DE ACCESO BFC ---\n");
        printf("Intentos fallidos: %d/%d\n\n", intentos, maxIntentos);
        printf("Ingrese su clave secreta (maximo 4 digitos): ");
        
        if (scanf("%d", &clave) != 1) 
        {
            printf("Error: La clave debe ser numerica.\n");
            limpiarBuffer();
            intentos++; // Suma un fallo por ingresar letras
            sleep(3);
        } 
        else 
        {
            limpiarBuffer();
            
            // Validación de longitud (0 a 9999)
            if (clave < 0 || clave > 9999) 
            {
                printf("Error: La clave debe ser de maximo 4 digitos positivos.\n");
                intentos++; // Suma un fallo por longitud invalida
                sleep(3);
            }
            // Comprobación de veracidad
            else if (clave == claveCorrecta)
            {
                break; // Clave perfecta: rompe el bucle de intentos para ir al menú
            }
            else 
            {
                printf("Error: Clave incorrecta.\n");
                intentos++; // Suma un fallo por clave erronea
                sleep(3);
            }
        }

        // Si llega al límite, bloquea y aborta el programa
        if (intentos >= maxIntentos)
        {
            system("cls");
            printf("\n[ALERTA CRITICA] Ha superado los 3 intentos permitidos.\n");
            printf("El sistema se ha bloqueado por seguridad.\n");
            sleep(5);
            return 0; // Termina el programa inmediatamente
        }

    } while (intentos < maxIntentos);

    // Mensaje de éxito al romper el bucle correctamente
    system("cls");
    printf("\nClave aceptada con exito. Acceso concedido.\n");
    sleep(3);

    // 2. BUCLE PRINCIPAL DEL CAJERO (Solo se ejecuta si no se bloqueó arriba)
    do 
    {
        do 
        {
            system("cls");
            menu();
            printf("Ingrese su opcion: ");
            if (scanf("%d", &op1) != 1) 
            {
                printf("Error: Solo se permiten numeros.\n");
                limpiarBuffer();
                op1 = -1;
                sleep(3);
            } 
            else if (op1 < 1 || op1 > 4 )
            {
                printf("Error: Solo se permiten numeros del 1 al 4\n");
                sleep(3);
            }
            else 
            {
                clearerr(stdin); // Asegura el estado limpio del flujo de entrada
            }
            
        } while (!validacionmenu(op1));

        // 3. LÓGICA DE LAS OPCIONES
        switch (op1) 
        {
            case 1: // Consulta
                system("cls");
                printf("\nSu saldo actual es de: %d $\n", saldo);
                sleep(3);
                break;

            case 2: // Depósito
                do 
                {
                    system("cls");
                    printf("\nIngrese el monto a depositar: ");
                    if (scanf("%d", &deposito) != 1) 
                    {
                        printf("Error: Ingrese un monto valido.\n");
                        limpiarBuffer();
                        deposito = -1;
                        sleep(3);
                    } 
                    else 
                    {
                        limpiarBuffer();
                        if (deposito <= 0) 
                        {
                            printf("Error: El deposito debe ser mayor a 0.\n");
                            sleep(3);
                        }
                    }
                } while (deposito <= 0);
                
                saldo += deposito;
                printf("Deposito exitoso. Nuevo saldo: %d $\n", saldo);
                sleep(3);
                break;

            case 3: // Retiro
                do 
                {
                    system("cls");
                    printf("\nIngrese el monto a retirar: ");
                    if (scanf("%d", &retiro) != 1) 
                    {
                        printf("Error: Ingrese un monto valido.\n");
                        limpiarBuffer();
                        retiro = -1;
                        sleep(3);
                    } 
                    else 
                    {
                        limpiarBuffer();
                        if (retiro > saldo) 
                        {
                            printf("Error: Fondos insuficientes. Su saldo es de %d $\n", saldo);
                            sleep(3);
                        } 
                        else if (retiro <= 0) 
                        {
                            printf("Error: El retiro debe ser mayor a 0.\n");
                            sleep(3);
                        }
                    }
                } while (retiro <= 0 || retiro > saldo);

                saldo -= retiro;
                printf("Retiro exitoso. Puede retirar su dinero. Nuevo saldo: %d $\n", saldo);
                sleep(3);
                break;

            case 4: // Salir
                system("cls");
                printf("\nGracias por usar los servicios de BFC. ¡Hasta luego!\n");
                sleep(5);
        }

    } while (op1 != 4);

    return 0;
}