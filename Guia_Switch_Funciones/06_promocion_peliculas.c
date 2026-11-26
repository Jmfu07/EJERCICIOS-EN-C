/* EJERCICIO 6: PROMOCION DE PELICULAS
 * Funcionamiento: de tres precios se paga la suma de las dos mas baratas (suma total menos el mayor).
 * Tipo: double para precios con centavos.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static double leer_precio(const char *prompt)
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
                for (i = 0; i < len; i++) if (buf[i] != '.') cuenta++;
                printf("\b \b");
            }
            continue;
        }
        if (c >= '0' && c <= '9' && cuenta < 8) {
            cuenta++; buf[len++] = (char)c; putchar(c);
        } else if ((c == '.' || c == ',') && !hay_punto) {
            hay_punto = 1; buf[len++] = '.'; putchar('.');
        }
    }
    buf[len] = '\0'; putchar('\n');
    return strtod(buf, NULL);
}

int main(void)
{
    double a = leer_precio("Precio de la pelicula 1: ");
    double b = leer_precio("Precio de la pelicula 2: ");
    double c = leer_precio("Precio de la pelicula 3: ");
    double m = a;
    if (b > m) m = b;
    if (c > m) m = c;
    printf("Cantidad a pagar (las 2 mas baratas): %.2f\n", a + b + c - m);
    system("pause");
    return 0;
}
