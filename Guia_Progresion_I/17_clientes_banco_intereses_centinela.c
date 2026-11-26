/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 17
 * Enunciado: Ingrese por teclado los siguientes datos de un cliente de un Banco:
 * Nombre, edad, Monto Deposito, Tasa, Tiempo, se pide:
 * a) Calcular el interes a pagar por el monto depositado e imprimirlo con el
 *    nombre del cliente: I = (M * Tasa * Tiempo) / 100
 * b) Cuantos intereses se encuentran entre 2.000 Bsf. y 45.000 Bsf. Inclusive.
 * c) Sumar todos los montos depositados.
 * Utilizar el ciclo while usando un valor centinela.
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

void leer_cadena_no_vacia(const char *prompt, char *destino, size_t tam) {
    while (1) {
        size_t i, len;
        int tiene_texto = 0;

        printf("%s", prompt);
        if (!fgets(destino, (int)tam, stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        len = strlen(destino);
        while (len > 0 && (destino[len - 1] == '\n' || destino[len - 1] == '\r')) {
            destino[--len] = '\0';
        }
        for (i = 0; i < len; i++) {
            if (!isspace((unsigned char)destino[i])) {
                tiene_texto = 1;
                break;
            }
        }
        if (!tiene_texto) {
            printf("Este campo no puede estar vacio.\n");
            continue;
        }
        return;
    }
}

int leer_entero_rango(const char *prompt, int min, int max) {
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
        if (valor < min || valor > max) {
            printf("El valor debe estar entre %d y %d.\n", min, max);
            continue;
        }
        return (int)valor;
    }
}

double leer_real_positivo(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un valor numerico.\n");
            continue;
        }
        if (valor <= 0.0) {
            printf("El valor debe ser mayor que cero.\n");
            continue;
        }
        return valor;
    }
}

int main(void) {
    char nombre[100];
    int clientes = 0;
    int intereses_en_rango = 0;
    double suma_montos = 0.0;

    printf("=== PROGRESION I - EJERCICIO 17: CLIENTES DE BANCO (VALOR CENTINELA) ===\n");
    printf("Nota: Escriba 'FIN' o '0' como nombre del cliente para terminar.\n\n");

    leer_cadena_no_vacia("Ingrese el nombre del cliente ('FIN' para terminar): ",
                         nombre, sizeof(nombre));

    while (strcmp(nombre, "FIN") != 0 && strcmp(nombre, "fin") != 0 && strcmp(nombre, "0") != 0) {
        int edad;
        double monto, tasa, tiempo, interes;

        edad = leer_entero_rango("Edad del cliente (18 a 120): ", 18, 120);
        monto = leer_real_positivo("Monto depositado (Bs.F): ");
        tasa = leer_real_positivo("Tasa de interes (%%): ");
        tiempo = leer_real_positivo("Tiempo del deposito (en periodos/anos): ");

        interes = (monto * tasa * tiempo) / 100.0;
        suma_montos += monto;
        clientes++;

        if (interes >= 2000.0 && interes <= 45000.0) {
            intereses_en_rango++;
        }

        printf("\n-> Cliente: %s (Edad: %d) | Interes a pagar: %.2f Bs.F.\n\n",
               nombre, edad, interes);

        leer_cadena_no_vacia("Ingrese el nombre del siguiente cliente ('FIN' para terminar): ",
                             nombre, sizeof(nombre));
    }

    printf("\n=======================================================\n");
    printf("                 RESUMEN BANCARIO FINAL                \n");
    printf("=======================================================\n");
    printf("Total de clientes atendidos                     : %d\n", clientes);
    printf("Intereses entre 2.000 Bs.F y 45.000 Bs.F (incl.): %d\n", intereses_en_rango);
    printf("Suma total de todos los montos depositados      : %.2f Bs.F.\n", suma_montos);
    printf("=======================================================\n\n");

    system("pause");
    return 0;
}
