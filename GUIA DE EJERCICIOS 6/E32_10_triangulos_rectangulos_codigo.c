/*
 * Ejercicio 32 (Estructura de datos): 10 triangulos rectangulos con codigo 'A'..'J'
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Crea un arreglo de 10 estructuras que almacenan:
 *     - codigo: tipo char ('A', 'B', 'C', ..., 'J')
 *     - lados enteros 'a' y 'b' obtenidos al azar en el rango [1, 100]
 *     - hipotenusa calculada con sqrt(a*a + b*b)
 *   Al ingresar un codigo ('A'..'J'), el usuario puede mostrar y editar
 *   los datos del triangulo correspondiente.
 *
 * COMO FUNCIONA:
 *   - Genera automaticamente los 10 triangulos con codigos 'A' a 'J'.
 *   - Presenta un menu interactivo para:
 *       1 - Mostrar todos los triangulos ('A'..'J')
 *       2 - Consultar / Mostrar un triangulo por su codigo
 *       3 - Editar los catetos a y b de un triangulo por su codigo
 *       4 - Salir
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - struct Triangulo: char codigo; int a; int b; double hipotenusa.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <conio.h>

#define CANT_TRIANGULOS 10

typedef struct {
    char codigo;
    int a;
    int b;
    double hipotenusa;
} Triangulo;

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
        printf("   [!] Error: ingrese un entero entre %d y %d.\n", min, max);
    }
}

char leerCodigoTriangulo(void) {
    int c;
    printf("Ingrese el codigo del triangulo ('A' a 'J'): ");
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        c = toupper(c);
        if (c >= 'A' && c <= 'J') {
            printf("%c\n", c);
            return (char)c;
        }
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

void inicializarTriangulos(Triangulo t[]) {
    int i;
    for (i = 0; i < CANT_TRIANGULOS; i++) {
        t[i].codigo = (char)('A' + i);
        t[i].a = 1 + rand() % 100;
        t[i].b = 1 + rand() % 100;
        t[i].hipotenusa = sqrt((double)(t[i].a * t[i].a + t[i].b * t[i].b));
    }
}

void mostrarTodos(const Triangulo t[]) {
    int i;
    printf("\nListado de los 10 Triangulos Rectangulos:\n");
    printf("------------------------------------------------\n");
    printf(" Codigo | Cateto a | Cateto b |   Hipotenusa\n");
    printf("--------+----------+----------+-----------------\n");
    for (i = 0; i < CANT_TRIANGULOS; i++) {
        printf("   %c    |   %4d   |   %4d   |    %9.4f\n",
               t[i].codigo, t[i].a, t[i].b, t[i].hipotenusa);
    }
    printf("------------------------------------------------\n");
}

void mostrarUno(const Triangulo *t) {
    printf("\n   [Triangulo '%c'] -> Cateto a = %d | Cateto b = %d | Hipotenusa = %.4f\n",
           t->codigo, t->a, t->b, t->hipotenusa);
}

int main(void) {
    Triangulo t[CANT_TRIANGULOS];
    char opcion, cod;
    int idx;

    srand((unsigned int)time(NULL));
    inicializarTriangulos(t);

    printf("==========================================================\n");
    printf("  ESTRUCTURAS 32: 10 TRIANGULOS RECTANGULOS ('A' A 'J')\n");
    printf("==========================================================\n");

    mostrarTodos(t);

    do {
        printf("\n------------------ MENU DE OPCIONES ------------------\n");
        printf("  1 - Mostrar tabla completa de los 10 triangulos\n");
        printf("  2 - Buscar y mostrar un triangulo por codigo ('A'..'J')\n");
        printf("  3 - Buscar y EDITAR un triangulo por codigo ('A'..'J')\n");
        printf("  4 - Salir\n");
        printf("------------------------------------------------------\n");
        printf("Seleccione una opcion (1-4): ");
        opcion = leerOpcion1a4();

        switch (opcion) {
            case '1':
                mostrarTodos(t);
                break;
            case '2':
                cod = leerCodigoTriangulo();
                idx = cod - 'A';
                mostrarUno(&t[idx]);
                break;
            case '3':
                cod = leerCodigoTriangulo();
                idx = cod - 'A';
                mostrarUno(&t[idx]);
                printf("   --- Editar lados del triangulo '%c' ---\n", cod);
                t[idx].a = leerEnteroRango("   Nuevo valor para cateto a", 1, 100);
                t[idx].b = leerEnteroRango("   Nuevo valor para cateto b", 1, 100);
                t[idx].hipotenusa = sqrt((double)(t[idx].a * t[idx].a + t[idx].b * t[idx].b));
                printf("   [OK] Triangulo actualizado correctamente:");
                mostrarUno(&t[idx]);
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
