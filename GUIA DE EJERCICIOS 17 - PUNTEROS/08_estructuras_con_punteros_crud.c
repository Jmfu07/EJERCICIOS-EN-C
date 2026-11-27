/* ============================================================================
 * GUIA DE EJERCICIOS 17 (PUNTEROS) - EJERCICIO 08:
 * Arreglo dinamico de estructuras con punteros (->): registro de estudiantes,
 * actualizacion de promedio por puntero, busqueda y ordenamiento descendente.
 * ============================================================================ */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <conio.h>

typedef struct {
    int cedula;
    char nombre[40];
    double promedio;
} Estudiante;

static int leer_int_rango(const char *msg, int min, int max) {
    char buf[32]; long v; char *fin;
    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtol(buf, &fin, 10);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0') && v >= min && v <= max)
            return (int)v;
        printf("  Error: fuera de rango [%d a %d].\n", min, max);
    }
}

static double leer_prom(const char *msg) {
    char buf[32]; double v; char *fin;
    for (;;) {
        printf("%s", msg);
        if (!fgets(buf, sizeof buf, stdin)) exit(0);
        v = strtod(buf, &fin);
        if (fin != buf && (*fin == '\n' || *fin == '\r' || *fin == '\0') && v >= 0.0 && v <= 20.0)
            return v;
        printf("  Error: promedio entre 0 y 20.\n");
    }
}

static Estudiante *buscar_por_cedula(Estudiante *lista, int n, int ced) {
    Estudiante *p;
    for (p = lista; p < lista + n; p++)
        if (p->cedula == ced) return p;
    return NULL;
}

static void ordenar_por_promedio(Estudiante *lista, int n) {
    int i, j;
    for (i = 0; i < n - 1; i++)
        for (j = 0; j < n - 1 - i; j++)
            if ((lista + j)->promedio < (lista + j + 1)->promedio) {
                Estudiante tmp = *(lista + j);
                *(lista + j) = *(lista + j + 1);
                *(lista + j + 1) = tmp;
            }
}

static void mostrar_estudiantes(const Estudiante *lista, int n) {
    const Estudiante *p;
    printf("\n%-10s | %-28s | %8s\n", "CEDULA", "NOMBRE", "PROMEDIO");
    printf("-----------+------------------------------+---------\n");
    for (p = lista; p < lista + n; p++)
        printf("%-10d | %-28s | %8.2f\n", p->cedula, p->nombre, p->promedio);
}

int main(void) {
    Estudiante *lista = NULL, *hallado;
    int n, i, ced;

    printf("=== PUNTEROS 08: ESTRUCTURAS DINAMICAS CON OPERADOR FLECHA (->) ===\n\n");
    n = leer_int_rango("Cantidad de estudiantes [1 a 20]: ", 1, 20);
    lista = (Estudiante *)malloc((size_t)n * sizeof(Estudiante));
    if (!lista) { printf("Error de memoria.\n"); return 1; }

    for (i = 0; i < n; i++) {
        Estudiante *e = lista + i;
        printf("\nEstudiante %d:\n", i + 1);
        e->cedula = leer_int_rango("  Cedula [100000 a 39999999]: ", 100000, 39999999);
        printf("  Nombre: ");
        if (!fgets(e->nombre, sizeof e->nombre, stdin)) { free(lista); return 0; }
        e->nombre[strcspn(e->nombre, "\r\n")] = '\0';
        if (e->nombre[0] == '\0') strcpy(e->nombre, "SinNombre");
        e->promedio = leer_prom("  Promedio [0 a 20]: ");
    }

    ordenar_por_promedio(lista, n);
    mostrar_estudiantes(lista, n);

    ced = leer_int_rango("\nIngrese cedula para actualizar promedio (0 para omitir): ", 0, 39999999);
    if (ced > 0) {
        hallado = buscar_por_cedula(lista, n, ced);
        if (hallado) {
            printf("  Encontrado: %s (promedio actual: %.2f)\n", hallado->nombre, hallado->promedio);
            hallado->promedio = leer_prom("  Nuevo promedio [0 a 20]: ");
            ordenar_por_promedio(lista, n);
            mostrar_estudiantes(lista, n);
        } else {
            printf("  No se encontro esa cedula.\n");
        }
    }

    free(lista);
    printf("\n");
    system("pause");
    return 0;
}
