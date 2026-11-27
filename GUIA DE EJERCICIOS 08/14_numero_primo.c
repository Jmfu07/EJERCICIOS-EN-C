/* EJERCICIO 14: NUMERO PRIMO
 * Funcionamiento: prueba divisores desde 2 hasta raiz de n; si alguno divide exactamente, no es primo.
 * Tipos: int para numero, divisor y resultado logico.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero_pos(const char *prompt)
{
    char buf[16]; int len = 0, c;
    printf("%s", prompt);
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (len > 0) break; continue; }
        if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
        if (c >= '0' && c <= '9' && len < 8) { buf[len++] = (char)c; putchar(c); }
    }
    buf[len] = '\0'; putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

int primo(int n, int d)
{
    if (n < 2) return 0;
    if (d > n / d) return 1;
    if (n % d == 0) return 0;
    return primo(n, d + 1);
}

int main(void)
{
    int n = leer_entero_pos("Ingrese un numero entero positivo: ");
    printf(primo(n, 2) ? "Es primo\n" : "No es primo\n");
    system("pause");
    return 0;
}
