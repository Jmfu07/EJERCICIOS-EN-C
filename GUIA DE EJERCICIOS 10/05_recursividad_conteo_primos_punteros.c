/* 5 EJERCICIOS DE PUNTEROS CON FUNCIONES Y MATRICES - EJERCICIO 5:
 * Recursividad y conteo en matrices:
 * Implemente en C una funcion recursiva que cuente cuantos numeros primos
 * existen en una matriz.
 * - Leer la matriz desde teclado y almacenarla con punteros.
 * - Usar una funcion recursiva que avance celda por celda.
 * - Mostrar el total de numeros primos encontrados.
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

static int es_primo_rec(int n, int d)
{
    if (n < 2) return 0;
    if (d * d > n) return 1;
    if (n % d == 0) return 0;
    return es_primo_rec(n, d + 1);
}

/* Funcion recursiva que avanza celda por celda con aritmetica de punteros */
int contar_primos_matriz_rec(const int *ptr_celda, int restantes)
{
    if (restantes <= 0)
        return 0;
    return es_primo_rec(*ptr_celda, 2) + contar_primos_matriz_rec(ptr_celda + 1, restantes - 1);
}

int main(void)
{
    int filas, cols, total, f, c, total_primos;
    int *matriz;

    printf("=== EJERCICIO 5: CONTEO RECURSIVO DE PRIMOS EN MATRIZ CON PUNTEROS ===\n\n");
    filas = leer_entero("Numero de filas [1 a 10]: ", 0, 1, 10);
    cols  = leer_entero("Numero de columnas [1 a 10]: ", 0, 1, 10);
    total = filas * cols;

    matriz = (int *)malloc((size_t)total * sizeof(int));
    if (!matriz) return 1;

    for (f = 0; f < filas; f++) {
        for (c = 0; c < cols; c++) {
            printf("  Matriz(%d, %d): ", f + 1, c + 1);
            *(matriz + f * cols + c) = leer_entero("", 1, -100000, 100000);
        }
    }

    printf("\nMatriz ingresada:\n");
    for (f = 0; f < filas; f++) {
        for (c = 0; c < cols; c++)
            printf("%6d ", *(matriz + f * cols + c));
        printf("\n");
    }

    total_primos = contar_primos_matriz_rec(matriz, total);
    printf("\nTotal de numeros primos encontrados en la matriz: %d\n", total_primos);

    free(matriz);
    system("pause");
    return 0;
}
