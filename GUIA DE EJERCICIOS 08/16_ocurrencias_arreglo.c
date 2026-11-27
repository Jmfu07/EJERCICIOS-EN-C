/* EJERCICIO 16: OCURRENCIAS EN UN ARREGLO
 * Funcionamiento: se compara el ultimo elemento y se suma 1 si coincide; luego se reduce n.
 * Tipos: int[] para arreglo, int para tamano, buscado y contador.
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

int ocurr(int a[], int n, int x)
{
    if (!n) return 0;
    return (a[n - 1] == x) + ocurr(a, n - 1, x);
}

int main(void)
{
    int a[100], n, x, i;
    n = leer_entero("Cantidad de elementos [1 a 100]: ", 0, 1, 100);
    for (i = 0; i < n; i++) {
        printf("  Elemento %d: ", i + 1);
        a[i] = leer_entero("", 1, -1000000, 1000000);
    }
    x = leer_entero("Numero a contar en el arreglo: ", 1, -1000000, 1000000);
    printf("Ocurrencias de %d: %d\n", x, ocurr(a, n, x));
    system("pause");
    return 0;
}
