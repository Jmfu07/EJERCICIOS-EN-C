/* 5 EJERCICIOS DE PUNTEROS CON FUNCIONES Y MATRICES - EJERCICIO 4:
 * Operaciones con punteros y funciones:
 * Construya un programa en C que permita aplicar diferentes operaciones
 * matematicas (suma, resta, multiplicacion) sobre todos los elementos de una matriz.
 * - Usar punteros a funciones para seleccionar la operacion.
 * - Recorrer la matriz con aritmetica de punteros.
 * - Mostrar la matriz resultante despues de aplicar la operacion.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int (* OperacionElemento)(int elemento, int valor);

static int op_sumar(int a, int b)       { return a + b; }
static int op_restar(int a, int b)      { return a - b; }
static int op_multiplicar(int a, int b) { return a * b; }

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

static void aplicar_operacion(const int *origen, int *destino, int total, int escalar, OperacionElemento fn)
{
    const int *p_orig = origen;
    int *p_dest = destino;
    const int *fin = origen + total;

    while (p_orig < fin) {
        *p_dest = fn(*p_orig, escalar);
        p_orig++;
        p_dest++;
    }
}

int main(void)
{
    int filas, cols, total, f, c, opcion, escalar;
    int *matriz, *resultado;
    OperacionElemento operaciones[3] = { op_sumar, op_restar, op_multiplicar };

    printf("=== EJERCICIO 4: OPERACIONES CON PUNTEROS A FUNCIONES EN MATRIZ ===\n\n");
    filas = leer_entero("Numero de filas [1 a 10]: ", 0, 1, 10);
    cols  = leer_entero("Numero de columnas [1 a 10]: ", 0, 1, 10);
    total = filas * cols;

    matriz    = (int *)malloc((size_t)total * sizeof(int));
    resultado = (int *)malloc((size_t)total * sizeof(int));
    if (!matriz || !resultado) return 1;

    for (f = 0; f < filas; f++) {
        for (c = 0; c < cols; c++) {
            printf("  Matriz(%d, %d): ", f + 1, c + 1);
            *(matriz + f * cols + c) = leer_entero("", 1, -10000, 10000);
        }
    }

    printf("\nSeleccione la operacion a aplicar sobre cada elemento:\n");
    printf("  1. Suma (+)\n");
    printf("  2. Resta (-)\n");
    printf("  3. Multiplicacion (*)\n");
    opcion  = leer_entero("Opcion [1 a 3]: ", 0, 1, 3);
    escalar = leer_entero("Ingrese el valor escalar a aplicar: ", 1, -10000, 10000);

    aplicar_operacion(matriz, resultado, total, escalar, *(operaciones + (opcion - 1)));

    printf("\nMatriz original:\n");
    for (f = 0; f < filas; f++) {
        for (c = 0; c < cols; c++)
            printf("%6d ", *(matriz + f * cols + c));
        printf("\n");
    }

    printf("\nMatriz resultante:\n");
    for (f = 0; f < filas; f++) {
        for (c = 0; c < cols; c++)
            printf("%6d ", *(resultado + f * cols + c));
        printf("\n");
    }

    free(matriz);
    free(resultado);
    system("pause");
    return 0;
}
