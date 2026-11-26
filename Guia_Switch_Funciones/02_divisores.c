/* EJERCICIO 2: DIVISORES
 * Funcionamiento: el operador % comprueba si el residuo es cero entre dos enteros.
 * Tipos: int para los dos enteros y sus residuos.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero(const char *prompt)
{
    char buf[16];
    int len = 0, cuenta = 0, i, c;
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
        if (c >= '0' && c <= '9' && cuenta < 8) {
            cuenta++; buf[len++] = (char)c; putchar(c);
        } else if (c == '-' && len == 0) {
            buf[len++] = '-'; putchar('-');
        }
    }
    buf[len] = '\0';
    putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

int main(void)
{
    int a = leer_entero("Ingrese el primer entero: ");
    int b = leer_entero("Ingrese el segundo entero: ");

    if (a == 0 || b == 0)
        printf("No se puede dividir entre cero.\n");
    else if (a % b == 0 || b % a == 0)
        printf("Uno es divisor exacto del otro.\n");
    else
        printf("No son divisores exactos.\n");

    system("pause");
    return 0;
}
