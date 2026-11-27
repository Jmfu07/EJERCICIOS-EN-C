#define _WIN32_WINNT 0x0601
#include <windows.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>

/* ELABORADO POR JUAN MEDINA 20/08/2026
 * 5. Que pinte un tablero de ajedrez, los peones con la letra P, las torres con T,
 *    los caballos con C, los alfiles con A, el rey con R y la reina con M.
 */

int main(void)
{
    int i, j;
    const char inicio[8][9] = {
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
    for (i = 0; i < 8; i++) {
        printf("%d | ", 8 - i);
        for (j = 0; j < 8; j++) {
            printf("%c ", inicio[i][j]);
        }
        printf("| %d\n", 8 - i);
    }
    printf("   -----------------\n");
    printf("    A B C D E F G H\n");

    printf("\nPresione cualquier tecla para salir...");
    _getch();
    return 0;
}
