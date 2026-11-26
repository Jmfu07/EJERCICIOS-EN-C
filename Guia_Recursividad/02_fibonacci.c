/* EJERCICIO 2: FIBONACCI
 * Funcionamiento: F(0)=0 y F(1)=1. Cada termino posterior es F(n-1)+F(n-2).
 * Tipos: int para n y long long para el termino calculado.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

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
        printf("  Valor fuera de rango (%d a %d).\n", min, max);
    }
}

long long fibonacci(int n)
{
    if (n <= 1) return n;
    return fibonacci(n - 1) + fibonacci(n - 2);
}

int main(void)
{
    int n = leer_rango("Ingrese n para calcular F(n)", 0, 40);
    printf("F(%d) = %lld\n", n, fibonacci(n));
    system("pause");
    return 0;
}
