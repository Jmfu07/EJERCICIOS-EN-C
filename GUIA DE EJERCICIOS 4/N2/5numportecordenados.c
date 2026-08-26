#define __USE_MINGW_ANSI_STDIO 1
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <math.h>

// ELABORADO POR JUAN MEDINA 20/08/2026

/*

Que lea 5 numeros por teclado, los copie en otro array multiplicados por 2 
y los muestre todos ordenados usando un tercer array.

*/


void limpiarBuffer() 
{
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int validacion(const char *mensaje)
{
    int num;
    do 
    {
        system("cls");
        printf("%s", mensaje);
        if (scanf("%d", &num) != 1 || num <= 0) 
        {
            printf("Error: Ingrese un numero entero positivo.\n");
            limpiarBuffer();
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
    int a_asc[5], b_asc[5], c_asc[10];
    int a_desc[5], b_desc[5], c_desc[10];
    int i, j, aux;

    // 1. LECTURA Y GENERACIÓN DE DATOS
    printf("=== INGRESO DE DATOS ===\n");
    for (i = 0; i < 5; i++)
    {
        printf("[%d/5] ", i + 1);
        a_asc[i] = validacion("Ingresa un numero positivo: ");
        b_asc[i] = a_asc[i] * 2;

        // Copia de los datos de origen para la versión descendente
        a_desc[i] = a_asc[i];
        b_desc[i] = b_asc[i];
    }

    // 2. UNIFICACIÓN EN LOS TERCEROS ARREGLOS 'c' (10 ELEMENTOS)
    for (i = 0; i < 5; i++)
    {
        c_asc[i] = a_asc[i];
        c_asc[i + 5] = b_asc[i];

        c_desc[i] = a_desc[i];
        c_desc[i + 5] = b_desc[i];
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

    // Arreglo B (5 elementos)
    for (i = 0; i < 5 - 1; i++)
    {
        for (j = 0; j < 5 - i - 1; j++)
        {
            if (b_asc[j] > b_asc[j + 1])
            {
                aux = b_asc[j];
                b_asc[j] = b_asc[j + 1];
                b_asc[j + 1] = aux;
            }
        }
    }

    // Arreglo C (10 elementos)
    for (i = 0; i < 10 - 1; i++)
    {
        for (j = 0; j < 10 - i - 1; j++)
        {
            if (c_asc[j] > c_asc[j + 1])
            {
                aux = c_asc[j];
                c_asc[j] = c_asc[j + 1];
                c_asc[j + 1] = aux;
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

    // Arreglo B (5 elementos)
    for (i = 0; i < 5 - 1; i++)
    {
        for (j = 0; j < 5 - i - 1; j++)
        {
            if (b_desc[j] < b_desc[j + 1])
            {
                aux = b_desc[j];
                b_desc[j] = b_desc[j + 1];
                b_desc[j + 1] = aux;
            }
        }
    }

    // Arreglo C (10 elementos)
    for (i = 0; i < 10 - 1; i++)
    {
        for (j = 0; j < 10 - i - 1; j++)
        {
            if (c_desc[j] < c_desc[j + 1])
            {
                aux = c_desc[j];
                c_desc[j] = c_desc[j + 1];
                c_desc[j + 1] = aux;
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

    printf("\nArreglo 'b' (5 elem):  ");
    for (i = 0; i < 5; i++) printf("%d ", b_asc[i]);

    printf("\nArreglo 'c' (10 elem): ");
    for (i = 0; i < 10; i++) printf("%d ", c_asc[i]);

    printf("\n\n----------------------------------------------------\n\n");

    printf("--- 2. ORDENAMIENTO DESCENDENTE (MAYOR A MENOR) ---\n");
    printf("Arreglo 'a' (5 elem):  ");
    for (i = 0; i < 5; i++) printf("%d ", a_desc[i]);

    printf("\nArreglo 'b' (5 elem):  ");
    for (i = 0; i < 5; i++) printf("%d ", b_desc[i]);

    printf("\nArreglo 'c' (10 elem): ");
    for (i = 0; i < 10; i++) printf("%d ", c_desc[i]);

    printf("\n\n====================================================\n");

    sleep(10);
    return 0;
}