/*
 * Ejercicio 9 (Funciones y procedimientos): Cambiar pares por 0 e impares por 1
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera un vector con 20 numeros enteros al azar del 1 al 10, lo muestra,
 *   cambia los numeros pares por 0 y los impares por 1, muestra el vector
 *   resultante y la cantidad de impares y pares.
 *
 * COMO FUNCIONA:
 *   - cargarVector(): llena las 20 posiciones con 1 + rand() % 10.
 *   - transformarParesImpares(): reemplaza cada par por 0 y cada impar por 1,
 *     contabilizando simultaneamente cuantos pares e impares habia.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Vector v[20], indices y contadores por puntero.
 *   - char: Opcion para repetir ejecucion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define TAM 20

void cargarVector(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = 1 + rand() % 10;
    }
}

void mostrarVector(const char *titulo, const int v[], int n) {
    int i;
    printf("%-28s: [", titulo);
    for (i = 0; i < n; i++) {
        printf("%2d%s", v[i], (i < n - 1) ? " " : "");
    }
    printf("]\n");
}

void transformarParesImpares(int v[], int n, int *pares, int *impares) {
    int i;
    *pares = 0;
    *impares = 0;
    for (i = 0; i < n; i++) {
        if (v[i] % 2 == 0) {
            v[i] = 0;
            (*pares)++;
        } else {
            v[i] = 1;
            (*impares)++;
        }
    }
}

char leerSiNo(void) {
    int c;
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        if (c == 's' || c == 'S' || c == 'n' || c == 'N') {
            printf("%c\n", c);
            return (char)c;
        }
    }
}

int main(void) {
    int v[TAM];
    int pares, impares;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 09: CAMBIAR PARES POR 0 E IMPARES POR 1\n");
        printf("==========================================================\n\n");

        cargarVector(v, TAM);
        mostrarVector("Vector original (1..10)", v, TAM);

        transformarParesImpares(v, TAM, &pares, &impares);
        mostrarVector("Vector resultante (0/1)", v, TAM);

        printf("\n-> Cantidad de numeros pares   (convertidos a 0): %d\n", pares);
        printf("-> Cantidad de numeros impares (convertidos a 1): %d\n", impares);

        printf("\nDesea generar otro vector? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
