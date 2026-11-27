/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 19
 * Enunciado: Una persona tiene una cuenta corriente en un Banco Nacional con un
 * monto inicial de 520 Bolivares Fuertes. La persona ingresa por teclado su
 * clave de acceso compuesta de cuatro digitos y luego una opcion numerica para
 * realizar las siguientes operaciones:
 * 1.- Consulta saldo.
 * 2.- Retiro (El monto de retiro se ingresa por teclado).
 * 3.- Deposito (El monto de deposito se ingresa por teclado).
 * Efectuar un algoritmo para:
 * a) Ingresar la clave de acceso y validar.
 * b) Ingresar la opcion del usuario y realizar la transaccion solicitada (Imprima saldo).
 * c) Validar todas las opciones e incluso cuando esta sea incorrecta.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void leer_clave_4_digitos(const char *prompt, char *destino, size_t tam) {
    while (1) {
        size_t i, len;
        int solo_digitos = 1;

        printf("%s", prompt);
        if (!fgets(destino, (int)tam, stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        len = strlen(destino);
        while (len > 0 && (destino[len - 1] == '\n' || destino[len - 1] == '\r')) {
            destino[--len] = '\0';
        }
        if (len != 4) {
            printf("Error: La clave debe estar compuesta exactamente por 4 digitos.\n");
            continue;
        }
        for (i = 0; i < 4; i++) {
            if (!isdigit((unsigned char)destino[i])) {
                solo_digitos = 0;
                break;
            }
        }
        if (!solo_digitos) {
            printf("Error: La clave solo debe contener digitos numericos (0-9).\n");
            continue;
        }
        return;
    }
}

int leer_entero(const char *prompt) {
    char buffer[128];
    long valor;
    char *fin;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        valor = strtol(buffer, &fin, 10);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == buffer || (*fin != '\n' && *fin != '\r' && *fin != '\0')) {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            continue;
        }
        return (int)valor;
    }
}

double leer_monto_positivo(const char *prompt) {
    char buffer[128];
    double valor;
    char *fin;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        valor = strtod(buffer, &fin);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == buffer || (*fin != '\n' && *fin != '\r' && *fin != '\0')) {
            printf("Entrada invalida. Ingrese un monto numerico.\n");
            continue;
        }
        if (valor <= 0.0) {
            printf("El monto debe ser mayor que cero.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    double saldo = 520.0;
    char clave[16];
    int opcion = 0;

    printf("=== PROGRESION I - EJERCICIO 19: CUENTA CORRIENTE (SALDO INICIAL 520 Bs.F) ===\n\n");
    leer_clave_4_digitos("a) Ingrese su clave de acceso de 4 digitos: ", clave, sizeof(clave));
    printf("Clave validada correctamente.\n");

    while (opcion != 4) {
        printf("\n--- MENU DE OPERACIONES BANCARIAS ---\n");
        printf("1.- Consulta saldo\n");
        printf("2.- Retiro\n");
        printf("3.- Deposito\n");
        printf("4.- Salir\n");

        opcion = leer_entero("Seleccione una opcion: ");

        switch (opcion) {
            case 1:
                printf("\n[Consulta] Su saldo actual disponible es: %.2f Bs.F.\n", saldo);
                break;
            case 2: {
                double retiro = leer_monto_positivo("Ingrese el monto que desea retirar (Bs.F): ");
                if (retiro > saldo) {
                    printf("\n[Error] Fondos insuficientes. Su saldo actual es: %.2f Bs.F.\n", saldo);
                } else {
                    saldo -= retiro;
                    printf("\n[Retiro Exitoso] Ha retirado %.2f Bs.F. Nuevo saldo: %.2f Bs.F.\n",
                           retiro, saldo);
                }
                break;
            }
            case 3: {
                double deposito = leer_monto_positivo("Ingrese el monto que desea depositar (Bs.F): ");
                saldo += deposito;
                printf("\n[Deposito Exitoso] Ha depositado %.2f Bs.F. Nuevo saldo: %.2f Bs.F.\n",
                       deposito, saldo);
                break;
            }
            case 4:
                printf("\nGracias por utilizar el Banco Nacional. Saldo final: %.2f Bs.F.\n\n", saldo);
                break;
            default:
                printf("\n[Opcion Incorrecta] La opcion %d no es valida. Elija 1, 2, 3 o 4.\n", opcion);
                break;
        }
    }

    system("pause");
    return 0;
}
