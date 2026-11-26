/*
 * Ejercicio 2 (Funciones y procedimientos): Menu de triangulo rectangulo
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Solicita los dos catetos 'a' y 'b' de un triangulo rectangulo y
 *   muestra un menu con las opciones:
 *     1 - Area
 *     2 - Perimetro
 *     3 - Hipotenusa
 *     4 - Salir
 *   Utiliza la libreria <math.h>.
 *
 * COMO FUNCIONA:
 *   - Valida que ambos catetos 'a' y 'b' sean reales positivos (> 0)
 *     mediante captura tecla a tecla con _getch().
 *   - Calcula hipotenusa = sqrt(a*a + b*b), area = (a*b)/2 y
 *     perimetro = a + b + hipotenusa en funciones separadas.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - double: Catetos a y b, area, perimetro e hipotenusa.
 *   - char  : Opcion del menu ('1' a '4').
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <conio.h>

double leerDoublePositivo(const char *mensaje) {
    char buf[32];
    int idx, tienePunto, c;
    double val;
    while (1) {
        printf("%s (> 0): ", mensaje);
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
        if (val > 0.0) {
            return val;
        }
        printf("   [!] Error: el cateto debe ser mayor que 0.\n");
    }
}

char leerOpcion1a4(void) {
    int c;
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        if (c >= '1' && c <= '4') {
            printf("%c\n", c);
            return (char)c;
        }
    }
}

double calcularHipotenusa(double a, double b) {
    return sqrt(a * a + b * b);
}

double calcularArea(double a, double b) {
    return (a * b) / 2.0;
}

double calcularPerimetro(double a, double b) {
    return a + b + calcularHipotenusa(a, b);
}

int main(void) {
    double a, b;
    char opcion;

    printf("==========================================================\n");
    printf("   FUNCIONES 02: MENU DE TRIANGULO RECTANGULO (a y b)\n");
    printf("==========================================================\n\n");

    a = leerDoublePositivo("Ingrese la longitud del cateto a");
    b = leerDoublePositivo("Ingrese la longitud del cateto b");

    do {
        printf("\n------------------ MENU DE OPCIONES ------------------\n");
        printf("  1 - Area       (catetos a=%.2f, b=%.2f)\n", a, b);
        printf("  2 - Perimetro  (catetos a=%.2f, b=%.2f)\n", a, b);
        printf("  3 - Hipotenusa (catetos a=%.2f, b=%.2f)\n", a, b);
        printf("  4 - Salir\n");
        printf("------------------------------------------------------\n");
        printf("Seleccione una opcion (1-4): ");
        opcion = leerOpcion1a4();

        switch (opcion) {
            case '1':
                printf("\n   => Area del triangulo      = %.4f\n", calcularArea(a, b));
                break;
            case '2':
                printf("\n   => Perimetro del triangulo = %.4f\n", calcularPerimetro(a, b));
                break;
            case '3':
                printf("\n   => Hipotenusa              = %.4f\n", calcularHipotenusa(a, b));
                break;
            case '4':
                printf("\nSaliendo del programa...\n");
                break;
        }
    } while (opcion != '4');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
