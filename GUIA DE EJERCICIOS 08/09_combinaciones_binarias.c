/* EJERCICIO 9: COMBINACIONES BINARIAS
 * Funcionamiento: en cada posicion se prueba primero 0 y despues 1 hasta completar n.
 * Tipos: char[] guarda la combinacion e int controla posiciones y longitud.
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

void generar(char s[], int p, int n)
{
    if (p == n) {
        s[p] = 0;
        printf("%s\n", s);
        return;
    }
    s[p] = '0';
    generar(s, p + 1, n);
    s[p] = '1';
    generar(s, p + 1, n);
}

int main(void)
{
    char s[30];
    int n = leer_rango("Ingrese la longitud n de las cadenas binarias", 1, 16);
    generar(s, 0, n);
    system("pause");
    return 0;
}
