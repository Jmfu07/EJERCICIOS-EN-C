/* EJERCICIO 5: VALOR ABSOLUTO
 * Funcionamiento: si x es negativo se cambia su signo; si no, permanece igual.
 * Tipo: double para admitir numeros reales.
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
    buf[len] = '\0'; putchar('\n');
    return strtod(buf, NULL);
}

int main(void)
{
    double x = leer_real("Ingrese un numero real x: ");
    double abs_x = (x > 0.0) ? x : -x;
    printf("El valor absoluto de %g es: %g\n", x, abs_x);
    system("pause");
    return 0;
}
