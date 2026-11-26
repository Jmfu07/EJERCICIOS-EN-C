/*
 * Ejercicio 34 (Estructura de datos): 100 triangulos y clasificacion automatica
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Almacena hasta 100 triangulos cuyos datos son:
 *     - Codigo (tres digitos: 100 a 999)
 *     - Longitudes en cm de sus tres lados A, B y C
 *     - Tipo determinado AUTOMATICAMENTE por el programa:
 *         'E' - Escaleno   (3 lados distintos)
 *         'Q' - Equilatero (3 lados iguales)
 *         'I' - Isosceles  (2 lados iguales)
 *   Muestra los datos en una tabla con el formato del enunciado:
 *     Codigo   Lado A   Lado B   Lado C   Tipo
 *     102      4,10     5,00     4,10     I
 *     117      7,15     1,00     3,80     E
 *
 * COMO FUNCIONA:
 *   - El usuario ingresa el codigo de 3 digitos y los lados A, B y C
 *     (o puede usar el modo de generación asistida para probar 100 registros).
 *   - clasificarTriangulo() compara los lados A, B y C con tolerancia
 *     de punto flotante (1e-6) y devuelve 'Q', 'I' o 'E'.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - struct TrianguloClasif: int codigo; double a, b, c; char tipo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <conio.h>

#define MAX_TRIANGULOS 100

typedef struct {
    int codigo;
    double a;
    double b;
    double c;
    char tipo;
} TrianguloClasif;

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
        printf("   [!] Error: ingrese un valor entre %d y %d.\n", min, max);
    }
}

double leerLadoPositivo(const char *mensaje) {
    char buf[32];
    int idx, tienePunto, c;
    double val;
    while (1) {
        printf("%s (cm > 0): ", mensaje);
        idx = 0;
        tienePunto = 0;
        while (1) {
            c = _getch();
            if (c == 0 || c == 224) {
                _getch();
                continue;
            }
            if (c == '\r' && idx > 0 && !(idx == 1 && buf[0] == '.')) {
                buf[idx] = '\0';
                printf("\n");
                break;
            } else if (c == '\b' && idx > 0) {
                idx--;
                if (buf[idx] == '.') tienePunto = 0;
                printf("\b \b");
            } else if ((c == '.' || c == ',') && !tienePunto && idx < 20) {
                buf[idx++] = '.';
                tienePunto = 1;
                printf(".");
            } else if (c >= '0' && c <= '9' && idx < 20) {
                buf[idx++] = (char)c;
                printf("%c", c);
            }
        }
        val = atof(buf);
        if (val > 0.0) {
            return val;
        }
        printf("   [!] Error: la longitud del lado debe ser mayor a 0.\n");
    }
}

int iguales(double x, double y) {
    return fabs(x - y) < 1e-6;
}

char clasificarTriangulo(double a, double b, double c) {
    if (iguales(a, b) && iguales(b, c)) {
        return 'Q';
    } else if (iguales(a, b) || iguales(a, c) || iguales(b, c)) {
        return 'I';
    } else {
        return 'E';
    }
}

int main(void) {
    TrianguloClasif t[MAX_TRIANGULOS];
    int cant, modo, i, sorteo;

    srand((unsigned int)time(NULL));

    printf("==============================================================\n");
    printf("  ESTRUCTURAS 34: 100 TRIANGULOS (CODIGO, LADOS Y TIPO E/Q/I)\n");
    printf("==============================================================\n\n");

    cant = leerEnteroRango("Cantidad de triangulos a procesar (100 segun enunciado)", 1, MAX_TRIANGULOS);
    modo = leerEnteroRango("Modo de carga (1 = Manual por teclado, 2 = Automatica de prueba)", 1, 2);

    for (i = 0; i < cant; i++) {
        if (modo == 1) {
            printf("\n--- Triangulo #%d ---\n", i + 1);
            t[i].codigo = leerEnteroRango("   Ingrese Codigo (3 digitos)", 100, 999);
            t[i].a = leerLadoPositivo("   Ingrese Lado A");
            t[i].b = leerLadoPositivo("   Ingrese Lado B");
            t[i].c = leerLadoPositivo("   Ingrese Lado C");
        } else {
            t[i].codigo = 100 + i;
            sorteo = rand() % 3;
            if (sorteo == 0) {
                /* Equilatero */
                t[i].a = (double)(100 + rand() % 900) / 100.0;
                t[i].b = t[i].a;
                t[i].c = t[i].a;
            } else if (sorteo == 1) {
                /* Isosceles */
                t[i].a = (double)(100 + rand() % 900) / 100.0;
                t[i].b = (double)(100 + rand() % 900) / 100.0;
                t[i].c = t[i].a;
            } else {
                /* Escaleno */
                t[i].a = (double)(100 + rand() % 900) / 100.0;
                t[i].b = (double)(100 + rand() % 900) / 100.0;
                t[i].c = (double)(100 + rand() % 900) / 100.0;
            }
        }
        t[i].tipo = clasificarTriangulo(t[i].a, t[i].b, t[i].c);
    }

    printf("\n%-8s %-10s %-10s %-10s %-6s\n", "Codigo", "Lado A", "Lado B", "Lado C", "Tipo");
    printf("------------------------------------------------\n");
    for (i = 0; i < cant; i++) {
        printf("%-8d %-10.2f %-10.2f %-10.2f %-6c\n",
               t[i].codigo, t[i].a, t[i].b, t[i].c, t[i].tipo);
    }
    printf("------------------------------------------------\n");
    printf("(Referencias: E = Escaleno, Q = Equilatero, I = Isosceles)\n");

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
