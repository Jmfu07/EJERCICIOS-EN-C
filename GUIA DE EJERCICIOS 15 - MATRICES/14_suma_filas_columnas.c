/* EJERCICIO 14: SUMA DE FILAS Y COLUMNAS
   Funcionamiento: se llena una matriz 10x10. Se recorre cada fila para mostrar
   sus elementos y su suma; despues se repite el procedimiento por columnas.
   Tipos: int para matriz, indices y sumas.
*/
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int matriz[10][10];
    int fila;
    int columna;
    int suma;

    printf("Ingrese los 100 elementos:\n");
    for (fila = 0; fila < 10; fila++)
    {
        for (columna = 0; columna < 10; columna++)
        {
            scanf("%d", &matriz[fila][columna]);
        }
    }

    printf("\nFilas:\n");
    for (fila = 0; fila < 10; fila++)
    {
        suma = 0;
        printf("Fila %d: ", fila);
        for (columna = 0; columna < 10; columna++)
        {
            printf("%d ", matriz[fila][columna]);
            suma = suma + matriz[fila][columna];
        }
        printf("| Suma = %d\n", suma);
    }

    printf("\nColumnas:\n");
    for (columna = 0; columna < 10; columna++)
    {
        suma = 0;
        printf("Columna %d: ", columna);
        for (fila = 0; fila < 10; fila++)
        {
            printf("%d ", matriz[fila][columna]);
            suma = suma + matriz[fila][columna];
        }
        printf("| Suma = %d\n", suma);
    }

    system("pause");
    return 0;
}
