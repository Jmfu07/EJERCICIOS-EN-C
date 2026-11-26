/* EJERCICIO 3: TERNAS PITAGORICAS
 * Funcionamiento: se prueban catetos a, b e hipotenusa c <= 500 y se valida a^2 + b^2 = c^2.
 * Tipos: int para lados y ciclos.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int a, b, c, total = 0;
    printf("=== TERNAS PITAGORICAS (LADOS <= 500) ===\n");
    for (a = 1; a <= 500; a++) {
        for (b = a; b <= 500; b++) {
            for (c = b; c <= 500; c++) {
                if (a * a + b * b == c * c) {
                    printf("%3d, %3d, %3d\n", a, b, c);
                    total++;
                }
            }
        }
    }
    printf("Total de ternas encontradas: %d\n", total);
    system("pause");
    return 0;
}
