/*
 * Ejercicio 10:
 * Escribir en Pseudocodigo y codificar en C un programa que lea 20
 * caracteres. Luego de la lectura indicar cuantas "a" se ingresaron, cuantas "e, i,
 * o, u".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

static char leer_caracter(int indice) {
    char linea[128];
    while (1) {
        printf("Ingrese el caracter %2d de 20: ", indice);
        if (fgets(linea, sizeof(linea), stdin) != NULL) {
            size_t len = strlen(linea);
            if (len > 0 && (linea[len - 1] == '\n' || linea[len - 1] == '\r')) {
                while (len > 0 && (linea[len - 1] == '\n' || linea[len - 1] == '\r')) {
                    linea[--len] = '\0';
                }
            } else {
                int c;
                while ((c = getchar()) != '\n' && c != EOF) {}
            }
            if (len == 1) {
                return linea[0];
            }
        }
        printf("Entrada invalida. Ingrese exactamente 1 caracter y presione Enter.\n");
    }
}

int main(void) {
    int cant_a = 0, cant_e = 0, cant_i = 0, cant_o = 0, cant_u = 0;
    int i;

    printf("=== EJERCICIO 10: CONTAR VOCALES EN 20 CARACTERES ===\n\n");

    for (i = 1; i <= 20; i++) {
        char c = (char)tolower((unsigned char)leer_caracter(i));
        switch (c) {
            case 'a': cant_a++; break;
            case 'e': cant_e++; break;
            case 'i': cant_i++; break;
            case 'o': cant_o++; break;
            case 'u': cant_u++; break;
            default: break;
        }
    }

    printf("\nResultados del conteo de vocales:\n");
    printf("- Cantidad de 'a': %d\n", cant_a);
    printf("- Cantidad de 'e': %d\n", cant_e);
    printf("- Cantidad de 'i': %d\n", cant_i);
    printf("- Cantidad de 'o': %d\n", cant_o);
    printf("- Cantidad de 'u': %d\n", cant_u);
    printf("- Total 'e, i, o, u': %d\n\n", cant_e + cant_i + cant_o + cant_u);

    system("pause");
    return 0;
}
