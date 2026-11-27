/* GUIA DE EJERCICIOS 5 - EJERCICIO 7:
 * Que rellene una matriz de 3x3 y muestre su traspuesta (la traspuesta se
 * consigue intercambiando filas por columnas y viceversa).
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero(int f, int c)
{
    char buf[16];
    int len = 0, cuenta = 0, i, ch;
    printf("Matriz[%d][%d]: ", f, c);
    for (;;) {
        ch = _getch();
        if (ch == 0 || ch == 0xE0) { _getch(); continue; }
        if (ch == '\r') { if (cuenta > 0) break; continue; }
        if (ch == '\b') {
            if (len > 0) {
                len--; cuenta = 0;
                for (i = 0; i < len; i++) if (buf[i] != '-') cuenta++;
                printf("\b \b");
            }
            continue;
        }
        if (ch >= '0' && ch <= '9' && cuenta < 6) {
            cuenta++; buf[len++] = (char)ch; putchar(ch);
        } else if (ch == '-' && len == 0) {
            buf[len++] = '-'; putchar('-');
        }
    }
    buf[len] = '\0'; putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

int main(void)
{
    int m[3][3], f, c;

    printf("=== MATRIZ 3x3 Y SU TRASPUESTA ===\n\n");
    for (f = 0; f < 3; f++)
        for (c = 0; c < 3; c++)
            m[f][c] = leer_entero(f + 1, c + 1);

    printf("\nMatriz original:\n");
    for (f = 0; f < 3; f++) {
        for (c = 0; c < 3; c++)
            printf("%6d", m[f][c]);
        printf("\n");
    }

    printf("\nMatriz traspuesta:\n");
    for (f = 0; f < 3; f++) {
        for (c = 0; c < 3; c++)
            printf("%6d", m[c][f]);
        printf("\n");
    }

    system("pause");
    return 0;
}
