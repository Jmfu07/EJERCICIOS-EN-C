/* GUIA DE EJERCICIOS 5 - EJERCICIO 3:
 * Que pinte un tablero de ajedrez, los peones con la letra P, las torres con T,
 * los caballos con C, los alfiles con A, el rey con R y la reina con M.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int f, c;
    const char tab[8][9] = {
        "TCAMRACT",
        "PPPPPPPP",
        "........",
        "........",
        "........",
        "........",
        "PPPPPPPP",
        "TCAMRACT"
    };

    printf("=== TABLERO DE AJEDREZ ===\n\n");
    printf("    A B C D E F G H\n");
    printf("   -----------------\n");
    for (f = 0; f < 8; f++) {
        printf("%d | ", 8 - f);
        for (c = 0; c < 8; c++)
            printf("%c ", tab[f][c]);
        printf("| %d\n", 8 - f);
    }
    printf("   -----------------\n");
    printf("    A B C D E F G H\n\n");

    system("pause");
    return 0;
}
