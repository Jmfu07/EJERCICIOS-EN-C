/* EJERCICIO 13: MCD
 * Funcionamiento: aplica Euclides: MCD(x,y)=MCD(y,x%y), hasta que y sea 0.
 * Tipos: int para ambos enteros y el resultado.
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

int mcd(int x, int y)
{
    if (y == 0) return x < 0 ? -x : x;
    return mcd(y, x % y);
}

int main(void)
{
    int x = leer_entero_pos("Ingrese el primer entero positivo: ");
    int y = leer_entero_pos("Ingrese el segundo entero positivo: ");
    printf("MCD: %d\n", mcd(x, y));
    system("pause");
    return 0;
}
