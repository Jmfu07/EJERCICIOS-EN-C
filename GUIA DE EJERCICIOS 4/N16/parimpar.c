#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

/* ELABORADO POR JUAN MEDINA 20/08/2026
 * 16. Ordenar pares e impares. Crear un programa llamado paresImpares que cree un array de 100
 * numeros aleatorios del 1 al 1000. Una vez creado, mostrar el contenido y despues organizarlo de
 * forma que esten juntos los elementos pares y los impares. Despues, volver a mostrar el array.
 */

int main(void)
{
    int numeros[100], auxiliar, i, j, c;

    srand((unsigned int)time(NULL));

    do {
        system("cls");
        printf("=== 100 NUMEROS ALEATORIOS (1 AL 1000): AGRUPAR PARES E IMPARES ===\n\n");
        for (i = 0; i < 100; i++)
            numeros[i] = rand() % 1000 + 1;

        printf("Arreglo original:\n");
        for (i = 0; i < 100; i++) {
            printf("%4d ", numeros[i]);
            if ((i + 1) % 10 == 0) printf("\n");
        }

        for (i = 0; i < 99; i++) {
            for (j = 0; j < 99 - i; j++) {
                if (numeros[j] % 2 != 0 && numeros[j + 1] % 2 == 0) {
                    auxiliar       = numeros[j];
                    numeros[j]     = numeros[j + 1];
                    numeros[j + 1] = auxiliar;
                }
            }
        }

        printf("\nArreglo organizado (pares primero e impares despues):\n");
        for (i = 0; i < 100; i++) {
            printf("%4d ", numeros[i]);
            if ((i + 1) % 10 == 0) printf("\n");
        }

        printf("\nDesea generar y ordenar otros 100 numeros? (S/N): ");
        do {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); c = 0; }
        } while (c != 's' && c != 'S' && c != 'n' && c != 'N');
        printf("%c\n", c);
    } while (c == 's' || c == 'S');

    return 0;
}
