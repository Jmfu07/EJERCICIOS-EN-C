/* GUIA DE EJERCICIOS 5 - EJERCICIO 8:
 * Que lea una cadena y la muestre al reves.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char cadena[128];
    int i, len;

    printf("=== MOSTRAR UNA CADENA AL REVES ===\n\n");
    do {
        printf("Ingrese una cadena de texto: ");
        if (!fgets(cadena, sizeof(cadena), stdin)) return 0;
        cadena[strcspn(cadena, "\r\n")] = '\0';
    } while (cadena[0] == '\0');

    len = (int)strlen(cadena);
    printf("Cadena al reves: ");
    for (i = len - 1; i >= 0; i--)
        putchar(cadena[i]);
    printf("\n");

    system("pause");
    return 0;
}
