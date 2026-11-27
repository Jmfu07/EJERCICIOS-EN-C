/*
 * Ejercicio 8 (Funciones y procedimientos): Dos vectores v1 y v2 y cantidad de pares
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Ingresa numeros al azar (del 1 al 9) a dos vectores de nombre v1 y v2,
 *   los muestra por pantalla y luego halla y muestra la cantidad de numeros
 *   pares que se encuentran en cada vector.
 *
 * COMO FUNCIONA:
 *   - Solicita la dimension N (por defecto entre 1 y 30, ej. 20) validada
 *     con _getch().
 *   - Reutiliza las mismas funciones cargarVector(), mostrarVector() y
 *     contarPares() para ambos vectores v1 y v2.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int: Vectores v1[30], v2[30], dimension n y contadores de pares.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAX_TAM 30

int leerEnteroRango(const char *mensaje, int min, int max) {
    char buf[16];
    int idx, c, val;
    while (1) {
        printf("%s (%d a %d): ", mensaje, min, max);
        idx = 0;
        while (1) {
            c = _getch();
            if (c == 0 || c == 224) {
                _getch();
                continue;
            }
            if (c == '\r' && idx > 0) {
                buf[idx] = '\0';
                printf("\n");
                break;
            } else if (c == '\b' && idx > 0) {
                idx--;
                printf("\b \b");
            } else if (c >= '0' && c <= '9' && idx < 9) {
                buf[idx++] = (char)c;
                printf("%c", c);
            }
        }
        val = atoi(buf);
        if (val >= min && val <= max) {
            return val;
        }
        printf("   [!] Error: ingrese un numero entre %d y %d.\n", min, max);
    }
}

void cargarVector(int v[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        v[i] = 1 + rand() % 9;
    }
}

void mostrarVector(const char *nombre, const int v[], int n) {
    int i;
    printf("   %s: [", nombre);
    for (i = 0; i < n; i++) {
        printf(" %d ", v[i]);
    }
    printf("]\n");
}

int contarPares(const int v[], int n) {
    int i, cant = 0;
    for (i = 0; i < n; i++) {
        if (v[i] % 2 == 0) {
            cant++;
        }
    }
    return cant;
}

int main(void) {
    int v1[MAX_TAM], v2[MAX_TAM];
    int n;

    srand((unsigned int)time(NULL));

    printf("==========================================================\n");
    printf("   FUNCIONES 08: DOS VECTORES v1 Y v2 (PARES EN C/U)\n");
    printf("==========================================================\n\n");

    n = leerEnteroRango("Ingrese la cantidad de elementos para v1 y v2", 1, MAX_TAM);

    cargarVector(v1, n);
    cargarVector(v2, n);

    printf("\nVectores generados (azar 1 al 9):\n");
    mostrarVector("v1", v1, n);
    mostrarVector("v2", v2, n);

    printf("\n-> Cantidad de numeros pares en v1: %d\n", contarPares(v1, n));
    printf("-> Cantidad de numeros pares en v2: %d\n", contarPares(v2, n));

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
