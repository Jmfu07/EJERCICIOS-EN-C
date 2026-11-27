/* EJERCICIO 12: DECIMAL A BINARIO
 * Funcionamiento: se divide entre 2 recursivamente y se imprime el residuo al regresar.
 * Tipos: unsigned int para la parte entera y double para fraccionaria opcional.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static double leer_decimal_pos(const char *prompt)
{
    char buf[20]; int len = 0, hay_punto = 0, cuenta = 0, i, c;
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
                for (i = 0; i < len; i++) if (buf[i] != '.') cuenta++;
                printf("\b \b");
            }
            continue;
        }
        if (c >= '0' && c <= '9' && cuenta < 9) {
            cuenta++; buf[len++] = (char)c; putchar(c);
        } else if ((c == '.' || c == ',') && !hay_punto) {
            hay_punto = 1; buf[len++] = '.'; putchar('.');
        }
    }
    buf[len] = '\0'; putchar('\n');
    return strtod(buf, NULL);
}

void bin_entera(unsigned int n)
{
    if (n > 1) bin_entera(n / 2);
    printf("%u", n % 2);
}

void bin_fraccionaria(double f, int paso)
{
    int bit;
    if (f <= 0.0 || paso >= 6) return;
    if (paso == 0) printf(".");
    f *= 2.0;
    bit = (int)f;
    printf("%d", bit);
    bin_fraccionaria(f - bit, paso + 1);
}

int main(void)
{
    double n = leer_decimal_pos("Introduce un numero decimal positivo: ");
    unsigned int parte_entera = (unsigned int)n;
    double parte_fraccionaria = n - parte_entera;

    printf("Representacion binaria: ");
    bin_entera(parte_entera);
    bin_fraccionaria(parte_fraccionaria, 0);
    printf("\n");

    system("pause");
    return 0;
}
