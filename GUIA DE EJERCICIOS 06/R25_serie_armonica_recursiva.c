/*
 * Ejercicio 25 (Recursividad): Serie armonica 1/1 + 1/2 + 1/3 + ... + 1/n
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Calcula de forma recursiva la suma de la serie armonica:
 *     S(n) = 1/1 + 1/2 + 1/3 + ... + 1/n
 *   donde n es un numero natural ingresado por el usuario.
 *
 * COMO FUNCIONA:
 *   - Valida que n sea un natural en [1, 5000] con _getch().
 *   - Caso base: si n == 1, retorna 1.0.
 *   - Paso recursivo: serieArmonica(n - 1) + (1.0 / n).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - double: Valor acumulado de la serie de fracciones.
 *   - int   : Termino natural n.
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
        printf("   [!] Error: ingrese un numero natural entre %d y %d.\n", min, max);
    }
}

double serieArmonica(int n) {
    if (n <= 1) {
        return 1.0;
    }
    return serieArmonica(n - 1) + (1.0 / (double)n);
}

void mostrarTerminos(int n) {
    int i;
    int limite = (n <= 12) ? n : 12;
    printf("   Serie: ");
    for (i = 1; i <= limite; i++) {
        printf("1/%d%s", i, (i < n) ? " + " : "");
    }
    if (n > 12) {
        printf("... + 1/%d", n);
    }
    printf("\n");
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
    int n;
    char repetir;

    do {
        printf("\n==========================================================\n");
        printf("  RECURSIVIDAD 25: SERIE 1/1 + 1/2 + 1/3 + ... + 1/n\n");
        printf("==========================================================\n\n");

        n = leerNatural("Ingrese el numero natural n", 1, 5000);
        mostrarTerminos(n);

        printf("\n-> Resultado de la serie para n = %d: %.6f\n", n, serieArmonica(n));

        printf("\nDesea calcular con otro valor de n? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
