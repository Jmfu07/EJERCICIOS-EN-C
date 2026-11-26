/* EJERCICIO 17: MULTIPLICACION SIN *
 * Funcionamiento: multiplicar a por b equivale a sumar a b veces. El caso negativo cambia el signo.
 * Tipos: int para operandos y resultado.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero(const char *prompt)
{
    char buf[16]; int len = 0, cuenta = 0, i, c;
    printf("%s", prompt);
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (cuenta > 0) break; continue; }
        if (c == '\b') {
            if (len > 0) {
                len--; cuenta = 0;
                for (i = 0; i < len; i++) if (buf[i] != '-') cuenta++;
                printf("\b \b");
            }
            continue;
        }
        if (c >= '0' && c <= '9' && cuenta < 5) {
            cuenta++; buf[len++] = (char)c; putchar(c);
        } else if (c == '-' && len == 0) {
            buf[len++] = '-'; putchar('-');
        }
    }
    buf[len] = '\0'; putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

int mult(int a, int b)
{
    if (!b) return 0;
    if (b < 0) return -mult(a, -b);
    return a + mult(a, b - 1);
}

int main(void)
{
    int a = leer_entero("Ingrese el primer entero a: ");
    int b = leer_entero("Ingrese el segundo entero b: ");
    printf("Resultado de %d x %d (sin operador *): %d\n", a, b, mult(a, b));
    system("pause");
    return 0;
}
