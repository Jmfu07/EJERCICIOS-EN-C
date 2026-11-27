/*
 * ============================================================================
 * GUIA VARIOS - EJERCICIO 6: Estructuras Punto Cartesiano y Libro
 * ============================================================================
 * Algoritmo: Implementa las dos estructuras adicionales de la guia:
 *              1. Estructura Punto (x, y) en el plano cartesiano y funciones
 *                 para leer puntos y calcular la distancia euclidiana entre
 *                 dos puntos: d = sqrt((x2 - x1)^2 + (y2 - y1)^2).
 *              2. Estructura Libro (titulo, autor, anio de publicacion) y
 *                 funciones para registrar y mostrar la informacion detallada
 *                 de uno o varios libros.
 * Funciones:
 *   - calcular_distancia_puntos()
 *   - leer_libro(), mostrar_libro()
 * Punteros y Recursividad:
 *   - Paso de estructuras por puntero constante (const Punto *, const Libro *).
 * Tipos de datos: struct Punto, struct Libro, double, int, char[].
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

typedef struct {
    double x;
    double y;
} Punto;

typedef struct {
    char titulo[80];
    char autor[60];
    int anio;
} Libro;

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

void leer_texto(const char *prompt, char *dest, size_t tam) {
    while (1) {
        printf("%s", prompt);
        if (!fgets(dest, (int)tam, stdin)) {
            strcpy(dest, "Sin especificar");
            return;
        }
        dest[strcspn(dest, "\r\n")] = '\0';
        int ok = 0;
        for (size_t i = 0; dest[i] != '\0'; i++) {
            if (!isspace((unsigned char)dest[i])) {
                ok = 1;
                break;
            }
        }
        if (ok) return;
        printf("  [Error] El campo no puede estar vacio.\n");
    }
}

double calcular_distancia_puntos(const Punto *p1, const Punto *p2) {
    double dx = p2->x - p1->x;
    double dy = p2->y - p1->y;
    return sqrt(dx * dx + dy * dy);
}

void leer_libro(Libro *lib, int num) {
    printf("\n  --- Ingreso de datos del Libro #%d ---\n", num);
    leer_texto("  Titulo del libro: ", lib->titulo, sizeof(lib->titulo));
    leer_texto("  Autor del libro : ", lib->autor, sizeof(lib->autor));
    lib->anio = leer_entero("  Anio de publicacion (1450 - 2030): ", 1450, 2030);
}

void mostrar_libro(const Libro *lib, int num) {
    printf("  Libro #%2d | Titulo: \"%-28s\" | Autor: %-20s | Anio: %4d\n",
           num, lib->titulo, lib->autor, lib->anio);
}

int main(void) {
    printf("=================================================================\n");
    printf("   ESTRUCTURAS EN C: PUNTO CARTESIANO (X, Y) Y LIBROS\n");
    printf("=================================================================\n\n");

    printf("--- PARTE 1: DISTANCIA ENTRE DOS PUNTOS EN EL PLANO CARTESIANO ---\n");
    Punto p1, p2;
    p1.x = leer_double("  Punto 1 - Coordenada X (-10000 a 10000): ", -10000.0, 10000.0);
    p1.y = leer_double("  Punto 1 - Coordenada Y (-10000 a 10000): ", -10000.0, 10000.0);
    p2.x = leer_double("  Punto 2 - Coordenada X (-10000 a 10000): ", -10000.0, 10000.0);
    p2.y = leer_double("  Punto 2 - Coordenada Y (-10000 a 10000): ", -10000.0, 10000.0);

    double dist = calcular_distancia_puntos(&p1, &p2);
    printf("\n  * Punto A = (%.2f, %.2f)\n", p1.x, p1.y);
    printf("  * Punto B = (%.2f, %.2f)\n", p2.x, p2.y);
    printf("  * Distancia Euclidiana d(A, B) = %.4f\n\n", dist);

    printf("--- PARTE 2: REGISTRO E IMPRESION DE LIBROS ---\n");
    int cant = leer_entero("  Cuantos libros desea registrar? (1 - 20): ", 1, 20);
    Libro *catalogo = (Libro *)malloc(cant * sizeof(Libro));
    if (!catalogo) {
        printf("Error de memoria.\n");
        system("pause");
        return 1;
    }

    for (int i = 0; i < cant; i++) {
        leer_libro(catalogo + i, i + 1);
    }

    printf("\n--- CATALOGO DE LIBROS REGISTRADOS ---\n");
    for (int i = 0; i < cant; i++) {
        mostrar_libro(catalogo + i, i + 1);
    }
    printf("\n");

    free(catalogo);
    system("pause");
    return 0;
}
