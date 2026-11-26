/* 5 EJERCICIOS DE PUNTEROS CON FUNCIONES Y MATRICES - EJERCICIO 3:
 * Recorrido recursivo de caminos:
 * Implemente en C un programa que, dada una matriz de caracteres, encuentre
 * todas las posibles rutas desde la esquina superior izquierda hasta la esquina
 * inferior derecha, moviendose solo hacia abajo o hacia la derecha.
 * - Usar recursividad para explorar los caminos.
 * - Representar la matriz con punteros.
 * - Imprimir cada ruta encontrada como una secuencia de coordenadas.
 */
#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <ctype.h>

static int leer_rango(const char *prompt, int min, int max)
{
    char buf[8]; int len, c, v;
    for (;;) {
        len = 0;
        printf("%s [%d a %d]: ", prompt, min, max);
        for (;;) {
            c = _getch();
            if (c == 0 || c == 0xE0) { _getch(); continue; }
            if (c == '\r') { if (len > 0) break; continue; }
            if (c == '\b') { if (len > 0) { len--; printf("\b \b"); } continue; }
            if (c >= '0' && c <= '9' && len < 2) { *(buf + len) = (char)c; len++; putchar(c); }
        }
        *(buf + len) = '\0'; putchar('\n');
        v = (int)strtol(buf, NULL, 10);
        if (v >= min && v <= max) return v;
    }
}

static char leer_letra(int f, int c)
{
    int ch;
    printf("  Caracter en (%d, %d): ", f, c);
    for (;;) {
        ch = _getch();
        if (ch == 0 || ch == 0xE0) { _getch(); continue; }
        if (isalnum(ch)) {
            ch = toupper(ch);
            printf("%c\n", ch);
            return (char)ch;
        }
    }
}

void explorar_caminos(const char *mat, int filas, int cols, int f, int c,
                      int *ruta_f, int *ruta_c, int paso, int *contador)
{
    int i;
    *(ruta_f + paso) = f;
    *(ruta_c + paso) = c;

    if (f == filas - 1 && c == cols - 1) {
        (*contador)++;
        printf("Ruta %2d: ", *contador);
        for (i = 0; i <= paso; i++) {
            int rf = *(ruta_f + i);
            int rc = *(ruta_c + i);
            printf("(%d,%d)[%c]%s", rf, rc, *(mat + rf * cols + rc), (i < paso) ? " -> " : "\n");
        }
        return;
    }

    /* Moverse hacia abajo */
    if (f + 1 < filas)
        explorar_caminos(mat, filas, cols, f + 1, c, ruta_f, ruta_c, paso + 1, contador);

    /* Moverse hacia la derecha */
    if (c + 1 < cols)
        explorar_caminos(mat, filas, cols, f, c + 1, ruta_f, ruta_c, paso + 1, contador);
}

int main(void)
{
    int filas, cols, f, c, total_rutas = 0;
    char *matriz;
    int *ruta_f, *ruta_c;

    printf("=== EJERCICIO 3: RECORRIDO RECURSIVO DE CAMINOS CON PUNTEROS ===\n\n");
    filas = leer_rango("Ingrese cantidad de filas", 2, 6);
    cols  = leer_rango("Ingrese cantidad de columnas", 2, 6);

    matriz = (char *)malloc((size_t)(filas * cols) * sizeof(char));
    ruta_f = (int *)malloc((size_t)(filas + cols) * sizeof(int));
    ruta_c = (int *)malloc((size_t)(filas + cols) * sizeof(int));
    if (!matriz || !ruta_f || !ruta_c) return 1;

    printf("\nIngrese un caracter (letra o digito) para cada celda:\n");
    for (f = 0; f < filas; f++)
        for (c = 0; c < cols; c++)
            *(matriz + f * cols + c) = leer_letra(f, c);

    printf("\nMatriz de caracteres:\n");
    for (f = 0; f < filas; f++) {
        for (c = 0; c < cols; c++)
            printf("  %c ", *(matriz + f * cols + c));
        printf("\n");
    }

    printf("\nRutas encontradas desde (0,0) hasta (%d,%d):\n", filas - 1, cols - 1);
    explorar_caminos(matriz, filas, cols, 0, 0, ruta_f, ruta_c, 0, &total_rutas);
    printf("\nTotal de rutas encontradas: %d\n", total_rutas);

    free(matriz);
    free(ruta_f);
    free(ruta_c);
    system("pause");
    return 0;
}
