/*
 * ============================================================================
 * GUIA VARIOS - EJERCICIO 3: Manejo de Archivos de Texto
 * ============================================================================
 * Algoritmo: Implementa las operaciones de lectura, escritura y busqueda en
 *            archivos de texto:
 *              1. Escribir lineas ingresadas por el usuario en un archivo.
 *              2. Leer un archivo de texto y devolver un arreglo dinamico de
 *                 strings (char **) con todas las lineas del archivo.
 *              3. Contar el numero total de lineas del archivo.
 *              4. Buscar una palabra especifica en el contenido del archivo
 *                 indicando cuantas veces aparece y en que lineas.
 * Funciones:
 *   - escribir_en_archivo()
 *   - leer_lineas_archivo()
 *   - contar_lineas_archivo()
 *   - buscar_palabra_en_archivo()
 * Punteros y Recursividad:
 *   - Puntero a archivo (FILE *) y arreglo dinamico de cadenas (char **).
 * Tipos de datos: FILE *, char **, char *, int.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void crear_archivo_ejemplo_si_no_existe(const char *nombre_archivo) {
    FILE *fp = fopen(nombre_archivo, "r");
    if (fp) {
        fclose(fp);
        return;
    }
    fp = fopen(nombre_archivo, "w");
    if (!fp) return;
    fprintf(fp, "El lenguaje C es un lenguaje de programacion de proposito general.\n");
    fprintf(fp, "El manejo de punteros y archivos en lenguaje C es muy eficiente.\n");
    fprintf(fp, "Aprender lenguaje C permite comprender la memoria del computador.\n");
    fclose(fp);
}

void escribir_en_archivo(const char *nombre_archivo, int agregar) {
    FILE *fp = fopen(nombre_archivo, agregar ? "a" : "w");
    if (!fp) {
        printf("  [Error] No se pudo abrir el archivo '%s' para escritura.\n", nombre_archivo);
        return;
    }

    int cant = leer_entero("  Cuantas lineas desea escribir en el archivo? (1 - 20): ", 1, 20);
    char linea[256];
    for (int i = 0; i < cant; i++) {
        printf("  Linea #%d: ", i + 1);
        if (fgets(linea, sizeof(linea), stdin)) {
            linea[strcspn(linea, "\r\n")] = '\0';
            fprintf(fp, "%s\n", linea);
        }
    }
    fclose(fp);
    printf("  [OK] Se guardaron %d linea(s) en '%s'.\n", cant, nombre_archivo);
}

char **leer_lineas_archivo(const char *nombre_archivo, int *num_lineas) {
    *num_lineas = 0;
    FILE *fp = fopen(nombre_archivo, "r");
    if (!fp) {
        return NULL;
    }

    int capacidad = 8;
    char **lineas = (char **)malloc(capacidad * sizeof(char *));
    char buffer[512];

    while (fgets(buffer, sizeof(buffer), fp)) {
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (*num_lineas >= capacidad) {
            capacidad *= 2;
            lineas = (char **)realloc(lineas, capacidad * sizeof(char *));
        }
        *(lineas + *num_lineas) = (char *)malloc((strlen(buffer) + 1) * sizeof(char));
        strcpy(*(lineas + *num_lineas), buffer);
        (*num_lineas)++;
    }
    fclose(fp);
    return lineas;
}

void liberar_lineas(char **lineas, int num_lineas) {
    if (!lineas) return;
    for (int i = 0; i < num_lineas; i++) {
        free(*(lineas + i));
    }
    free(lineas);
}

int contar_lineas_archivo(const char *nombre_archivo) {
    int total = 0;
    char **lineas = leer_lineas_archivo(nombre_archivo, &total);
    liberar_lineas(lineas, total);
    return total;
}

void buscar_palabra_en_archivo(const char *nombre_archivo, const char *palabra) {
    int num_lineas = 0;
    char **lineas = leer_lineas_archivo(nombre_archivo, &num_lineas);
    if (!lineas) {
        printf("  [Error] No se pudo leer el archivo.\n");
        return;
    }

    size_t len_pal = strlen(palabra);
    int total_ocurrencias = 0;

    printf("\n  --- Resultados de busqueda para \"%s\" ---\n", palabra);
    for (int i = 0; i < num_lineas; i++) {
        const char *cursor = *(lineas + i);
        int en_esta_linea = 0;
        while ((cursor = strstr(cursor, palabra)) != NULL) {
            en_esta_linea++;
            total_ocurrencias++;
            cursor += len_pal;
        }
        if (en_esta_linea > 0) {
            printf("  * Linea %2d (%d vez/veces): %s\n", i + 1, en_esta_linea, *(lineas + i));
        }
    }
    printf("  -> Total de ocurrencias de \"%s\": %d\n", palabra, total_ocurrencias);
    liberar_lineas(lineas, num_lineas);
}

int main(void) {
    const char *archivo = "datos_texto.txt";
    crear_archivo_ejemplo_si_no_existe(archivo);

    printf("=================================================================\n");
    printf("   MANEJO DE ARCHIVOS DE TEXTO (LECTURA, ESCRITURA Y BUSQUEDA)\n");
    printf("=================================================================\n");
    printf("Archivo activo: %s\n", archivo);

    int opcion = 0;
    do {
        printf("\n------------------ MENU DE ARCHIVOS ------------------\n");
        printf("  1. Leer archivo y mostrar arreglo de strings (lineas)\n");
        printf("  2. Escribir/Sobrescribir datos en el archivo\n");
        printf("  3. Agregar nuevas lineas al final del archivo\n");
        printf("  4. Contar el numero de lineas del archivo\n");
        printf("  5. Buscar ocurrencias de una palabra en el archivo\n");
        printf("  6. Salir\n");
        opcion = leer_entero("Seleccione una opcion (1 - 6): ", 1, 6);

        if (opcion == 1) {
            int n = 0;
            char **lineas = leer_lineas_archivo(archivo, &n);
            printf("\n  --- CONTENIDO DEL ARCHIVO (%d lineas leidas en char **) ---\n", n);
            for (int i = 0; i < n; i++) {
                printf("  [%2d]: %s\n", i + 1, *(lineas + i));
            }
            liberar_lineas(lineas, n);
        } else if (opcion == 2) {
            escribir_en_archivo(archivo, 0);
        } else if (opcion == 3) {
            escribir_en_archivo(archivo, 1);
        } else if (opcion == 4) {
            int total = contar_lineas_archivo(archivo);
            printf("\n  * El archivo '%s' contiene %d linea(s).\n", archivo, total);
        } else if (opcion == 5) {
            char palabra[100];
            while (1) {
                printf("  Ingrese la palabra a buscar: ");
                if (fgets(palabra, sizeof(palabra), stdin)) {
                    palabra[strcspn(palabra, "\r\n")] = '\0';
                    if (strlen(palabra) > 0) break;
                }
                printf("  [Error] La palabra no puede estar vacia.\n");
            }
            buscar_palabra_en_archivo(archivo, palabra);
        }
    } while (opcion != 6);

    printf("\nPrograma finalizado.\n");
    system("pause");
    return 0;
}
