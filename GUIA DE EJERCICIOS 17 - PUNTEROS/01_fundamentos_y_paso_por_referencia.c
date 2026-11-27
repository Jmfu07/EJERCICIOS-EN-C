/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 01:
 * Fundamentos de punteros (&, *) y paso por referencia: intercambio (swap),
 * cociente/resto por puntero y minimo/maximo de tres numeros por referencia.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>

static int leer_entero(const char *msg) {
    char buf[32]; long v; char *fin;
    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtol(buf, &fin, 10);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0')) return (int)v;
        printf("  Error: ingrese un entero valido.\n");
    }
}

static void intercambiar(int *a, int *b) {
    int tmp = *a;
    *a = *b;
    *b = tmp;
}

static int dividir_ref(int dividendo, int divisor, int *cociente, int *resto) {
    if (divisor == 0) return 0;
    *cociente = dividendo / divisor;
    *resto    = dividendo % divisor;
    return 1;
}

static void min_max_3(int a, int b, int c, int *minimo, int *maximo) {
    *minimo = a;
    *maximo = a;
    if (b < *minimo) *minimo = b;
    if (c < *minimo) *minimo = c;
    if (b > *maximo) *maximo = b;
    if (c > *maximo) *maximo = c;
}

int main(void) {
    int x, y, z, coc, res, mn, mx;
    int *px = &x;

    printf("=== PUNTEROS 01: FUNDAMENTOS Y PASO POR REFERENCIA ===\n\n");
    x = leer_entero("Ingrese entero X: ");
    y = leer_entero("Ingrese entero Y: ");
    z = leer_entero("Ingrese entero Z: ");

    printf("\n1) Direcciones y desreferenciacion:\n");
    printf("   x = %d | &x = %p | px = %p | *px = %d\n", x, (void *)&x, (void *)px, *px);

    printf("\n2) Intercambio por punteros (X <-> Y):\n");
    printf("   Antes  : X = %d, Y = %d\n", x, y);
    intercambiar(&x, &y);
    printf("   Despues: X = %d, Y = %d\n", x, y);

    printf("\n3) Division por referencia (X / Y):\n");
    if (dividir_ref(x, y, &coc, &res))
        printf("   %d / %d -> Cociente = %d, Resto = %d\n", x, y, coc, res);
    else
        printf("   No se puede dividir entre cero.\n");

    min_max_3(x, y, z, &mn, &mx);
    printf("\n4) Minimo y maximo de (%d, %d, %d) por referencia:\n", x, y, z);
    printf("   Minimo = %d | Maximo = %d\n\n", mn, mx);

    system("pause");
    return 0;
}
