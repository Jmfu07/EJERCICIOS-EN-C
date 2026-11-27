/*
 * ============================================================================
 * EJERCICIO 12: Sistema de Cifrado y Descifrado con Matrices y Patron Zigzag
 * ============================================================================
 * Algoritmo: Lee una cadena validando longitud minima (>= 4) y que solo
 *            contenga caracteres imprimibles permitidos (letras, digitos y
 *            espacios/puntuacion basica, prohibiendo caracteres de control o
 *            simbolos reservados como '@', '#', '$').
 *            Almacena la cadena en una matriz dinamica de caracteres F x C
 *            (rellenando espacios sobrantes).
 *            Proceso de Cifrado:
 *              1. Invierte recursivamente cada bloque (fila de longitud C)
 *                 mediante punteros.
 *              2. Reorganiza la matriz aplicando una lectura en patron Zigzag
 *                 por filas (filas pares de izq a der, filas impares de der
 *                 a izq; o transposicion en serpentina diagonal) usando punteros.
 *            Proceso de Descifrado:
 *              1. Revierte el patron Zigzag hacia la matriz.
 *              2. Invierte recursivamente cada bloque para recuperar el
 *                 mensaje original.
 * Funciones:
 *   - invertir_bloque_rec(): Invierte recursivamente un bloque de caracteres.
 *   - aplicar_zigzag() / revertir_zigzag(): Reorganiza con punteros.
 * Punteros y Recursividad:
 *   - Matriz dinamica de caracteres manipulada con aritmetica de punteros.
 * Tipos de datos: char, char *, int, size_t.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

int validar_cadena(const char *cad, size_t min_len) {
    size_t len = strlen(cad);
    if (len < min_len) {
        printf("  [Error] La cadena debe tener al menos %u caracteres (actual: %u).\n",
               (unsigned)min_len, (unsigned)len);
        return 0;
    }
    for (size_t i = 0; i < len; i++) {
        char c = *(cad + i);
        if (!isprint((unsigned char)c) || c == '@' || c == '#' || c == '$') {
            printf("  [Error] Caracter no permitido '%c' en la posicion %u. (Evite @, #, $).\n",
                   c, (unsigned)i);
            return 0;
        }
    }
    return 1;
}

void invertir_bloque_rec(char *izq, char *der) {
    if (izq >= der) {
        return;
    }
    char temp = *izq;
    *izq = *der;
    *der = temp;
    invertir_bloque_rec(izq + 1, der - 1);
}

void cifrar_bloques_rec(char *mat, int fila_act, int filas, int cols) {
    if (fila_act >= filas) {
        return;
    }
    char *inicio_fila = mat + fila_act * cols;
    invertir_bloque_rec(inicio_fila, inicio_fila + cols - 1);
    cifrar_bloques_rec(mat, fila_act + 1, filas, cols);
}

/*
 * Reorganiza la matriz aplicando un recorrido en Zigzag diagonal/serpentina
 * por columnas alternadas hacia el buffer de salida usando punteros.
 */
void aplicar_zigzag(const char *mat, char *salida, int filas, int cols) {
    int k = 0;
    for (int col = 0; col < cols; col++) {
        if (col % 2 == 0) {
            for (int f = 0; f < filas; f++) {
                *(salida + k) = *(mat + f * cols + col);
                k++;
            }
        } else {
            for (int f = filas - 1; f >= 0; f--) {
                *(salida + k) = *(mat + f * cols + col);
                k++;
            }
        }
    }
    *(salida + k) = '\0';
}

void revertir_zigzag(const char *entrada, char *mat, int filas, int cols) {
    int k = 0;
    for (int col = 0; col < cols; col++) {
        if (col % 2 == 0) {
            for (int f = 0; f < filas; f++) {
                *(mat + f * cols + col) = *(entrada + k);
                k++;
            }
        } else {
            for (int f = filas - 1; f >= 0; f--) {
                *(mat + f * cols + col) = *(entrada + k);
                k++;
            }
        }
    }
}

void mostrar_matriz_chars(const char *mat, int filas, int cols, const char *titulo) {
    printf("\n--- %s (%dx%d) ---\n", titulo, filas, cols);
    for (int i = 0; i < filas; i++) {
        printf("  Bloque %2d: [", i);
        for (int j = 0; j < cols; j++) {
            printf("%c", *(mat + i * cols + j));
        }
        printf("]\n");
    }
}

int main(void) {
    printf("=================================================================\n");
    printf("   SISTEMA DE CIFRADO Y DESCIFRADO POR BLOQUES Y PATRON ZIGZAG\n");
    printf("=================================================================\n");
    printf("Requisitos: Longitud minima = 6 caracteres. Prohibidos: @, #, $\n\n");

    char buffer[256];
    while (1) {
        printf("Ingrese el mensaje a cifrar: ");
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            strcpy(buffer, "LENGUAJE C AVANZADO");
        }
        buffer[strcspn(buffer, "\r\n")] = '\0';
        if (validar_cadena(buffer, 6)) {
            break;
        }
    }

    int len_orig = (int)strlen(buffer);
    int cols = (int)ceil(sqrt((double)len_orig));
    if (cols < 3) cols = 3;
    int filas = (len_orig + cols - 1) / cols;
    int total = filas * cols;

    char *mat = (char *)malloc((total + 1) * sizeof(char));
    char *texto_cifrado = (char *)malloc((total + 1) * sizeof(char));
    char *mat_descifrada = (char *)malloc((total + 1) * sizeof(char));

    for (int i = 0; i < total; i++) {
        *(mat + i) = (i < len_orig) ? *(buffer + i) : ' ';
    }
    *(mat + total) = '\0';

    mostrar_matriz_chars(mat, filas, cols, "1. MATRIZ ORIGINAL POR BLOQUES");

    cifrar_bloques_rec(mat, 0, filas, cols);
    mostrar_matriz_chars(mat, filas, cols, "2. MATRIZ TRAS INVERSION RECURSIVA DE BLOQUES");

    aplicar_zigzag(mat, texto_cifrado, filas, cols);

    revertir_zigzag(texto_cifrado, mat_descifrada, filas, cols);
    cifrar_bloques_rec(mat_descifrada, 0, filas, cols);
    *(mat_descifrada + len_orig) = '\0';

    printf("\n=================================================================\n");
    printf("   RESULTADOS Y COMPARACION DEL PROCESO\n");
    printf("=================================================================\n");
    printf("  * Texto Original   (%2d chars): \"%s\"\n", len_orig, buffer);
    printf("  * Texto Cifrado    (%2d chars): \"%s\"\n", total, texto_cifrado);
    printf("  * Texto Descifrado (%2d chars): \"%s\"\n",
           (int)strlen(mat_descifrada), mat_descifrada);

    int exito = (strcmp(buffer, mat_descifrada) == 0);
    printf("  * Comparacion Final          : %s\n\n",
           exito ? "EXITO (El texto descifrado coincide 100% con el original)"
                 : "ERROR (Diferencia detectada)");

    free(mat);
    free(texto_cifrado);
    free(mat_descifrada);

    system("pause");
    return 0;
}
