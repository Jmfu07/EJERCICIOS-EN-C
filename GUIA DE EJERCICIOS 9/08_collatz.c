/* EJERCICIO 8: CONJETURA DE COLLATZ
 * Funcionamiento: si n es par se divide entre 2; si es impar se calcula 3n+1; termina en 1.
 * Tipo: long long para soportar valores mayores durante la secuencia.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static long long leer_entero_pos(const char *prompt)
{
    char buf[16];
    int len = 0, c;
    long long v;
    for (;;) {
        len = 0;
        printf("%s", prompt);
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (len > 0) break; continue; }
            if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
            if (c >= '0' && c <= '9' && len < 9) {
                buf[len++] = (char)c; putchar(c);
            }
        }
        buf[len] = '\0'; putchar('\n');
        v = strtoll(buf, NULL, 10);
        if (v > 0) return v;
        printf("El numero debe ser un entero positivo mayor a 0.\n");
    }
}

int main(void)
{
    long long n = leer_entero_pos("Ingrese un entero positivo para la secuencia de Collatz: ");
    printf("Secuencia: ");
    while (n != 1) {
        printf("%lld, ", n);
        if (n % 2 == 0) n /= 2;
        else            n = 3 * n + 1;
    }
    printf("1\n");
    system("pause");
    return 0;
}
