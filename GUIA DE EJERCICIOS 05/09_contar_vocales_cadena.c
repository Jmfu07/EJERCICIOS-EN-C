/* GUIA DE EJERCICIOS 5 - EJERCICIO 9:
 * Que lea una cadena y diga cuantas vocales hay.
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

int main(void)
{
    char cadena[128];
    int i, total = 0;

    printf("=== CONTAR VOCALES EN UNA CADENA ===\n\n");
    do {
        printf("Ingrese una cadena de texto: ");
        if (!fgets(cadena, sizeof(cadena), stdin)) return 0;
        cadena[strcspn(cadena, "\r\n")] = '\0';
    } while (cadena[0] == '\0');

    for (i = 0; cadena[i] != '\0'; i++) {
        char c = (char)tolower((unsigned char)cadena[i]);
        if (c == 'a' || c == 'e' || c == 'i' || c == 'o' || c == 'u')
            total++;
    }

    printf("La cadena contiene %d vocal(es).\n", total);
    system("pause");
    return 0;
}
