/* GUIA DE EJERCICIOS 5 - EJERCICIO 6:
 * Que rellene un array de dos dimensiones con numeros pares, lo pinte y
 * despues que pida una posicion X,Y y muestre el numero correspondiente.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define FILAS 5
#define COLS  5

static int leer_rango(const char *prompt, int min, int max)
{
    char buf[8]; int len, c, v;
    for (;;) {
        len = 0;
        printf("%s [%d a %d]: ", prompt, min, max);
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (len > 0) break; continue; }
            if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
            if (c >= '0' && c <= '9' && len < 2) { buf[len++] = (char)c; putchar(c); }
        }
        buf[len] = '\0'; putchar('\n');
        v = (int)strtol(buf, NULL, 10);
        if (v >= min && v <= max) return v;
        printf("  Posicion fuera de rango (%d a %d).\n", min, max);
    }
}

int main(void)
{
    int m[FILAS][COLS];
    int f, c, par = 2, x, y;

    printf("=== MATRIZ %dx%d DE NUMEROS PARES ===\n\n", FILAS, COLS);
    for (f = 0; f < FILAS; f++) {
        for (c = 0; c < COLS; c++) {
            m[f][c] = par;
            par += 2;
            printf("%4d", m[f][c]);
        }
        printf("\n");
    }

    printf("\n");
    x = leer_rango("Ingrese la fila X", 1, FILAS);
    y = leer_rango("Ingrese la columna Y", 1, COLS);

    printf("\nEl numero en la posicion (%d, %d) es: %d\n", x, y, m[x - 1][y - 1]);
    system("pause");
    return 0;
}
