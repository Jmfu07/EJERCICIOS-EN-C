/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 07:
 * Punteros a funciones (callbacks): arreglo de operaciones matematicas y
 * ordenamiento generico recibiendo un puntero a funcion comparadora.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

static double sumar(double a, double b)  { return a + b; }
static double restar(double a, double b) { return a - b; }
static double mult(double a, double b)   { return a * b; }
static double dividir(double a, double b){ return (fabs(b) < 1e-12) ? 0.0 : (a / b); }

static int cmp_asc(int a, int b)  { return a > b; }
static int cmp_desc(int a, int b) { return a < b; }

static void ordenar_con_callback(int *v, int n, int (*criterio)(int, int)) {
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1 - i; j++)
            if (criterio(*(v + j), *(v + j + 1))) {
                int tmp = *(v + j); *(v + j) = *(v + j + 1); *(v + j + 1) = tmp;
            }
}

static double leer_double(const char *msg) {
    char buf[40]; double v; char *fin;
    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtod(buf, &fin);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0')) return v;
        printf("  Error: numero invalido.\n");
    }
}

int main(void) {
    double (*ops[4])(double, double) = { sumar, restar, mult, dividir };
    const char *noms[4] = { "Suma", "Resta", "Multiplicacion", "Division" };
    int datos[8] = { 34, -5, 12, 0, 89, -20, 7, 45 };
    double a, b;
    int op, i;

    printf("=== PUNTEROS 07: PUNTEROS A FUNCIONES (CALLBACKS) ===\n\n");
    printf("--- Parte 1: Calculadora via arreglo de punteros a funcion ---\n");
    a = leer_double("Numero A: ");
    b = leer_double("Numero B: ");
    printf("Elija operacion (1=Suma, 2=Resta, 3=Mult, 4=Div): ");
    do { op = _getch(); if (op == 0 || op == 0xE0) { _getch(); op = 0; } } while (op < '1' || op > '4');
    printf("%c\n", op);
    op -= '1';
    if (op == 3 && fabs(b) < 1e-12) printf("Error: division por cero.\n");
    else printf("%s(%.2f, %.2f) = %.4f\n", noms[op], a, b, ops[op](a, b));

    printf("\n--- Parte 2: Ordenamiento generico con callback ---\n");
    ordenar_con_callback(datos, 8, cmp_asc);
    printf("Ascendente : "); for (i = 0; i < 8; i++) printf("%d ", datos[i]); printf("\n");
    ordenar_con_callback(datos, 8, cmp_desc);
    printf("Descendente: "); for (i = 0; i < 8; i++) printf("%d ", datos[i]); printf("\n\n");

    system("pause");
    return 0;
}
