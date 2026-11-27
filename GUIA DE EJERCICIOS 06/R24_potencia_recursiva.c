/*
 * Ejercicio 24 (Recursividad): Potencia recursiva a^b con numeros naturales
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Calcula la potencia a^b de dos numeros naturales ingresados por el
 *   usuario utilizando una funcion recursiva.
 *
 * COMO FUNCIONA:
 *   - Valida 'a' y 'b' como numeros naturales con _getch().
 *   - Caso base: si b == 0 retorna 1; si b == 1 retorna a.
 *   - Paso recursivo: potencia(a, b) = a * potencia(a, b - 1).
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - unsigned long long: Para almacenar potencias grandes sin overflow.
 *   - int               : Base natural 'a' y exponente natural 'b'.
 */

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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
        printf("   [!] Error: ingrese un natural entre %d y %d.\n", min, max);
    }
}

unsigned long long potencia(int a, int b, int *overflow) {
    unsigned long long sub;
    if (b == 0) {
        return 1ULL;
    }
    if (b == 1) {
        return (unsigned long long)a;
    }
    sub = potencia(a, b - 1, overflow);
    if (*overflow || (a > 0 && sub > ULLONG_MAX / (unsigned long long)a)) {
        *overflow = 1;
        return 0ULL;
    }
    return (unsigned long long)a * sub;
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
        printf("       RECURSIVIDAD 24: POTENCIA RECURSIVA (a^b)\n");
        printf("==========================================================\n\n");

        a = leerNatural("Ingrese la base natural (a)", 1, 100);
        b = leerNatural("Ingrese el exponente natural (b)", 1, 18);

        {
            int overflow = 0;
            unsigned long long res = potencia(a, b, &overflow);
            if (overflow)
                printf("\n-> [Aviso] %d ^ %d supera el limite de unsigned long long (%.4e aprox).\n", a, b, 1.8446744e19);
            else
                printf("\n-> Resultado recursivo: %d ^ %d = %llu\n", a, b, res);
        }

        printf("\nDesea calcular otra potencia? (S/N): ");
        repetir = leerSiNo();
    } while (repetir == 's' || repetir == 'S');

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
