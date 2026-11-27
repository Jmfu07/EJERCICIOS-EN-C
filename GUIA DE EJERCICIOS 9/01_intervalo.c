/* EJERCICIO 1: INTERVALO
 * Funcionamiento: se verifica si el numero real cumple 100 <= x <= 200.
 * Tipos: double para el numero real con validacion de entrada.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static double leer_real(const char *prompt)
{
    char buf[24];
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
        if (c >= '0' && c <= '9' && cuenta < 9) {
            cuenta++; buf[len++] = (char)c; putchar(c);
        } else if ((c == '.' || c == ',') && !hay_punto) {
            hay_punto = 1; buf[len++] = '.'; putchar('.');
        } else if (c == '-' && len == 0) {
            buf[len++] = '-'; putchar('-');
        }
    }
    buf[len] = '\0';
    putchar('\n');
    return strtod(buf, NULL);
}

int main(void)
{
    double x = leer_real("Ingrese un numero real para verificar el intervalo [100, 200]: ");
    printf(x >= 100.0 && x <= 200.0 ? "Dentro del intervalo [100, 200]\n" : "Fuera del intervalo [100, 200]\n");
    system("pause");
    return 0;
}
