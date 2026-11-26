/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 18
 * Enunciado: En una eleccion realizada en un municipio, se presentaron tres
 * candidatos: Pedro, Maria y Juan. Por cada votante se elaboro un registro asi:
 * Cedula de Identidad y el valor del voto:
 * - Si voto por Pedro su valor es uno (1).
 * - Si voto por Maria su valor es dos (2).
 * - Si voto por Juan su valor es tres (3).
 * Se pide:
 * a) Cuantos votos obtuvo cada candidato.
 * b) Cuantos votos son nulos (voto nulo cualquier otro digito).
 * c) Quien gano las elecciones.
 * Utilizar el ciclo while con respuesta al operador.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

long leer_cedula(const char *prompt) {
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
            printf("Entrada invalida. Ingrese un numero de cedula valido (solo digitos).\n");
            continue;
        }
        if (valor <= 0) {
            printf("El numero de cedula debe ser positivo.\n");
            continue;
        }
        return valor;
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
            printf("Entrada invalida. Ingrese un numero entero para el voto.\n");
            continue;
        }
        return (int)valor;
    }
}

char leer_respuesta(const char *prompt) {
    char buffer[128];

    while (1) {
        size_t i = 0;
        char c;

        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        c = (char)toupper((unsigned char)buffer[i]);
        if (c != 'S' && c != 'N') {
            printf("Respuesta invalida. Ingrese 'S' o 'N'.\n");
            continue;
        }
        i++;
        while (buffer[i] == ' ' || buffer[i] == '\t') {
            i++;
        }
        if (buffer[i] != '\n' && buffer[i] != '\r' && buffer[i] != '\0') {
            printf("Ingrese unicamente una letra ('S' o 'N').\n");
            continue;
        }
        return c;
    }
}

int main(void) {
    char respuesta = 'S';
    int votos_pedro = 0, votos_maria = 0, votos_juan = 0, votos_nulos = 0;

    printf("=== PROGRESION I - EJERCICIO 18: ELECCIONES MUNICIPALES ===\n");
    printf("Candidatos: [1] Pedro | [2] Maria | [3] Juan | [Otro] Voto Nulo\n");

    while (respuesta == 'S') {
        long cedula;
        int voto;

        printf("\n--- Registro de Votante ---\n");
        cedula = leer_cedula("Ingrese la Cedula de Identidad: ");
        voto = leer_entero("Ingrese el valor del voto (1=Pedro, 2=Maria, 3=Juan): ");

        if (voto == 1) {
            votos_pedro++;
            printf("Voto registrado (C.I. %ld): Pedro\n", cedula);
        } else if (voto == 2) {
            votos_maria++;
            printf("Voto registrado (C.I. %ld): Maria\n", cedula);
        } else if (voto == 3) {
            votos_juan++;
            printf("Voto registrado (C.I. %ld): Juan\n", cedula);
        } else {
            votos_nulos++;
            printf("Voto registrado (C.I. %ld): VOTO NULO\n", cedula);
        }

        respuesta = leer_respuesta("\nHay otro votante para registrar? (S/N): ");
    }

    printf("\n============================================\n");
    printf("           RESULTADOS ELECTORALES           \n");
    printf("============================================\n");
    printf("a) Votos para Pedro : %d\n", votos_pedro);
    printf("   Votos para Maria : %d\n", votos_maria);
    printf("   Votos para Juan  : %d\n", votos_juan);
    printf("b) Votos Nulos      : %d\n", votos_nulos);
    printf("--------------------------------------------\n");

    if (votos_pedro == 0 && votos_maria == 0 && votos_juan == 0) {
        printf("c) Resultado: No hubo votos validos para ningun candidato.\n");
    } else if (votos_pedro > votos_maria && votos_pedro > votos_juan) {
        printf("c) Ganador de las elecciones: PEDRO con %d voto(s)!\n", votos_pedro);
    } else if (votos_maria > votos_pedro && votos_maria > votos_juan) {
        printf("c) Ganador de las elecciones: MARIA con %d voto(s)!\n", votos_maria);
    } else if (votos_juan > votos_pedro && votos_juan > votos_maria) {
        printf("c) Ganador de las elecciones: JUAN con %d voto(s)!\n", votos_juan);
    } else {
        printf("c) Resultado: EMPATE en el primer lugar.\n");
    }
    printf("============================================\n\n");

    system("pause");
    return 0;
}
