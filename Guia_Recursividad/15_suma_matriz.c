/* EJERCICIO 15: SUMA DE MATRIZ
 * Funcionamiento: la posicion lineal visita cada celda; fila=pos/columnas y columna=pos%columnas.
 * Tipos: int[][] para la matriz e int para dimensiones, posicion y suma.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero(const char *prompt, int permite_neg, int min, int max)
{
    char buf[16]; int len, cuenta, i, c, v;
    for (;;) {
        len = 0; cuenta = 0;
        printf("%s", prompt);
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (cuenta > 0) break; continue; }
            if (c == '\b') {
                if (len > 0) {
                    len--; cuenta = 0;
                    for (i = 0; i < len; i++) if (buf[i] != '-') cuenta++;
                    printf("\b \b");
                }
                continue;
            }
            if (c >= '0' && c <= '9' && cuenta < 7) {
                cuenta++; buf[len++] = (char)c; putchar(c);
            } else if (permite_neg && c == '-' && len == 0) {
                buf[len++] = '-'; putchar('-');
            }
        }
        buf[len] = '\0'; putchar('\n');
        v = (int)strtol(buf, NULL, 10);
        if (v >= min && v <= max) return v;
        printf("  Valor fuera de rango [%d a %d].\n", min, max);
    }
}

int suma(int a[][20], int f, int c, int p)
{
    if (p == f * c) return 0;
    return a[p / c][p % c] + suma(a, f, c, p + 1);
}

int main(void)
{
    int a[20][20], f, c, i, j;
    f = leer_entero("Numero de filas [1 a 20]: ", 0, 1, 20);
    c = leer_entero("Numero de columnas [1 a 20]: ", 0, 1, 20);

    for (i = 0; i < f; i++) {
        for (j = 0; j < c; j++) {
            printf("  Matriz[%d][%d]: ", i + 1, j + 1);
            a[i][j] = leer_entero("", 1, -100000, 100000);
        }
    }
    printf("Suma total de la matriz: %d\n", suma(a, f, c, 0));
    system("pause");
    return 0;
}
