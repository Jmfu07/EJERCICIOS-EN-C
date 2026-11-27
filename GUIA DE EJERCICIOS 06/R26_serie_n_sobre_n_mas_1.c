/*
 * Ejercicio 26 (Recursividad): Serie 1/2 + 2/3 + 3/4 + ... + (n-1)/n
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Calcula de forma recursiva la serie:
 *     1/2 + 2/3 + 3/4 + ... + (n - 1)/n
 *   sabiendo que n (natural >= 2) lo ingresa el usuario.
 *
 * COMO FUNCIONA:
 *   - Valida que n sea un numero natural >= 2 con _getch().
 *   - Caso base: cuando n == 2, el unico termino es (2-1)/2 = 1/2 = 0.5.
 *   - Paso recursivo: serie(n - 1) + (double)(n - 1) / (double)n.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - double: Resultado acumulado de la serie.
 *   - int   : Numero natural n ingresado por el usuario.
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

double serieFracciones(int n) {
    if (n <= 2) {
        return 1.0 / 2.0;
    }
    return serieFracciones(n - 1) + ((double)(n - 1) / (double)n);
}

void mostrarTerminos(int n) {
    int i;
    int limite = (n <= 12) ? n : 12;
    printf("   Serie: ");
    for (i = 2; i <= limite; i++) {
        printf("%d/%d%s", i - 1, i, (i < n) ? " + " : "");
    }
    if (n > 12) {
        printf("... + %d/%d", n - 1, n);
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
        printf("  RECURSIVIDAD 26: SERIE 1/2 + 2/3 + 3/4 + ... + (n-1)/n\n");
        printf("==========================================================\n\n");

        n = leerNatural("Ingrese el numero natural n (minimo 2)", 2, 5000);
        mostrarTerminos(n);

        printf("\n-> Resultado de la serie hasta (%d/%d): %.6f\n",
               n - 1, n, serieFracciones(n));

        printf("\nDesea calcular con otro valor de n? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
