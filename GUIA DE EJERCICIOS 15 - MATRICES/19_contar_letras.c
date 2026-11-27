/* EJERCICIO 19: CONTAR LETRAS REPETIDAS
   Funcionamiento: se generan 50 letras mayusculas. La posicion 0 del contador
   representa A, la 1 representa B y asi hasta Z.
   Tipos: char para letras; int para contador, indices y cantidades.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int main(void)
{
    char letras[50];
    int contador[26] = {0};
    int i;

    srand((unsigned int) time(NULL));
    for (i = 0; i < 50; i++)
    {
        letras[i] = (char) ('A' + rand() % 26);
        contador[letras[i] - 'A']++;
    }

    printf("Letras generadas:\n");
    for (i = 0; i < 50; i++)
    {
        printf("%c ", letras[i]);
        if ((i + 1) % 10 == 0)
        {
            printf("\n");
        }
    }

    printf("\nCantidad por letra:\n");
    for (i = 0; i < 26; i++)
    {
        printf("%c: %d\n", 'A' + i, contador[i]);
    }

    system("pause");
    return 0;
}
