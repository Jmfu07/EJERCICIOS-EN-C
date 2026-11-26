/*
 * Ejercicios 27, 28 y 29 (Estructura de datos): Registro de N paises
 * -------------------------------------------------------------------
 * QUE HACE:
 *   - (Ej. 27) Llena un arreglo de estructuras de N paises con los campos:
 *       * Codigo internacional telefonico (int)
 *       * Continente: 'a'->America, 'b'->Asia, 'c'->Africa, 'e'->Europa, 'o'->Oceania
 *       * Poblacion (long long)
 *       * Superficie en km2 (double)
 *     y muestra todos los registros cargados.
 *   - (Ej. 28) Calcula y muestra la cantidad total de poblacion y el
 *     promedio total de superficie.
 *   - (Ej. 29) Ordena los registros de modo ascendente por Codigo
 *     internacional telefonico y muestra todos los datos ordenados.
 *
 * COMO FUNCIONA:
 *   - Valida cada campo tecla a tecla con _getch(), incluyendo que el
 *     continente solo pueda ser 'a', 'b', 'c', 'e' u 'o'.
 *   - Utiliza el metodo de la burbuja para ordenar el arreglo de structs
 *     por el campo `codigo`.
 *
 * TIPOS DE DATOS UTILIZADOS:
 *   - struct Pais: int codigo; char continente; long long poblacion; double superficie.
 */

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <conio.h>

#define MAX_PAISES 50

typedef struct {
    int codigo;
    char continente;
    long long poblacion;
    double superficie;
} Pais;

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

long long leerPoblacion(const char *mensaje) {
    char buf[20];
    int idx, c;
    long long val;
    while (1) {
        printf("%s (> 0): ", mensaje);
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
            } else if (c >= '0' && c <= '9' && idx < 15) {
                buf[idx++] = (char)c;
                printf("%c", c);
            }
        }
        val = atoll(buf);
        if (val > 0) {
            return val;
        }
        printf("   [!] Error: la poblacion debe ser mayor a 0.\n");
    }
}

double leerSuperficie(const char *mensaje) {
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
            } else if ((c == '.' || c == ',') && !tienePunto && idx < 24) {
                buf[idx++] = '.';
                tienePunto = 1;
                printf(".");
            } else if (c >= '0' && c <= '9' && idx < 24) {
                buf[idx++] = (char)c;
                printf("%c", c);
            }
        }
        val = atof(buf);
        if (val > 0.0) {
            return val;
        }
        printf("   [!] Error: la superficie debe ser mayor a 0.\n");
    }
}

char leerContinente(void) {
    int c;
    printf("   Continente ('a'=America, 'b'=Asia, 'c'=Africa, 'e'=Europa, 'o'=Oceania): ");
    while (1) {
        c = _getch();
        if (c == 0 || c == 224) {
            _getch();
            continue;
        }
        c = tolower(c);
        if (c == 'a' || c == 'b' || c == 'c' || c == 'e' || c == 'o') {
            printf("%c\n", c);
            return (char)c;
        }
    }
}

const char *nombreContinente(char c) {
    switch (c) {
        case 'a': return "America";
        case 'b': return "Asia";
        case 'c': return "Africa";
        case 'e': return "Europa";
        case 'o': return "Oceania";
        default : return "Desconocido";
    }
}

void mostrarPaises(const char *titulo, const Pais p[], int n) {
    int i;
    printf("\n%s:\n", titulo);
    printf("---------------------------------------------------------------------\n");
    printf(" # | Cod. Tel. | Continente       |    Poblacion    | Superficie (km2)\n");
    printf("---+-----------+------------------+-----------------+----------------\n");
    for (i = 0; i < n; i++) {
        printf("%2d |   +%4d   | (%c) %-12s | %15lld | %14.2f\n",
               i + 1, p[i].codigo, p[i].continente,
               nombreContinente(p[i].continente),
               p[i].poblacion, p[i].superficie);
    }
    printf("---------------------------------------------------------------------\n");
}

void ordenarPorCodigoAscendente(Pais p[], int n) {
    int i, j;
    Pais aux;
    for (i = 0; i < n - 1; i++) {
        for (j = 0; j < n - 1 - i; j++) {
            if (p[j].codigo > p[j + 1].codigo) {
                aux = p[j];
                p[j] = p[j + 1];
                p[j + 1] = aux;
            }
        }
    }
}

int main(void) {
    Pais paises[MAX_PAISES];
    int n, i;
    long long totalPoblacion = 0;
    double sumaSuperficie = 0.0, promSuperficie;

    printf("=====================================================================\n");
    printf("  ESTRUCTURAS 27, 28 Y 29: REGISTRO Y ORDENAMIENTO DE N PAISES\n");
    printf("=====================================================================\n\n");

    n = leerEnteroRango("Ingrese la cantidad de paises a registrar (N)", 1, MAX_PAISES);

    for (i = 0; i < n; i++) {
        printf("\n--- Datos del Pais #%d ---\n", i + 1);
        paises[i].codigo = leerEnteroRango("   Codigo internacional telefonico", 1, 9999);
        paises[i].continente = leerContinente();
        paises[i].poblacion = leerPoblacion("   Poblacion (cantidad de habitantes)");
        paises[i].superficie = leerSuperficie("   Superficie en km2");

        totalPoblacion += paises[i].poblacion;
        sumaSuperficie += paises[i].superficie;
    }

    /* Ejercicio 27: Mostrar registros cargados */
    mostrarPaises("EJERCICIO 27 - Paises registrados (orden de ingreso)", paises, n);

    /* Ejercicio 28: Total de poblacion y promedio de superficie */
    promSuperficie = sumaSuperficie / (double)n;
    printf("\nEJERCICIO 28 - Estadisticas generales:\n");
    printf("-> Cantidad total de poblacion   : %lld habitantes\n", totalPoblacion);
    printf("-> Promedio total de superficie  : %.2f km2\n", promSuperficie);

    /* Ejercicio 29: Ordenar ascendente por codigo telefonico y mostrar */
    ordenarPorCodigoAscendente(paises, n);
    mostrarPaises("EJERCICIO 29 - Paises ordenados ascendentemente por Codigo Telefonico", paises, n);

    printf("\nPresione cualquier tecla para finalizar...");
    _getch();
    return 0;
}
