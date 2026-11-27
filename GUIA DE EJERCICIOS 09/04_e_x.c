/* EJERCICIO 4: e^x
 * Funcionamiento: aproxima e^x con 1 + x/1! + x^2/2! + ... usando n terminos.
 * Tipos: double para x, factoriales y suma; int para terminos y ciclos.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static double leer_real(const char *prompt)
{
    char buf[20];
    int len = 0, hay_punto = 0, cuenta = 0, i, c;
    printf("%s", prompt);
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (cuenta > 0) break; continue; }
        if (c == '\b') {
            if (len > 0) {
                len--;
                if (buf[len] == '.') hay_punto = 0;
                cuenta = 0;
                for (i = 0; i < len; i++) if (buf[i] != '-' && buf[i] != '.') cuenta++;
                printf("\b \b");
            }
            continue;
        }
        if (c >= '0' && c <= '9' && cuenta < 8) {
            cuenta++; buf[len++] = (char)c; putchar(c);
        } else if ((c == '.' || c == ',') && !hay_punto) {
            hay_punto = 1; buf[len++] = '.'; putchar('.');
        } else if (c == '-' && len == 0) {
            buf[len++] = '-'; putchar('-');
        }
    }
    buf[len] = '\0'; putchar('\n');
    return strtod(buf, NULL);
}

static int leer_entero_pos(const char *prompt, int min, int max)
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
            if (c >= '0' && c <= '9' && len < 3) {
                buf[len++] = (char)c; putchar(c);
            }
        }
        buf[len] = '\0'; putchar('\n');
        v = (int)strtol(buf, NULL, 10);
        if (v >= min && v <= max) return v;
    }
}

int main(void)
{
    double x = leer_real("Ingrese el valor de x: ");
    int n = leer_entero_pos("Ingrese la cantidad de terminos de la serie", 1, 50);
    double s = 0.0, p = 1.0, f = 1.0;
    int i;

    for (i = 0; i < n; i++) {
        if (i > 0) {
            p *= x;
            f *= i;
        }
        s += p / f;
    }
    printf("e^(%.4f) = %.10f\n", x, s);
    system("pause");
    return 0;
}
