/*
 * Ejercicio 4 (Funciones y procedimientos): Vector binario e inversion
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Permite ingresar (manualmente validando solo 0 y 1, o al azar) un
 *   vector de numeros binarios (0 y 1) y crea otro vector invirtiendo
 *   los numeros del primer vector (muestra tanto el complemento logico
 *   0->1, 1->0 como el orden invertido de las posiciones).
 *
 * COMO FUNCIONA:
 *   - El usuario elige el tamano N (1 a 20) y modo de carga (1=Manual
 *     aceptando solo teclas '0' y '1', 2=Aleatoria).
 *   - invertirBits(): cambia cada 0 por 1 y cada 1 por 0.
 *   - invertirOrden(): invierte el orden de los elementos del vector.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int : Vectores vOrig[20], vComp[20], vReverso[20], tamano N.
 *   - char: Opcion de modo de carga.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define MAX_TAM 20

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

int leerBit(int pos) {
    int c;
    printf("   Ingrese bit para posicion [%2d] (solo 0 o 1): ", pos);
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        if (c == '0' || c == '1') {
            printf("%c\n", c);
            return c - '0';
        }
    }
}

void cargarVectorBinario(int v[], int n, int modo) {
    int i;
    for (i = 0; i < n; i++) {
        if (modo == 1) {
            v[i] = leerBit(i);
        } else {
            v[i] = rand() % 2;
        }
    }
}

void invertirBits(const int orig[], int dest[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = (orig[i] == 0) ? 1 : 0;
    }
}

void invertirOrden(const int orig[], int dest[], int n) {
    int i;
    for (i = 0; i < n; i++) {
        dest[i] = orig[n - 1 - i];
    }
}

void mostrarVector(const char *titulo, const int v[], int n) {
    int i;
    printf("%-36s: [", titulo);
    for (i = 0; i < n; i++) {
        printf(" %d ", v[i]);
    }
    printf("]\n");
}

int main(void) {
    int vOrig[MAX_TAM], vComp[MAX_TAM], vOrdenInv[MAX_TAM];
    int n, modo;

    srand((unsigned int)time(NULL));

    printf("==========================================================\n");
    printf("   FUNCIONES 04: VECTOR DE NUMEROS BINARIOS INVERTIDO\n");
    printf("==========================================================\n\n");

    n = leerEnteroRango("Ingrese la cantidad de elementos del vector binario", 1, MAX_TAM);
    modo = leerEnteroRango("Modo de carga (1 = Manual tecla a tecla, 2 = Al azar)", 1, 2);
    printf("\n");

    cargarVectorBinario(vOrig, n, modo);
    invertirBits(vOrig, vComp, n);
    invertirOrden(vOrig, vOrdenInv, n);

    printf("\nResultados:\n");
    mostrarVector("Vector original", vOrig, n);
    mostrarVector("Vector invertido (0->1 y 1->0)", vComp, n);
    mostrarVector("Vector en orden inverso", vOrdenInv, n);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
