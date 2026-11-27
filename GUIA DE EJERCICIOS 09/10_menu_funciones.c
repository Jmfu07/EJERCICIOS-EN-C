/* EJERCICIO 10: MENU DE FUNCIONES
 * Funcionamiento: un switch selecciona clasificar 10 enteros en pares/impares,
 * buscar numeros perfectos o invertir un entero de 4 digitos.
 * Tipos: int para enteros, sumas, opciones y posiciones.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

static int leer_entero_dig(int min_dig, int max_dig, int permite_neg)
{
    char buf[16];
    int len = 0, cuenta = 0, i, c;
    for (;;) {
        c = _getch();
        if (c == 0 || c == 0xE0) { _getch(); continue; }
        if (c == '\r') { if (cuenta >= min_dig) break; continue; }
        if (c == '\b') {
            if (len > 0) {
                len--; cuenta = 0;
                for (i = 0; i < len; i++) if (buf[i] != '-') cuenta++;
                printf("\b \b");
            }
            continue;
        }
        if (c >= '0' && c <= '9' && cuenta < max_dig) {
            cuenta++; buf[len++] = (char)c; putchar(c);
        } else if (permite_neg && c == '-' && len == 0) {
            buf[len++] = '-'; putchar('-');
        }
    }
    buf[len] = '\0'; putchar('\n');
    return (int)strtol(buf, NULL, 10);
}

static int es_perfecto(int n)
{
    int s = 0, d;
    if (n < 2) return 0;
    for (d = 1; d <= n / 2; d++)
        if (n % d == 0) s += d;
    return s == n;
}

static void pares_impares(void)
{
    int i, x;
    printf("\n--- 10 ENTEROS: PARES E IMPARES ---\n");
    for (i = 0; i < 10; i++) {
        printf("Entero %2d/10: ", i + 1);
        x = leer_entero_dig(1, 7, 1);
        printf("  -> %d es %s\n", x, (x % 2 == 0) ? "PAR" : "IMPAR");
    }
}

static void mostrar_perfectos(void)
{
    int n;
    printf("\n--- NUMEROS PERFECTOS (RANGO 1 A 10000) ---\n");
    for (n = 1; n <= 10000; n++)
        if (es_perfecto(n))
            printf("  Numero perfecto: %d\n", n);
}

static int invertir_4_digitos(int n)
{
    int inv = 0, i;
    for (i = 0; i < 4; i++) {
        inv = inv * 10 + (n % 10);
        n /= 10;
    }
    return inv;
}

int main(void)
{
    int op, r, n;
    do {
        printf("\n=== MENU DE FUNCIONES ===\n");
        printf("1. Determinar pares e impares de 10 enteros\n");
        printf("2. Mostrar numeros perfectos\n");
        printf("3. Invertir un numero entero de 4 digitos\n");
        printf("0. Salir\n");
        printf("Opcion [0-3]: ");

        do {
            r = _getch();
            if (r == 0 || r == 0xE0) { _getch(); r = -1; }
        } while (r < '0' || r > '3');
        printf("%c\n", r);
        op = r - '0';

        switch (op) {
        case 1:
            pares_impares();
            break;
        case 2:
            mostrar_perfectos();
            break;
        case 3:
            printf("Ingrese un entero positivo de 4 digitos: ");
            n = leer_entero_dig(4, 4, 0);
            printf("Invertido: %04d\n", invertir_4_digitos(n));
            break;
        }
    } while (op != 0);

    system("pause");
    return 0;
}
