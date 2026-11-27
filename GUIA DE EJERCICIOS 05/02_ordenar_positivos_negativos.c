/* GUIA DE EJERCICIOS 5 - EJERCICIO 2:
 * Que mediante un array almacene numeros tanto positivos como negativos
 * y los muestre ordenados.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero(int idx)
{
    char buf[16];
    int len = 0, cuenta = 0, i, c;
    printf("Numero %2d/10 (positivo o negativo): ", idx);
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
        } else if (c == '-' && len == 0) {
            buf[len++] = '-'; putchar('-');
        }
    }
    buf[len] = '\0'; putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

int main(void)
{
    int v[10], i, j, aux;

    printf("=== ALMACENAR Y ORDENAR NUMEROS POSITIVOS Y NEGATIVOS ===\n\n");
    for (i = 0; i < 10; i++)
        v[i] = leer_entero(i + 1);

    for (i = 0; i < 9; i++)
        for (j = 0; j < 9 - i; j++)
            if (v[j] > v[j + 1]) {
                aux = v[j]; v[j] = v[j + 1]; v[j + 1] = aux;
            }

    printf("\nNumeros ordenados de menor a mayor:\n");
    for (i = 0; i < 10; i++) printf("%d ", v[i]);
    printf("\n");

    system("pause");
    return 0;
}
