/* EJERCICIO 18: RESERVA DE ASIENTOS
   Funcionamiento: la matriz 25x4 usa 0 para asiento libre y 1 para ocupado.
   El menu permite reservar, cancelar y mostrar el estado de la sala.
   Tipos: int para matriz, filas, columnas, indices y opcion.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int asientos[25][4] = {0};
    int fila;
    int columna;
    int opcion;
    int i;
    int j;

    do
    {
        printf("\n--- RESERVA DE ASIENTOS ---\n");
        printf("1. Reservar\n2. Cancelar\n3. Mostrar sala\n0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
                printf("Fila (1 a 25): ");
                scanf("%d", &fila);
                printf("Columna (1 a 4): ");
                scanf("%d", &columna);
                if (fila < 1 || fila > 25 || columna < 1 || columna > 4)
                {
                    printf("Asiento invalido.\n");
                }
                else if (asientos[fila - 1][columna - 1] == 1)
                {
                    printf("El asiento ya esta ocupado.\n");
                }
                else
                {
                    asientos[fila - 1][columna - 1] = 1;
                    printf("Asiento reservado.\n");
                }
                break;

            case 2:
                printf("Fila (1 a 25): ");
                scanf("%d", &fila);
                printf("Columna (1 a 4): ");
                scanf("%d", &columna);
                if (fila < 1 || fila > 25 || columna < 1 || columna > 4)
                {
                    printf("Asiento invalido.\n");
                }
                else if (asientos[fila - 1][columna - 1] == 0)
                {
                    printf("El asiento ya esta libre.\n");
                }
                else
                {
                    asientos[fila - 1][columna - 1] = 0;
                    printf("Reserva cancelada.\n");
                }
                break;

            case 3:
                printf("O = ocupado, L = libre\n");
                for (i = 0; i < 25; i++)
                {
                    printf("Fila %2d: ", i + 1);
                    for (j = 0; j < 4; j++)
                    {
                        if (asientos[i][j] == 1)
                        {
                            printf("O ");
                        }
                        else
                        {
                            printf("L ");
                        }
                    }
                    printf("\n");
                }
                break;

            case 0:
                break;

            default:
                printf("Opcion invalida.\n");
        }
    }
    while (opcion != 0);

    system("pause");
    return 0;
}
