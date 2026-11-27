/* EJERCICIO 7: BUSQUEDA BINARIA RECURSIVA
 * Funcionamiento: se compara con el elemento central y se descarta la mitad que no sirve.
 * El arreglo se ordena automaticamente si no lo esta.
 * Tipos: int[] para datos e int para indices, valor y posicion.
 */
#include <stdio.h>
#include <stdlib.h>
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
            if (c >= '0' && c <= '9' && cuenta < 7) {
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

int buscar(int a[], int i, int f, int x)
{
    int m;
    if (i > f) return -1;
    m = (i + f) / 2;
    if (a[m] == x) return m;
    if (x < a[m]) return buscar(a, i, m - 1, x);
    return buscar(a, m + 1, f, x);
}

int main(void)
{
    int a[100], n, x, i, j, aux, p;
    n = leer_entero("Cantidad de elementos [1 a 100]: ", 0, 1, 100);
    for (i = 0; i < n; i++) {
        printf("  Elemento %d: ", i + 1);
        a[i] = leer_entero("", 1, -1000000, 1000000);
    }
    /* Asegura orden ascendente para busqueda binaria */
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1 - i; j++)
            if (a[j] > a[j + 1]) { aux = a[j]; a[j] = a[j + 1]; a[j + 1] = aux; }

    printf("Arreglo ordenado: ");
    for (i = 0; i < n; i++) printf("%d ", a[i]);
    printf("\n");

    x = leer_entero("Valor a buscar: ", 1, -1000000, 1000000);
    p = buscar(a, 0, n - 1, x);
    if (p < 0) printf("No encontrado (-1)\n");
    else       printf("Encontrado en el indice: %d (posicion %d)\n", p, p + 1);

    system("pause");
    return 0;
}
