/* GUIA DE EJERCICIOS 5 - EJERCICIO 1:
 * Que lea 10 numeros por teclado, los almacene en un array y muestre la
 * suma, resta, multiplicacion y division de todos.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static double leer_real(int idx)
{
    char buf[20];
    int len = 0, hay_punto = 0, cuenta = 0, i, c;
    printf("Numero %2d/10: ", idx);
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (cuenta > 0) break; continue; }
        if (c == '\b') {
            if (len > 0) {
                len--;
                if (buf[len] == '.') hay_punto = 0;
                cuenta = 0;
                for (i = 0; i < len; i++) if (buf[i] != '-' && buf[i] != '.') cuenta++;
                printf("\b \b");
            }
            continue;
        }
        if (c >= '0' && c <= '9' && cuenta < 8) {
            cuenta++; buf[len++] = (char)c; putchar(c);
        } else if ((c == '.' || c == ',') && !hay_punto) {
            hay_punto = 1; buf[len++] = '.'; putchar('.');
        } else if (c == '-' && len == 0) {
            buf[len++] = '-'; putchar('-');
        }
    }
    buf[len] = '\0'; putchar('\n');
    return strtod(buf, NULL);
}

int main(void)
{
    double v[10], suma, resta, multi, divi;
    int i, hay_cero = 0;

    printf("=== 10 NUMEROS EN UN ARRAY: SUMA, RESTA, MULTIPLICACION Y DIVISION ===\n\n");
    for (i = 0; i < 10; i++)
        v[i] = leer_real(i + 1);

    suma  = v[0];
    resta = v[0];
    multi = v[0];
    divi  = v[0];

    for (i = 1; i < 10; i++) {
        suma  += v[i];
        resta -= v[i];
        multi *= v[i];
        if (v[i] == 0.0) hay_cero = 1;
        else if (!hay_cero) divi /= v[i];
    }

    printf("\n--- RESULTADOS ---\n");
    printf("Suma de todos:           %.4f\n", suma);
    printf("Resta acumulada:         %.4f\n", resta);
    printf("Multiplicacion de todos: %.4f\n", multi);
    if (hay_cero)
        printf("Division acumulada:      Indeterminada (hay al menos un divisor igual a 0)\n");
    else
        printf("Division acumulada:      %.6f\n", divi);

    system("pause");
    return 0;
}
