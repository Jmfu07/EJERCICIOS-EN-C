/* EJERCICIO 18: IMPRIMIR ARREGLO EN ORDEN INVERSO
 * Funcionamiento: imprime el ultimo elemento y vuelve a llamar con un tamano menor.
 * Tipos: int[] para datos e int para tamano.
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

void inverso(int a[], int n)
{
    if (!n) return;
    printf("%d ", a[n - 1]);
    inverso(a, n - 1);
}

int main(void)
{
    int a[100], n, i;
    n = leer_entero("Cantidad de elementos [1 a 100]: ", 0, 1, 100);
    for (i = 0; i < n; i++) {
        printf("  Elemento %d: ", i + 1);
        a[i] = leer_entero("", 1, -1000000, 1000000);
    }
    printf("Arreglo en orden inverso: ");
    inverso(a, n);
    printf("\n");
    system("pause");
    return 0;
}
