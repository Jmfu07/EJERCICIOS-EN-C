/* 5 EJERCICIOS DE PUNTEROS CON FUNCIONES Y MATRICES - EJERCICIO 1:
 * Recursividad con matrices:
 * Implemente en C una funcion recursiva que, dada una matriz cuadrada de enteros,
 * calcule la suma de todos los elementos de la diagonal principal y secundaria.
 * La matriz debe manejarse unicamente con punteros (sin notacion [][]).
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
                    for (i = 0; i < len; i++) if (*(buf + i) != '-') cuenta++;
                    printf("\b \b");
                }
                continue;
            }
            if (c >= '0' && c <= '9' && cuenta < 6) {
                cuenta++; *(buf + len) = (char)c; len++; putchar(c);
            } else if (permite_neg && c == '-' && len == 0) {
                *(buf + len) = '-'; len++; putchar('-');
            }
        }
        *(buf + len) = '\0'; putchar('\n');
        v = (int)strtol(buf, NULL, 10);
        if (v >= min && v <= max) return v;
        printf("  Valor fuera de rango [%d a %d].\n", min, max);
    }
}

/* Funcion recursiva que avanza por las posiciones i de las diagonales usando solo punteros */
int suma_diagonales_rec(const int *mat, int n, int i, int *suma_prin, int *suma_sec)
{
    int val_p, val_s;
    if (i >= n) return 0;

    val_p = *(mat + i * n + i);
    val_s = *(mat + i * n + (n - 1 - i));

    *suma_prin += val_p;
    *suma_sec  += val_s;

    return val_p + ((i != n - 1 - i) ? val_s : 0) + suma_diagonales_rec(mat, n, i + 1, suma_prin, suma_sec);
}

int main(void)
{
    int n, f, c, suma_p = 0, suma_s = 0, suma_total;
    int *matriz;

    printf("=== EJERCICIO 1: SUMA RECURSIVA DE DIAGONALES CON PUNTEROS ===\n\n");
    n = leer_entero("Ingrese el tamano N de la matriz cuadrada [2 a 10]: ", 0, 2, 10);

    matriz = (int *)malloc((size_t)(n * n) * sizeof(int));
    if (!matriz) {
        printf("Error de memoria.\n");
        return 1;
    }

    for (f = 0; f < n; f++) {
        for (c = 0; c < n; c++) {
            printf("  Elemento (%d, %d): ", f + 1, c + 1);
            *(matriz + f * n + c) = leer_entero("", 1, -100000, 100000);
        }
    }

    printf("\nMatriz ingresada:\n");
    for (f = 0; f < n; f++) {
        for (c = 0; c < n; c++)
            printf("%6d ", *(matriz + f * n + c));
        printf("\n");
    }

    suma_total = suma_diagonales_rec(matriz, n, 0, &suma_p, &suma_s);

    printf("\nSuma diagonal principal:  %d\n", suma_p);
    printf("Suma diagonal secundaria: %d\n", suma_s);
    printf("Suma total de ambas:      %d\n", suma_total);

    free(matriz);
    system("pause");
    return 0;
}
