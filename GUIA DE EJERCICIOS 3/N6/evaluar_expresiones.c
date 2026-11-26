/* ELABORADO POR JUAN MEDINA
 * GUIA DE EJERCICIOS 3 - EJERCICIO 6:
 * Dado el fragmento de codigo:
 *   int i1, i2, i3;
 *   float r1, r2, r3;
 *   i1=12; i2=5; r1=12.0; r2=5;
 *   r3 = r1/r2;
 *   r3 = i1/i2;
 *   i3 = i1/r2;
 * Identifique las declaraciones de variables, las instrucciones de asignacion,
 * los operadores y las expresiones, y muestre el resultado de evaluar cada una.
 */
#include <stdio.h>
#include <stdlib.h>

int main(void)
{
    int i1, i2, i3;
    float r1, r2, r3;

    i1 = 12;
    i2 = 5;
    r1 = 12.0f;
    r2 = 5.0f;

    printf("=== ANALISIS DEL FRAGMENTO DE CODIGO (EJERCICIO 6) ===\n\n");
    printf("1. Declaraciones de variables:\n");
    printf("   - Enteras (int):   i1, i2, i3\n");
    printf("   - Reales  (float): r1, r2, r3\n\n");

    printf("2. Instrucciones de asignacion iniciales:\n");
    printf("   i1 = %d, i2 = %d, r1 = %.1f, r2 = %.1f\n\n", i1, i2, r1, r2);

    printf("3. Evaluacion de expresiones con el operador division (/):\n");

    r3 = r1 / r2;
    printf("   a) r3 = r1 / r2  (real / real: 12.0 / 5.0)   -> r3 = %.2f\n", r3);

    r3 = (float)(i1 / i2);
    printf("   b) r3 = i1 / i2  (entero / entero: 12 / 5=2) -> r3 = %.2f\n", r3);

    i3 = (int)(i1 / r2);
    printf("   c) i3 = i1 / r2  (12 / 5.0 = 2.4 -> truncado a int) -> i3 = %d\n\n", i3);

    system("pause");
    return 0;
}
