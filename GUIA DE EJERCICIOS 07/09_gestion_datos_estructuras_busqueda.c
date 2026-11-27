/*
 * ============================================================================
 * EJERCICIO 9: Sistema de Gestion de Datos con Matriz de Estructuras
 * ============================================================================
 * Algoritmo: Genera dinamicamente una matriz F x C de estructuras Registro
 *            (ID, nombre, valor numerico). Valida al ingresar cada registro:
 *              - Que el ID no este duplicado en la matriz.
 *              - Que el nombre no este vacio.
 *              - Que el valor numerico este dentro del rango permitido.
 *            Implementa QuickSort recursivo con punteros para ordenar la
 *            matriz completa por ID y por nombre, busqueda binaria recursiva
 *            tanto por ID como por nombre, y calcula estadisticas (minimo,
 *            maximo y promedio) recorriendo la matriz mediante punteros.
 * Funciones:
 *   - quicksort_id_rec() / quicksort_nombre_rec()
 *   - busqueda_binaria_id_rec() / busqueda_binaria_nombre_rec()
 *   - calcular_estadisticas()
 * Punteros y Recursividad:
 *   - Acceso lineal y matricial a estructuras con punteros (Registro *).
 * Tipos de datos: struct Registro, int, double, char[].
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

typedef struct {
    int id;
    char nombre[48];
    double valor;
} Registro;

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leer_entero(const char *prompt, int min, int max) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1) {
            printf("  [Error] Ingrese un numero entero valido.\n");
            limpiar_buffer();
        } else if (valor < min || valor > max) {
            printf("  [Error] Rango permitido: [%d, %d].\n", min, max);
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

double leer_double(const char *prompt, double min, double max) {
    double valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%lf", &valor) != 1) {
            printf("  [Error] Ingrese un numero real valido.\n");
            limpiar_buffer();
        } else if (valor < min || valor > max) {
            printf("  [Error] Rango permitido: [%.2f, %.2f].\n", min, max);
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

void leer_nombre_no_vacio(const char *prompt, char *dest, size_t tam) {
    while (1) {
        printf("%s", prompt);
        if (!fgets(dest, (int)tam, stdin)) {
            strcpy(dest, "Item");
            return;
        }
        dest[strcspn(dest, "\r\n")] = '\0';
        int tiene_char = 0;
        for (size_t i = 0; *(dest + i) != '\0'; i++) {
            if (!isspace((unsigned char)*(dest + i))) {
                tiene_char = 1;
                break;
            }
        }
        if (tiene_char) {
            return;
        }
        printf("  [Error] El nombre no puede estar vacio.\n");
    }
}

int id_duplicado(const Registro *mat, int cantidad_actual, int id_buscado) {
    for (int i = 0; i < cantidad_actual; i++) {
        if ((mat + i)->id == id_buscado) {
            return 1;
        }
    }
    return 0;
}

void intercambiar_reg(Registro *a, Registro *b) {
    Registro temp = *a;
    *a = *b;
    *b = temp;
}

void quicksort_id_rec(Registro *arr, int izq, int der) {
    if (izq >= der) return;
    int pivote = (arr + der)->id;
    int i = izq - 1;
    for (int j = izq; j < der; j++) {
        if ((arr + j)->id < pivote) {
            i++;
            intercambiar_reg(arr + i, arr + j);
        }
    }
    intercambiar_reg(arr + i + 1, arr + der);
    int p = i + 1;
    quicksort_id_rec(arr, izq, p - 1);
    quicksort_id_rec(arr, p + 1, der);
}

void quicksort_nombre_rec(Registro *arr, int izq, int der) {
    if (izq >= der) return;
    char *pivote = (arr + der)->nombre;
    int i = izq - 1;
    for (int j = izq; j < der; j++) {
        if (strcmp((arr + j)->nombre, pivote) < 0) {
            i++;
            intercambiar_reg(arr + i, arr + j);
        }
    }
    intercambiar_reg(arr + i + 1, arr + der);
    int p = i + 1;
    quicksort_nombre_rec(arr, izq, p - 1);
    quicksort_nombre_rec(arr, p + 1, der);
}

int busqueda_binaria_nombre_rec(const Registro *arr, int izq, int der, const char *nombre) {
    if (izq > der) return -1;
    int med = izq + (der - izq) / 2;
    int cmp = strcmp((arr + med)->nombre, nombre);
    if (cmp == 0) return med;
    if (cmp > 0) return busqueda_binaria_nombre_rec(arr, izq, med - 1, nombre);
    return busqueda_binaria_nombre_rec(arr, med + 1, der, nombre);
}

int busqueda_binaria_id_rec(const Registro *arr, int izq, int der, int id) {
    if (izq > der) return -1;
    int med = izq + (der - izq) / 2;
    if ((arr + med)->id == id) return med;
    if ((arr + med)->id > id) return busqueda_binaria_id_rec(arr, izq, med - 1, id);
    return busqueda_binaria_id_rec(arr, med + 1, der, id);
}

void mostrar_matriz_registros(const Registro *mat, int filas, int cols, const char *titulo) {
    printf("\n--- %s (%dx%d) ---\n", titulo, filas, cols);
    for (int i = 0; i < filas; i++) {
        for (int j = 0; j < cols; j++) {
            const Registro *r = mat + i * cols + j;
            printf("  [%d][%d] ID: %4d | Nombre: %-18s | Valor: %8.2f\n",
                   i, j, r->id, r->nombre, r->valor);
        }
    }
}

void calcular_estadisticas(const Registro *mat, int total,
                           const Registro **min_reg, const Registro **max_reg,
                           double *promedio) {
    *min_reg = mat;
    *max_reg = mat;
    double suma = 0.0;
    for (int i = 0; i < total; i++) {
        const Registro *r = mat + i;
        suma += r->valor;
        if (r->valor < (*min_reg)->valor) *min_reg = r;
        if (r->valor > (*max_reg)->valor) *max_reg = r;
    }
    *promedio = suma / total;
}

int main(void) {
    printf("=================================================================\n");
    printf("   SISTEMA DE GESTION DE DATOS CON MATRIZ DE ESTRUCTURAS\n");
    printf("=================================================================\n\n");

    int filas = leer_entero("Numero de filas de la matriz de registros (1 - 5): ", 1, 5);
    int cols  = leer_entero("Numero de columnas de la matriz de registros (2 - 5): ", 2, 5);
    int total = filas * cols;

    Registro *mat = (Registro *)malloc(total * sizeof(Registro));
    if (!mat) {
        printf("Error de memoria.\n");
        system("pause");
        return 1;
    }

    for (int i = 0; i < total; i++) {
        printf("\n--- Ingreso del Registro [%d][%d] (#%d de %d) ---\n",
               i / cols, i % cols, i + 1, total);
        while (1) {
            int id = leer_entero("  ID unico (1 - 9999): ", 1, 9999);
            if (id_duplicado(mat, i, id)) {
                printf("  [Error] El ID %d ya existe. Ingrese un ID diferente.\n", id);
            } else {
                (mat + i)->id = id;
                break;
            }
        }
        leer_nombre_no_vacio("  Nombre (no vacio): ", (mat + i)->nombre, sizeof((mat + i)->nombre));
        (mat + i)->valor = leer_double("  Valor numerico (0.00 - 100000.00): ", 0.0, 100000.0);
    }

    mostrar_matriz_registros(mat, filas, cols, "MATRIZ ORIGINAL DE REGISTROS");

    quicksort_id_rec(mat, 0, total - 1);
    mostrar_matriz_registros(mat, filas, cols, "MATRIZ ORDENADA POR ID (QUICKSORT RECURSIVO)");

    int id_buscar = leer_entero("\nIngrese un ID a buscar con Busqueda Binaria Recursiva: ", 1, 9999);
    int pos_id = busqueda_binaria_id_rec(mat, 0, total - 1, id_buscar);
    if (pos_id != -1) {
        Registro *r = mat + pos_id;
        printf("  [ENCONTRADO en Posicion [%d][%d]] -> ID: %d | Nombre: %s | Valor: %.2f\n",
               pos_id / cols, pos_id % cols, r->id, r->nombre, r->valor);
    } else {
        printf("  [NO ENCONTRADO] El ID %d no existe en la matriz.\n", id_buscar);
    }

    quicksort_nombre_rec(mat, 0, total - 1);
    char nombre_buscar[48];
    leer_nombre_no_vacio("\nIngrese un Nombre exacto a buscar con Busqueda Binaria Recursiva: ",
                         nombre_buscar, sizeof(nombre_buscar));
    int pos_nom = busqueda_binaria_nombre_rec(mat, 0, total - 1, nombre_buscar);
    if (pos_nom != -1) {
        Registro *r = mat + pos_nom;
        printf("  [ENCONTRADO] -> ID: %d | Nombre: %s | Valor: %.2f\n",
               r->id, r->nombre, r->valor);
    } else {
        printf("  [NO ENCONTRADO] El nombre '%s' no se encontro.\n", nombre_buscar);
    }

    quicksort_id_rec(mat, 0, total - 1);

    const Registro *min_r = NULL, *max_r = NULL;
    double promedio = 0.0;
    calcular_estadisticas(mat, total, &min_r, &max_r, &promedio);

    printf("\n=================================================================\n");
    printf("   REPORTE FINAL DE ESTADISTICAS (CALCULADAS CON PUNTEROS)\n");
    printf("=================================================================\n");
    printf("  * Total de registros : %d\n", total);
    printf("  * Valor Minimo       : %.2f (ID: %d, Nombre: %s)\n",
           min_r->valor, min_r->id, min_r->nombre);
    printf("  * Valor Maximo       : %.2f (ID: %d, Nombre: %s)\n",
           max_r->valor, max_r->id, max_r->nombre);
    printf("  * Valor Promedio     : %.2f\n\n", promedio);

    free(mat);
    system("pause");
    return 0;
}
