/* EJERCICIO 7: CALIFICACIONES
 * Funcionamiento: recibe cuatro calificaciones y devuelve promedio, maxima y minima.
 * Tipos: double para notas/promedio; int para el contador.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static double leer_nota(int num)
{
    char buf[16];
    int len, hay_punto, cuenta, i, c;
    double v;
    for (;;) {
        len = 0; hay_punto = 0; cuenta = 0;
        printf("Ingrese calificacion %d [0 a 20]: ", num);
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (cuenta > 0) break; continue; }
            if (c == '\b') {
                if (len > 0) {
                    len--;
                    if (buf[len] == '.') hay_punto = 0;
                    cuenta = 0;
                    for (i = 0; i < len; i++) if (buf[i] != '.') cuenta++;
                    printf("\b \b");
                }
                continue;
            }
            if (c >= '0' && c <= '9' && cuenta < 5) {
                cuenta++; buf[len++] = (char)c; putchar(c);
            } else if ((c == '.' || c == ',') && !hay_punto) {
                hay_punto = 1; buf[len++] = '.'; putchar('.');
            }
        }
        buf[len] = '\0'; putchar('\n');
        v = strtod(buf, NULL);
        if (v >= 0.0 && v <= 20.0) return v;
        printf("  Nota fuera de rango (0 a 20).\n");
    }
}

int main(void)
{
    double x, s = 0.0, ma = 0.0, mi = 0.0;
    int i;

    for (i = 0; i < 4; i++) {
        x = leer_nota(i + 1);
        s += x;
        if (i == 0 || x > ma) ma = x;
        if (i == 0 || x < mi) mi = x;
    }
    printf("\nPromedio: %.2f | Maxima: %.2f | Minima: %.2f\n", s / 4.0, ma, mi);
    system("pause");
    return 0;
}
