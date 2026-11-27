/*
 * Ejercicio 33 (Estructura de datos): 100 registros con N1, N2 y TIPO ('P', 'I', 'M')
 * -------------------------------------------------------------------
 * QUE HACE:
 *   Almacena hasta 100 registros (la cantidad total por enunciado es 100,
 *   permitiendo elegir cargar los 100 o una cantidad N <= 100, manual o
 *   asistida) con tres campos:
 *     - N1 (int)
 *     - N2 (int)
 *     - TIPO (char): determinado AUTOMATICAMENTE por el programa:
 *         'P' si los dos numeros son pares
 *         'I' si los dos numeros son impares
 *         'M' si uno es par y el otro es impar
 *   Luego halla la sumatoria de los N1 (Todos) y el promedio de los N2
 *   (solo los del TIPO 'M') y muestra ambos resultados.
 *
 * COMO FUNCIONA:
 *   - El usuario solo ingresa N1 y N2 (o puede optar por carga rapida
 *     para probar los 100 registros de inmediato).
 *   - determinarTipo(n1, n2) devuelve 'P', 'I' o 'M' automaticamente.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - struct Registro: int n1; int n2; char tipo.
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <conio.h>

#define TOTAL_REGISTROS 100

typedef struct {
    int n1;
    int n2;
    char tipo;
} Registro;

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
            if (c == '\r' && idx > 0 && !(idx == 1 && buf[0] == '-')) {
                buf[idx] = '\0';
                printf("\n");
                break;
            } else if (c == '\b' && idx > 0) {
                idx--;
                printf("\b \b");
            } else if (c == '-' && idx == 0 && min < 0) {
                buf[idx++] = '-';
                printf("-");
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

char determinarTipo(int n1, int n2) {
    int par1 = (n1 % 2 == 0);
    int par2 = (n2 % 2 == 0);
    if (par1 && par2) {
        return 'P';
    } else if (!par1 && !par2) {
        return 'I';
    } else {
        return 'M';
    }
}

int main(void) {
    Registro reg[TOTAL_REGISTROS];
    int cant, modo, i;
    long long sumaN1 = 0;
    long long sumaN2TipoM = 0;
    int cantTipoM = 0;

    srand((unsigned int)time(NULL));

    printf("==============================================================\n");
    printf("  ESTRUCTURAS 33: 100 REGISTROS (N1, N2 Y TIPO AUTOMATICO)\n");
    printf("==============================================================\n\n");

    cant = leerEnteroRango("Cantidad de registros a procesar (100 segun enunciado)", 1, TOTAL_REGISTROS);
    modo = leerEnteroRango("Modo de carga de N1 y N2 (1 = Manual por teclado, 2 = Automatica de prueba)", 1, 2);

    for (i = 0; i < cant; i++) {
        if (modo == 1) {
            printf("\n--- Registro #%d ---\n", i + 1);
            reg[i].n1 = leerEnteroRango("   Ingrese N1", -10000, 10000);
            reg[i].n2 = leerEnteroRango("   Ingrese N2", -10000, 10000);
        } else {
            reg[i].n1 = 1 + rand() % 100;
            reg[i].n2 = 1 + rand() % 100;
        }
        reg[i].tipo = determinarTipo(reg[i].n1, reg[i].n2);

        sumaN1 += reg[i].n1;
        if (reg[i].tipo == 'M') {
            sumaN2TipoM += reg[i].n2;
            cantTipoM++;
        }
    }

    printf("\nTabla de Registros Cargados:\n");
    printf("----------------------------------------\n");
    printf("  #  |    N1    |    N2    | TIPO (Auto)\n");
    printf("-----+----------+----------+------------\n");
    for (i = 0; i < cant; i++) {
        printf(" %3d |  %6d  |  %6d  |     %c\n",
               i + 1, reg[i].n1, reg[i].n2, reg[i].tipo);
    }
    printf("----------------------------------------\n");

    printf("\nResultados solicitados:\n");
    printf("-> Sumatoria de los N1 (Todos los %d registros): %lld\n", cant, sumaN1);
    if (cantTipoM > 0) {
        printf("-> Promedio de los N2 (Solo los %d de TIPO 'M'): %.2f\n",
               cantTipoM, (double)sumaN2TipoM / (double)cantTipoM);
    } else {
        printf("-> Promedio de los N2 (Solo los de TIPO 'M')   : No hubo registros de tipo 'M'\n");
    }

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
