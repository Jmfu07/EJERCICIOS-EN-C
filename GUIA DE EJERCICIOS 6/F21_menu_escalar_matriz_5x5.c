/*
 * Ejercicio 21 (Funciones y procedimientos): Menu de operaciones con escalar en matriz 5x5
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Genera una matriz de 5x5 con numeros aleatorios del 0 al 9 y presenta
 *   un menu de opciones para operar cada elemento de la matriz con un
 *   escalar ingresado por el usuario:
 *     A - Sumar escalar
 *     B - Restar escalar
 *     C - Multiplicar por escalar
 *     D - Dividir por escalar (validando que no sea cero)
 *     X - Salir
 *
 * COMO FUNCIONA:
 *   - Lee la opcion con _getch() aceptando A, B, C, D o X.
 *   - Solicita el escalar validando entrada numerica tecla a tecla.
 *   - Calcula y muestra la matriz resultante de 5x5 para la operacion elegida.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - int   : Matriz m[5][5] e indices.
 *   - double: Escalar ingresado y matriz resultante para soportar division.
 *   - char  : Opcion seleccionada en el menu.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <ctype.h>
#include <math.h>
#include <conio.h>

double leerEscalar(const char *mensaje, int permitirCero) {
    char buf[32];
    int idx, tienePunto, c;
    double val;
    while (1) {
        printf("%s: ", mensaje);
        idx = 0;
        tienePunto = 0;
        while (1) {
            c = _getch();
            if (c == 0 || c == 224) {
                _getch();
                continue;
            }
            if (c == '\r' && idx > 0 && !(idx == 1 && (buf[0] == '-' || buf[0] == '.')) &&
                !(idx == 2 && buf[0] == '-' && buf[1] == '.')) {
                buf[idx] = '\0';
                printf("\n");
                break;
            } else if (c == '\b' && idx > 0) {
                idx--;
                if (buf[idx] == '.') tienePunto = 0;
                printf("\b \b");
            } else if (c == '-' && idx == 0) {
                buf[idx++] = '-';
                printf("-");
            } else if ((c == '.' || c == ',') && !tienePunto && idx < 25) {
                buf[idx++] = '.';
                tienePunto = 1;
                printf(".");
            } else if (c >= '0' && c <= '9' && idx < 25) {
                buf[idx++] = (char)c;
                printf("%c", c);
            }
        }
        val = atof(buf);
        if (!permitirCero && fabs(val) < 1e-12) {
            printf("   [!] Error: no se puede dividir por cero.\n");
            continue;
        }
        return val;
    }
}

char leerOpcionMenu(void) {
    int c;
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        c = toupper(c);
        if (c == 'A' || c == 'B' || c == 'C' || c == 'D' || c == 'X') {
            printf("%c\n", c);
            return (char)c;
        }
    }
}

void cargarMatriz(int m[5][5]) {
    int i, j;
    for (i = 0; i < 5; i++) {
        for (j = 0; j < 5; j++) {
            m[i][j] = rand() % 10;
        }
    }
}

void mostrarMatrizOriginal(int m[5][5]) {
    int i, j;
    printf("\nMatriz Original (5x5):\n\n");
    for (i = 0; i < 5; i++) {
        printf("   |");
        for (j = 0; j < 5; j++) {
            printf(" %3d ", m[i][j]);
        }
        printf("|\n");
    }
}

void operarConEscalar(int m[5][5], double escalar, char op) {
    int i, j;
    double res;
    printf("\nMatriz Resultante de 5x5:\n\n");
    for (i = 0; i < 5; i++) {
        printf("   |");
        for (j = 0; j < 5; j++) {
            if (op == 'A') res = m[i][j] + escalar;
            else if (op == 'B') res = m[i][j] - escalar;
            else if (op == 'C') res = m[i][j] * escalar;
            else res = (double)m[i][j] / escalar;
            printf(" %7.2f ", res);
        }
        printf("|\n");
    }
}

int main(void) {
    int m[5][5];
    double escalar;
    char opcion;

    srand((unsigned int)time(NULL));
    cargarMatriz(m);

    printf("==========================================================\n");
    printf("   FUNCIONES 21: OPERACIONES CON ESCALAR EN MATRIZ 5x5\n");
    printf("==========================================================\n");

    mostrarMatrizOriginal(m);

    do {
        printf("\n------------------ MENU DE OPCIONES ------------------\n");
        printf("  A - Sumar un escalar a la matriz\n");
        printf("  B - Restar un escalar a la matriz\n");
        printf("  C - Multiplicar la matriz por un escalar\n");
        printf("  D - Dividir la matriz por un escalar\n");
        printf("  X - Salir\n");
        printf("------------------------------------------------------\n");
        printf("Seleccione una opcion (A/B/C/D/X): ");
        opcion = leerOpcionMenu();

        if (opcion != 'X') {
            escalar = leerEscalar("Ingrese el valor del escalar", opcion != 'D');
            operarConEscalar(m, escalar, opcion);
        } else {
            printf("\nSaliendo del programa...\n");
        }
    } while (opcion != 'X');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
