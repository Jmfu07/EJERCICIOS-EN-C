/*
 * Ejercicio 10 (Funciones y procedimientos): Separar impares y pares en vi y vp
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera un vector "v" con 20 numeros enteros al azar del 1 al 10.
 *   Usando dos vectores "vi" y "vp" de 20 elementos, llena el primero con
 *   los numeros impares de "v" y el segundo con los pares (de forma
 *   compacta al inicio de cada vector) y muestra los tres vectores sin
 *   imprimir la "basura" sobrante al final de "vi" y "vp".
 *
 * COMO FUNCIONA:
 *   - separarImparesPares(): recorre "v" y usa dos contadores independientes
 *     (cantI y cantP) para insertar al principio de vi y vp.
 *   - mostrarVector(): recibe el limite exacto de elementos validos para
 *     no mostrar posiciones no inicializadas.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Vectores v[20], vi[20], vp[20], contadores cantI y cantP.
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

void separarImparesPares(const int v[], int n, int vi[], int *cantI, int vp[], int *cantP) {
    int i;
    *cantI = 0;
    *cantP = 0;
    for (i = 0; i < n; i++) {
        if (v[i] % 2 != 0) {
            vi[*cantI] = v[i];
            (*cantI)++;
        } else {
            vp[*cantP] = v[i];
            (*cantP)++;
        }
    }
}

void mostrarVector(const char *nombre, const int vec[], int cantidad) {
    int i;
    printf("   %-18s (%2d elem.): [", nombre, cantidad);
    for (i = 0; i < cantidad; i++) {
        printf("%2d%s", vec[i], (i < cantidad - 1) ? ", " : "");
    }
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
    int v[TAM], vi[TAM], vp[TAM];
    int cantI, cantP;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 10: SEPARAR IMPARES (vi) Y PARES (vp) SIN BASURA\n");
        printf("==========================================================\n\n");

        cargarVector(v, TAM);
        separarImparesPares(v, TAM, vi, &cantI, vp, &cantP);

        mostrarVector("Vector original v", v, TAM);
        mostrarVector("Vector impares vi", vi, cantI);
        mostrarVector("Vector pares   vp", vp, cantP);

        printf("\nDesea generar otro vector? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
