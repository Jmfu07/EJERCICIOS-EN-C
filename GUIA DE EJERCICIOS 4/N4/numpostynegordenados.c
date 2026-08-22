#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Mediante un array almacene numeros tanto positivos como negativos y los 
muestre ordenados.

*/

void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    
}

int validacion (const char *mensaje)
{
    int num;
    do 
    {
        system("cls");
        printf("%s", mensaje);
        if (scanf("%d", &num) != 1) 
        {
            printf("Error: Ingrese un numero entero positivo.\n");
            limpiarBuffer();
            sleep(3);
        } 
        else 
        {
            limpiarBuffer();
            return num;
        }
    } while (1);

}


int main()
{
    // Arreglos base y copias para conservar ambas versiones
    int a_asc[5], b_asc[5];
    int a_desc[5], b_desc[5];
    int i, j, aux;

    // 1. LECTURA Y GENERACIÓN DE DATOS
    printf("=== INGRESO DE DATOS ===\n");
    for (i = 0; i < 5; i++)
    {
        printf("[%d/5] ", i + 1);
        a_asc[i] = validacion("Ingresa un numero positivo: ");

        // Copia de los datos de origen para la versión descendente
        a_desc[i] = a_asc[i];
        b_desc[i] = b_asc[i];
    }

  

    // =========================================================================
    //                        1. ORDENAMIENTO ASCENDENTE (>)
    // =========================================================================
    
    // Arreglo A (5 elementos)
    for (i = 0; i < 5 - 1; i++)
    {
        for (j = 0; j < 5 - i - 1; j++)
        {
            if (a_asc[j] > a_asc[j + 1])
            {
                aux = a_asc[j];
                a_asc[j] = a_asc[j + 1];
                a_asc[j + 1] = aux;
            }
        }
    }

    
    // =========================================================================
    //                        2. ORDENAMIENTO DESCENDENTE (<)
    // =========================================================================
    
    // Arreglo A (5 elementos)
    for (i = 0; i < 5 - 1; i++)
    {
        for (j = 0; j < 5 - i - 1; j++)
        {
            if (a_desc[j] < a_desc[j + 1])
            {
                aux = a_desc[j];
                a_desc[j] = a_desc[j + 1];
                a_desc[j + 1] = aux;
            }
        }
    }

    
    // 3. IMPRESIÓN DE RESULTADOS
    system("cls");
    printf("====================================================\n");
    printf("        RESULTADOS ORDENADOS DE AMBAS FORMAS        \n");
    printf("====================================================\n\n");

    printf("--- 1. ORDENAMIENTO ASCENDENTE (MENOR A MAYOR) ---\n");
    printf("Arreglo 'a' (5 elem):  ");
    for (i = 0; i < 5; i++) printf("%d ", a_asc[i]);

    
    printf("\n\n----------------------------------------------------\n\n");

    printf("--- 2. ORDENAMIENTO DESCENDENTE (MAYOR A MENOR) ---\n");
    printf("Arreglo 'a' (5 elem):  ");
    for (i = 0; i < 5; i++) printf("%d ", a_desc[i]);
    
    printf("\n\n====================================================\n");

    sleep(10);
    return 0;
}