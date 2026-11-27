/* ELABORADO POR JUAN MEDINA
 * GUIA DE EJERCICIOS 3 - EJERCICIO 12:
 * Indique cuales de estas instrucciones son correctas y cuales incorrectas,
 * e indique cual es el resultado de su ejecucion:
 *   1) i = i + 1;
 *   2) printf("%d", 4+20);
 *   3) printf("suma=var_uno + var_dos = %i + %i", var_uno, var_dos, suma);
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i = 10;
    int var_uno = 15, var_dos = 25, suma = var_uno + var_dos;

    printf("=== ANALISIS DE INSTRUCCIONES (EJERCICIO 12) ===\n\n");

    printf("1) Instruccion: i = i + 1;\n");
    printf("   - Estado: CORRECTA (si 'i' fue declarada e inicializada previamente).\n");
    printf("   - Antes: i = %d", i);
    i = i + 1;
    printf(" | Despues: i = %d\n\n", i);

    printf("2) Instruccion: printf(\"%%d\", 4+20);\n");
    printf("   - Estado: CORRECTA. Evalua 4+20 e imprime el entero resultante.\n");
    printf("   - Resultado en pantalla: ");
    printf("%d", 4 + 20);
    printf("\n\n");

    printf("3) Instruccion: printf(\"suma=var_uno + var_dos = %%i + %%i\", var_uno, var_dos, suma);\n");
    printf("   - Estado: INCORRECTA / INCOMPLETA. Se pasan 3 argumentos (var_uno, var_dos, suma)\n");
    printf("     pero la cadena de formato solo tiene 2 especificadores (%%i + %%i); falta '= %%i' para 'suma'.\n");
    printf("   - Version corregida: printf(\"suma=var_uno + var_dos = %%i + %%i = %%i\", var_uno, var_dos, suma);\n");
    printf("   - Resultado corregido: suma=var_uno + var_dos = %i + %i = %i\n\n", var_uno, var_dos, suma);

    system("pause");
    return 0;
}
