/*
 * Ejercicio 6 (Funciones y procedimientos): Vector de 20 tiros de dado
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Carga en un vector de 20 posiciones numeros de un dado al azar (1 al 6),
 *   cuenta la cantidad de veces que salio el 6 y sustituye todos los 5 por 3.
 *
 * COMO FUNCIONA:
 *   - cargarDado(): asigna 1 + rand() % 6 a cada una de las 20 posiciones.
 *   - contarSeis(): retorna cuantos elementos valen 6.
 *   - sustituirCincoPorTres(): reemplaza cada 5 por un 3 y devuelve cuantos
 *     reemplazos se hicieron.
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

void cargarDado(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = 1 + rand() % 6;
    }
}

void mostrarVector(const char *titulo, const int v[], int n) {
    int i;
    printf("%-34s: [", titulo);
    for (i = 0; i < n; i++) {
        printf(" %d ", v[i]);
    }
    printf("]\n");
}

int contarSeis(const int v[], int n) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (v[i] == 6) {
            cant++;
        }
    }
    return cant;
}

int sustituirCincoPorTres(int v[], int n) {
    int i, cambios = 0;
    for (i = 0; i < n; i++) {
        if (v[i] == 5) {
            v[i] = 3;
            cambios++;
        }
    }
    return cambios;
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
    int cant6, cambios;
    char repetir;

    srand((unsigned int)time(NULL));

    do {
        printf("\n==========================================================\n");
        printf("  FUNCIONES 06: DADO AL AZAR (CONTAR 6, CAMBIAR 5 POR 3)\n");
        printf("==========================================================\n\n");

        cargarDado(v, TAM);
        mostrarVector("Vector original (dado 1..6)", v, TAM);

        cant6 = contarSeis(v, TAM);
        cambios = sustituirCincoPorTres(v, TAM);

        mostrarVector("Vector modificado (5 -> 3)", v, TAM);

        printf("\n-> Cantidad de numeros 6 encontrados : %d\n", cant6);
        printf("-> Cantidad de numeros 5 sustituidos : %d\n", cambios);

        printf("\nDesea realizar otra tirada? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
