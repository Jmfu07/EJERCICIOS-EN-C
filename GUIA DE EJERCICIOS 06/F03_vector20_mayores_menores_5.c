/*
 * Ejercicio 3 (Funciones y procedimientos): Vector de 20 posiciones (> 5 y <= 5)
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Carga un vector de 20 posiciones con numeros al azar del 1 al 10,
 *   muestra dichos numeros, halla la cantidad de numeros mayores a 5 y
 *   la cantidad de numeros menores o iguales que 5, y muestra ambos
 *   resultados usando funciones y procedimientos.
 *
 * COMO FUNCIONA:
 *   - cargarVector(): llena las 20 posiciones con 1 + rand() % 10.
 *   - mostrarVector(): imprime los 20 elementos.
 *   - contarMayoresA5() y contarMenoresIgualesA5(): retornan las cantidades.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Vector v[20], indices y contadores.
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

void mostrarVector(const int v[], int n) {
    int i;
    printf("Vector (%d elementos): [", n);
    for (i = 0; i < n; i++) {
        printf("%d%s", v[i], (i < n - 1) ? ", " : "");
    }
    printf("]\n");
}

int contarMayoresA5(const int v[], int n) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (v[i] > 5) {
            cant++;
        }
    }
    return cant;
}

int contarMenoresIgualesA5(const int v[], int n) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (v[i] <= 5) {
            cant++;
        }
    }
    return cant;
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
    int mayores, menoresIguales;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 03: VECTOR DE 20 ELEMENTOS (> 5 y <= 5)\n");
        printf("==========================================================\n\n");

        cargarVector(v, TAM);
        mostrarVector(v, TAM);

        mayores = contarMayoresA5(v, TAM);
        menoresIguales = contarMenoresIgualesA5(v, TAM);

        printf("\n-> Cantidad de numeros mayores a 5          : %d\n", mayores);
        printf("-> Cantidad de numeros menores o iguales a 5: %d\n", menoresIguales);

        printf("\nDesea generar otro vector? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
