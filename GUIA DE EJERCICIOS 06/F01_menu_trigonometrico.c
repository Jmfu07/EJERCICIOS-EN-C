/*
 * Ejercicio 1 (Funciones y procedimientos): Menu trigonometrico
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Solicita el ingreso de un angulo en grados sexagesimales y presenta
 *   un menu en pantalla que permite calcular:
 *     A - Seno
 *     B - Coseno
 *     C - Tangente
 *     D - Salir
 *   Realiza el pasaje de grados a radianes y utiliza <math.h>.
 *
 * COMO FUNCIONA:
 *   - Lee el angulo en grados validando tecla por tecla con _getch()
 *     (admite signo negativo y punto decimal).
 *   - Define funciones dedicadas: gradosARadianes(), calcularSeno(),
 *     calcularCoseno() y calcularTangente().
 *   - El menu solo acepta las teclas A, B, C o D (mayusculas o minusculas).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - double: Angulo en grados, radianes y resultado trigonometrico.
 *   - char  : Opcion seleccionada en el menu.
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>

#define PI 3.14159265358979323846

double leerDouble(const char *mensaje) {
    char buf[32];
    int idx = 0, tienePunto = 0, c;
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
                return atof(buf);
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
    }
}

char leerOpcionABCD(void) {
    int c;
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        c = toupper(c);
        if (c >= 'A' && c <= 'D') {
            printf("%c\n", c);
            return (char)c;
        }
    }
}

double gradosARadianes(double grados) {
    return grados * (PI / 180.0);
}

double calcularSeno(double grados) {
    return sin(gradosARadianes(grados));
}

double calcularCoseno(double grados) {
    return cos(gradosARadianes(grados));
}

double calcularTangente(double grados) {
    return tan(gradosARadianes(grados));
}

int main(void) {
    double grados, rad, coseno;
    char opcion;

    printf("==========================================================\n");
    printf("   FUNCIONES 01: MENU TRIGONOMETRICO (SENO/COSENO/TAN)\n");
    printf("==========================================================\n\n");

    grados = leerDouble("Ingrese el angulo en grados");
    rad = gradosARadianes(grados);
    printf("-> Angulo ingresado: %.4f grados (%.6f radianes)\n", grados, rad);

    do {
        printf("\n------------------ MENU DE OPCIONES ------------------\n");
        printf("  A - Hallar Seno     de %.2f grados\n", grados);
        printf("  B - Hallar Coseno   de %.2f grados\n", grados);
        printf("  C - Hallar Tangente de %.2f grados\n", grados);
        printf("  D - Salir\n");
        printf("------------------------------------------------------\n");
        printf("Seleccione una opcion (A/B/C/D): ");
        opcion = leerOpcionABCD();

        switch (opcion) {
            case 'A':
                printf("\n   => sen(%.2f grados) = %.6f\n", grados, calcularSeno(grados));
                break;
            case 'B':
                printf("\n   => cos(%.2f grados) = %.6f\n", grados, calcularCoseno(grados));
                break;
            case 'C':
                coseno = calcularCoseno(grados);
                if (fabs(coseno) < 1e-12) {
                    printf("\n   => tan(%.2f grados) = Indefinida (coseno = 0)\n", grados);
                } else {
                    printf("\n   => tan(%.2f grados) = %.6f\n", grados, calcularTangente(grados));
                }
                break;
            case 'D':
                printf("\nSaliendo del menu trigonometrico...\n");
                break;
        }
    } while (opcion != 'D');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
