/* EJERCICIO 12: ACKERMANN
 * Funcionamiento: A(0,n)=n+1; A(m,0)=A(m-1,1); en otro caso A(m,n)=A(m-1,A(m,n-1)).
 * Tipos: unsigned int para argumentos naturales y unsigned long long para el resultado.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static unsigned int leer_natural(const char *prompt, unsigned int max)
{
    char buf[8];
    int len, c;
    unsigned int v;
    for (;;) {
        len = 0;
        printf("%s [0 a %u]: ", prompt, max);
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (len > 0) break; continue; }
            if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
            if (c >= '0' && c <= '9' && len < 2) {
                buf[len++] = (char)c; putchar(c);
            }
        }
        buf[len] = '\0'; putchar('\n');
        v = (unsigned int)strtoul(buf, NULL, 10);
        if (v <= max) return v;
        printf("  Valor demasiado grande (maximo %u para evitar desborde de pila).\n", max);
    }
}

unsigned long long A(unsigned int m, unsigned int n)
{
    if (m == 0) return n + 1;
    if (n == 0) return A(m - 1, 1);
    return A(m - 1, (unsigned int)A(m, n - 1));
}

int main(void)
{
    unsigned int m = leer_natural("Ingrese parametro m", 3);
    unsigned int n = leer_natural("Ingrese parametro n", 10);
    printf("A(%u, %u) = %llu\n", m, n, A(m, n));
    system("pause");
    return 0;
}
