/* EJERCICIO 1: FACTORIAL
 * Funcionamiento: n! es 1 cuando n=0 o n=1. Para los demas valores se calcula
 * n * (n-1)!; por eso la funcion se llama a si misma.
 * Tipos: int para el dato y long long para evitar desbordamiento.
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

long long factorial(int n)
{
    if (n <= 1) return 1;
    return n * factorial(n - 1);
}

int main(void)
{
    int n = leer_rango("Ingrese n para calcular n!", 0, 20);
    printf("%d! = %lld\n", n, factorial(n));
    system("pause");
    return 0;
}
