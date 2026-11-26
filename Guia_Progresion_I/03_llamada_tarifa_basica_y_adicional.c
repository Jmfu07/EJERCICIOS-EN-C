/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 3
 * Enunciado: Ingrese por teclado los datos de una llamada telefonica, nombre,
 * numero de telefono, y cantidad de minutos de la llamada (valor entero).
 * Se pide calcular el monto de la llamada telefonica teniendo presente:
 * a.- Si la duracion es hasta tres (3) minutos inclusive, tarifa basica de 400 Bs.
 * b.- Si excede mas de tres (3) minutos, tarifa basica + 600 Bs por cada minuto adicional.
 * c.- Imprimir el monto de la llamada telefonica con el nombre del usuario.
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

void leer_telefono(const char *prompt, char *destino, size_t tam) {
    while (1) {
        size_t i, len;
        int valido = 1, digitos = 0;

        leer_cadena_no_vacia(prompt, destino, tam);
        len = strlen(destino);
        for (i = 0; i < len; i++) {
            unsigned char c = (unsigned char)destino[i];
            if (isdigit(c)) {
                digitos++;
            } else if (c != '-' && c != '+' && c != ' ') {
                valido = 0;
                break;
            }
        }
        if (!valido || digitos < 7) {
            printf("Ingrese un numero de telefono valido (al menos 7 digitos).\n");
            continue;
        }
        return;
    }
}

int leer_entero_positivo(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un numero entero de minutos.\n");
            continue;
        }
        if (valor <= 0) {
            printf("La duracion en minutos debe ser mayor que cero.\n");
            continue;
        }
        return (int)valor;
    }
}

int main(void) {
    char nombre[100];
    char telefono[40];
    int minutos;
    long monto;

    printf("=== PROGRESION I - EJERCICIO 3: COSTO DE LLAMADA TELEFONICA ===\n\n");
    leer_cadena_no_vacia("Ingrese el nombre del usuario: ", nombre, sizeof(nombre));
    leer_telefono("Ingrese el numero de telefono: ", telefono, sizeof(telefono));
    minutos = leer_entero_positivo("Ingrese la cantidad de minutos de la llamada: ");

    if (minutos <= 3) {
        monto = 400L;
    } else {
        monto = 400L + (long)(minutos - 3) * 600L;
    }

    printf("\nResultados:\n");
    printf("Usuario                  : %s\n", nombre);
    printf("Telefono                 : %s\n", telefono);
    printf("Minutos hablados         : %d\n", minutos);
    printf("Monto total de la llamada: %ld Bs.\n\n", monto);

    system("pause");
    return 0;
}
