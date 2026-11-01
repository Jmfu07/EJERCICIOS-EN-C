/* EJERCICIO 6: NOTAS DE UNA CLASE
   Funcionamiento: se almacenan nombres y notas de 20 alumnos. El menu permite
   buscar, modificar notas, calcular medias y mostrar mejor y peor alumno.
   Tipos: char[][] para nombres, double[] para notas e int para indices.
*/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(void)
{
    char nombres[20][50];
    double notas[20];
    char buscado[50];
    double suma;
    double sumaMenores;
    double promedio;
    double promedioMenores;
    int cantidadMenores;
    int encontrado;
    int mejor;
    int peor;
    int opcion;
    int i;

    for (i = 0; i < 20; i++)
    {
        printf("Nombre del alumno %d: ", i + 1);
        scanf(" %49[^\n]", nombres[i]);
        printf("Nota de %s: ", nombres[i]);
        scanf("%lf", &notas[i]);
    }

    do
    {
        printf("\n--- MENU DE NOTAS ---\n");
        printf("1. Buscar alumno\n2. Modificar nota\n");
        printf("3. Media de todas\n4. Media de menores de 5\n");
        printf("5. Mejor nota\n6. Peor nota\n0. Salir\n");
        printf("Opcion: ");
        scanf("%d", &opcion);

        switch (opcion)
        {
            case 1:
            case 2:
                printf("Nombre del alumno: ");
                scanf(" %49[^\n]", buscado);
                encontrado = -1;
                for (i = 0; i < 20; i++)
                {
                    if (strcmp(nombres[i], buscado) == 0)
                    {
                        encontrado = i;
                        break;
                    }
                }
                if (encontrado == -1)
                {
                    printf("Alumno no encontrado.\n");
                }
                else if (opcion == 1)
                {
                    printf("Alumno: %s - Nota: %.2f\n",
                           nombres[encontrado], notas[encontrado]);
                }
                else
                {
                    printf("Nueva nota: ");
                    scanf("%lf", &notas[encontrado]);
                    printf("Nota modificada.\n");
                }
                break;

            case 3:
                suma = 0.0;
                for (i = 0; i < 20; i++)
                {
                    suma = suma + notas[i];
                }
                promedio = suma / 20.0;
                printf("Media de la clase: %.2f\n", promedio);
                break;

            case 4:
                sumaMenores = 0.0;
                cantidadMenores = 0;
                for (i = 0; i < 20; i++)
                {
                    if (notas[i] < 5.0)
                    {
                        sumaMenores = sumaMenores + notas[i];
                        cantidadMenores++;
                    }
                }
                if (cantidadMenores == 0)
                {
                    printf("No hay notas menores de 5.\n");
                }
                else
                {
                    promedioMenores = sumaMenores / cantidadMenores;
                    printf("Media de notas menores de 5: %.2f\n",
                           promedioMenores);
                }
                break;

            case 5:
                mejor = 0;
                for (i = 1; i < 20; i++)
                {
                    if (notas[i] > notas[mejor])
                    {
                        mejor = i;
                    }
                }
                printf("Mejor alumno: %s - %.2f\n",
                       nombres[mejor], notas[mejor]);
                break;

            case 6:
                peor = 0;
                for (i = 1; i < 20; i++)
                {
                    if (notas[i] < notas[peor])
                    {
                        peor = i;
                    }
                }
                printf("Peor alumno: %s - %.2f\n",
                       nombres[peor], notas[peor]);
                break;

            case 0:
                printf("Saliendo del programa.\n");
                break;

            default:
                printf("Opcion invalida.\n");
        }
    }
    while (opcion != 0);

    system("pause");
    return 0;
}
