#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

/* ELABORADO POR JUAN MEDINA 20/08/2026
 * 19. Contar letras repetidas. Crear un programa que cree un array con 50 letras mayusculas aleatorias
 *     y que cuente cuantas veces aparece cada letra en el array.
 */

int main(void)
{
    char letras[50];
    int contador[26];
    int i, c;

    srand((unsigned int)time(NULL));

    do {
        system("cls");
        printf("=== CONTAR LETRAS REPETIDAS EN 50 LETRAS MAYUSCULAS ALEATORIAS ===\n\n");
        for (i = 0; i < 26; i++) contador[i] = 0;

        for (i = 0; i < 50; i++) {
            letras[i] = (char)('A' + rand() % 26);
            contador[letras[i] - 'A']++;
        }

        printf("Letras generadas:\n");
        for (i = 0; i < 50; i++) {
            printf("%c ", letras[i]);
            if ((i + 1) % 10 == 0) printf("\n");
        }

        printf("\nFrecuencia de cada letra:\n");
        for (i = 0; i < 26; i++) {
            printf("%c: %2d   ", 'A' + i, contador[i]);
            if ((i + 1) % 6 == 0) printf("\n");
        }
        printf("\n");

        printf("\nDesea generar otras 50 letras? (S/N): ");
        do {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); c = 0; }
        } while (c != 's' && c != 'S' && c != 'n' && c != 'N');
        printf("%c\n", c);
    } while (c == 's' || c == 'S');

    return 0;
}
