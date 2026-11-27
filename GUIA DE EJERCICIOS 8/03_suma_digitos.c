/* EJERCICIO 3: SUMA DE DIGITOS
 * Funcionamiento: se suma el ultimo digito n%10 y se repite con n/10 hasta quedar un digito.
 * Tipos: int para el numero y la suma.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero_pos(const char *prompt)
{
    char buf[16]; int len = 0, c;
    printf("%s", prompt);
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (len > 0) break; continue; }
        if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
        if (c >= '0' && c <= '9' && len < 9) { buf[len++] = (char)c; putchar(c); }
    }
    buf[len] = '\0'; putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

int sumaDigitos(int n)
{
    if (n < 0) n = -n;
    if (n < 10) return n;
    return (n % 10) + sumaDigitos(n / 10);
}

int main(void)
{
    int n = leer_entero_pos("Ingrese un numero entero positivo: ");
    printf("Suma de sus digitos = %d\n", sumaDigitos(n));
    system("pause");
    return 0;
}
