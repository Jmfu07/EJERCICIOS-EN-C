/* EJERCICIO 19: POTENCIA
 * Funcionamiento: a^b es 1 cuando b=0; en otro caso es a por a^(b-1).
 * Tipos: int para base/exponente y long long para resultado.
 */
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <conio.h>

static int leer_entero(const char *prompt, int permite_neg, int min, int max)
{
    char buf[16]; int len, cuenta, i, c, v;
    for (;;) {
        len = 0; cuenta = 0;
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
            if (c >= '0' && c <= '9' && cuenta < 4) {
                cuenta++; buf[len++] = (char)c; putchar(c);
            } else if (permite_neg && c == '-' && len == 0) {
                buf[len++] = '-'; putchar('-');
            }
        }
        buf[len] = '\0'; putchar('\n');
        v = (int)strtol(buf, NULL, 10);
        if (v >= min && v <= max) return v;
        printf("  Valor fuera de rango [%d a %d].\n", min, max);
    }
}

long long potencia(int a, int b, int *overflow)
{
    long long sub, abs_a;
    if (!b) return 1;
    sub = potencia(a, b - 1, overflow);
    abs_a = (a < 0) ? -(long long)a : (long long)a;
    if (*overflow || (abs_a > 0 && llabs(sub) > LLONG_MAX / abs_a)) {
        *overflow = 1;
        return 0;
    }
    return (long long)a * sub;
}

int main(void)
{
    int a = leer_entero("Ingrese la base entera a [-100 a 100]: ", 1, -100, 100);
    int b = leer_entero("Ingrese el exponente entero no negativo b [0 a 18]: ", 0, 0, 18);
    int overflow = 0;
    long long res = potencia(a, b, &overflow);
    if (overflow) printf("Aviso: %d^%d excede el limite de long long (64 bits).\n", a, b);
    else printf("Resultado de %d^%d: %lld\n", a, b, res);
    system("pause");
    return 0;
}
