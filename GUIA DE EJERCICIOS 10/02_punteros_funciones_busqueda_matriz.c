/* 5 EJERCICIOS DE PUNTEROS CON FUNCIONES Y MATRICES - EJERCICIO 2:
 * Punteros con funciones y busqueda:
 * Disene un programa en C que permita buscar un numero en una matriz usando
 * diferentes estrategias de busqueda (lineal y binaria).
 * - Almacenar la matriz en un bloque contiguo de memoria con malloc.
 * - Definir dos funciones de busqueda (busqueda_lineal, busqueda_binaria)
 *   y un puntero a funcion que seleccione cual usar.
 * - Mostrar la posicion (fila, columna) del numero si existe, o un mensaje si no se encuentra.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int (* EstrategiaBusqueda)(const int *mat, int total, int objetivo);

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

int busqueda_lineal(const int *mat, int total, int objetivo)
{
    int i;
    for (i = 0; i < total; i++)
        if (*(mat + i) == objetivo)
            return i;
    return -1;
}

static int binaria_rec(const int *mat, int ini, int fin, int objetivo)
{
    int med;
    if (ini > fin) return -1;
    med = (ini + fin) / 2;
    if (*(mat + med) == objetivo) return med;
    if (objetivo < *(mat + med)) return binaria_rec(mat, ini, med - 1, objetivo);
    return binaria_rec(mat, med + 1, fin, objetivo);
}

int busqueda_binaria(const int *mat, int total, int objetivo)
{
    return binaria_rec(mat, 0, total - 1, objetivo);
}

static void ordenar_matriz(int *mat, int total)
{
    int i, j, aux;
    for (i = 0; i < total - 1; i++) {
        for (j = 0; j < total - 1 - i; j++) {
            if (*(mat + j) > *(mat + j + 1)) {
                aux = *(mat + j);
                *(mat + j) = *(mat + j + 1);
                *(mat + j + 1) = aux;
            }
        }
    }
}

int main(void)
{
    int filas, cols, total, f, c, objetivo, modo, pos;
    int *matriz;
    EstrategiaBusqueda fn_buscar;

    printf("=== EJERCICIO 2: PUNTEROS A FUNCIONES Y BUSQUEDA EN MATRIZ ===\n\n");
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

    printf("\nEstrategia de busqueda:\n");
    printf("  1. Busqueda Lineal\n");
    printf("  2. Busqueda Binaria (ordena la matriz automaticamente)\n");
    modo = leer_entero("Seleccione [1 a 2]: ", 0, 1, 2);

    if (modo == 2) {
        ordenar_matriz(matriz, total);
        fn_buscar = busqueda_binaria;
    } else {
        fn_buscar = busqueda_lineal;
    }

    printf("\nMatriz actual:\n");
    for (f = 0; f < filas; f++) {
        for (c = 0; c < cols; c++)
            printf("%6d ", *(matriz + f * cols + c));
        printf("\n");
    }

    objetivo = leer_entero("\nIngrese el numero a buscar: ", 1, -100000, 100000);
    pos = fn_buscar(matriz, total, objetivo);

    if (pos >= 0)
        printf("Numero %d encontrado en Fila %d, Columna %d (indice lineal %d).\n",
               objetivo, (pos / cols) + 1, (pos % cols) + 1, pos);
    else
        printf("El numero %d no se encuentra en la matriz.\n", objetivo);

    free(matriz);
    system("pause");
    return 0;
}
