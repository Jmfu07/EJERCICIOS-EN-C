/* EJERCICIO 10: TORRES DE HANOI
 * Funcionamiento: mueve n-1 discos a la torre auxiliar, mueve el mayor al destino y vuelve a mover n-1.
 * Tipos: int para discos y char para nombres de torres.
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

void hanoi(int n, char o, char a, char d)
{
    if (n == 1) {
        printf("Mover disco 1 de %c a %c\n", o, d);
        return;
    }
    hanoi(n - 1, o, d, a);
    printf("Mover disco %d de %c a %c\n", n, o, d);
    hanoi(n - 1, a, o, d);
}

int main(void)
{
    int n = leer_rango("Cantidad de discos para Torres de Hanoi", 1, 15);
    hanoi(n, 'A', 'B', 'C');
    system("pause");
    return 0;
}
