/*
 * Ejercicio 12 (Funciones y procedimientos): Comparar dos vectores va y vb en vc
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera dos vectores "va" y "vb" con 20 numeros al azar del 0 al 10.
 *   Crea otro vector "vc" que contiene en cada posicion la letra 'A' si
 *   va[i] > vb[i], 'B' si vb[i] > va[i], o '=' si son iguales. Muestra
 *   los tres vectores y la cantidad de 'A' y de 'B'.
 *
 * COMO FUNCIONA:
 *   - cargarVector(): llena va y vb con rand() % 11 (valores 0..10).
 *   - construirVectorComparacion(): llena vc[20] con 'A', 'B' o '='.
 *   - contarLetra(): cuenta cuantas veces aparece un caracter en vc.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Vectores va[20], vb[20], indices y contadores.
 *   - char: Vector vc[20] y opcion de repeticion.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define TAM 20

void cargarVector(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = rand() % 11;
    }
}

void construirVectorComparacion(const int va[], const int vb[], char vc[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        if (va[i] > vb[i]) {
            vc[i] = 'A';
        } else if (vb[i] > va[i]) {
            vc[i] = 'B';
        } else {
            vc[i] = '=';
        }
    }
}

int contarLetra(const char vc[], int n, char letra) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (vc[i] == letra) {
            cant++;
        }
    }
    return cant;
}

void mostrarVectores(const int va[], const int vb[], const char vc[], int n) {
    int i;
    printf("   va -> [");
    for (i = 0; i < n; i++) printf("%2d ", va[i]);
    printf("]\n");

    printf("   vb -> [");
    for (i = 0; i < n; i++) printf("%2d ", vb[i]);
    printf("]\n");

    printf("   vc -> [");
    for (i = 0; i < n; i++) printf(" %c ", vc[i]);
    printf("]\n");
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
    int va[TAM], vb[TAM];
    char vc[TAM];
    int cantA, cantB, cantIgual;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 12: COMPARAR VECTORES va Y vb EN vc ('A','B','=')\n");
        printf("==========================================================\n\n");

        cargarVector(va, TAM);
        cargarVector(vb, TAM);
        construirVectorComparacion(va, vb, vc, TAM);

        mostrarVectores(va, vb, vc, TAM);

        cantA = contarLetra(vc, TAM, 'A');
        cantB = contarLetra(vc, TAM, 'B');
        cantIgual = contarLetra(vc, TAM, '=');

        printf("\n-> Cantidad de 'A' (va > vb): %d\n", cantA);
        printf("-> Cantidad de 'B' (vb > va): %d\n", cantB);
        printf("-> Cantidad de '=' (iguales): %d\n", cantIgual);

        printf("\nDesea generar otros vectores? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
