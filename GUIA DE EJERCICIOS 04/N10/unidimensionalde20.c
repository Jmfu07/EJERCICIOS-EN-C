#include <stdio.h>
#include <conio.h>

/* ELABORADO POR JUAN MEDINA 20/08/2026
 * 10. Realizar un programa que tenga un array unidimensional de 20 elementos de tipo
 * numerico entero y de nombre "pares". Cargar el array con los 20 primeros numeros
 * pares y mostrar el contenido del array en pantalla en grupos de 5.
 */

int main(void)
{
    int pares[20];
    int i;

    for (i = 0; i < 20; i++)
        pares[i] = (i + 1) * 2;

    printf("=== PRIMEROS 20 NUMEROS PARES (EN GRUPOS DE 5) ===\n\n");
    for (i = 0; i < 20; i++) {
        printf("%4d ", pares[i]);
        if ((i + 1) % 5 == 0)
            printf("\n");
    }

    printf("\nPresione cualquier tecla para salir...");
    _getch();
    return 0;
}
