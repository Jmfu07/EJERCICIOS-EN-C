/*
 * Ejercicio 23 (Recursividad): Producto recursivo a x b con numeros naturales
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Calcula el producto a * b de dos numeros naturales ingresados por el
 *   usuario aplicando recursividad mediante la relacion:
 *     producto(a, b) = a + producto(a, b - 1)
 *
 * COMO FUNCIONA:
 *   - Valida que 'a' y 'b' sean numeros naturales (enteros >= 0, o >= 1)
 *     leidos tecla por tecla con _getch().
 *   - Caso base: si b == 0 retorna 0; si b == 1 retorna a.
 *   - Paso recursivo: retorna a + producto(a, b - 1).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - long long: Resultado del producto para evitar desbordamiento.
 *   - int      : Numeros naturales a y b.
 */

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

int leerNatural(const char *mensaje, int min, int max) {
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
        printf("   [!] Error: debe ingresar un numero natural entre %d y %d.\n", min, max);
    }
}

long long producto(int a, int b) {
    if (b == 0) {
        return 0;
    }
    if (b == 1) {
        return a;
    }
    return (long long)a + producto(a, b - 1);
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
    int a, b;
    char repetir;

    do {
        printf("\n==========================================================\n");
        printf("      RECURSIVIDAD 23: PRODUCTO RECURSIVO (a x b)\n");
        printf("==========================================================\n\n");

        a = leerNatural("Ingrese el numero natural a", 1, 10000);
        b = leerNatural("Ingrese el numero natural b", 1, 5000);

        printf("\n-> Resultado recursivo: %d x %d = %lld\n", a, b, producto(a, b));

        printf("\nDesea calcular otro producto? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
