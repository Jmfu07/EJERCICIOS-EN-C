/* EJERCICIO 11: MCD RECURSIVO
 * Funcionamiento: aplica MCD(x,y) = MCD(y, x%y) hasta que y = 0.
 * Tipo: int para entradas y resultado.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero_pos(const char *prompt)
{
    char buf[16];
    int len = 0, c;
    printf("%s", prompt);
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (len > 0) break; continue; }
        if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
        if (c >= '0' && c <= '9' && len < 8) {
            buf[len++] = (char)c; putchar(c);
        }
    }
    buf[len] = '\0'; putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

int mcd(int x, int y)
{
    if (y == 0) return x;
    return mcd(y, x % y);
}

int main(void)
{
    int x = leer_entero_pos("Ingrese el primer entero x: ");
    int y = leer_entero_pos("Ingrese el segundo entero y: ");
    printf("MCD(%d, %d) = %d\n", x, y, mcd(x, y));
    system("pause");
    return 0;
}
