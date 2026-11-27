/*
 * ============================================================================
 * EJERCICIO 2: Sistema de Rotacion Multinivel de Matrices con Validacion
 * ============================================================================
 * Algoritmo: Divide una matriz cuadrada N x N en N/2 anillos concentricos.
 *            Cada anillo k rota 1 posicion en sentido horario si k es par,
 *            o en sentido antihorario si k es impar (sentidos alternados),
 *            realizando los intercambios in-place mediante aritmetica de
 *            punteros sin usar matrices auxiliares. Luego valida la paridad
 *            del numero de niveles y verifica si existen filas o columnas
 *            palindromicas.
 * Funciones:
 *   - rotar_anillo_horario() / rotar_anillo_antihorario()
 *   - rotar_multinivel_rec(): Procesa recursivamente cada anillo.
 *   - verificar_palindromos(): Detecta filas y columnas palindromicas.
 * Punteros y Recursividad:
 *   - Matriz almacenada en bloque dinamico accedida con *(mat + i * n + j).
 *   - Rotacion recursiva por nivel de anillo.
 * Tipos de datos: int, punteros simples int *.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int leer_entero(const char *prompt, int min, int max) {
    int valor;
    while (1) {
        printf("%s", prompt);
        if (scanf("%d", &valor) != 1) {
            printf("  [Error] Entrada invalida. Ingrese un numero entero.\n");
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

void imprimir_matriz(const int *mat, int n, const char *titulo) {
    printf("\n--- %s (%dx%d) ---\n", titulo, n, n);
    for (int i = 0; i < n; i++) {
        printf("  ");
        for (int j = 0; j < n; j++) {
            printf("%4d ", *(mat + i * n + j));
        }
        printf("\n");
    }
}

void rotar_anillo_horario(int *mat, int n, int nivel) {
    int ini = nivel;
    int fin = n - 1 - nivel;
    if (ini >= fin) return;

    int prev = *(mat + (ini + 1) * n + ini);

    for (int j = ini; j <= fin; j++) {
        int temp = *(mat + ini * n + j);
        *(mat + ini * n + j) = prev;
        prev = temp;
    }
    for (int i = ini + 1; i <= fin; i++) {
        int temp = *(mat + i * n + fin);
        *(mat + i * n + fin) = prev;
        prev = temp;
    }
    for (int j = fin - 1; j >= ini; j--) {
        int temp = *(mat + fin * n + j);
        *(mat + fin * n + j) = prev;
        prev = temp;
    }
    for (int i = fin - 1; i >= ini + 1; i--) {
        int temp = *(mat + i * n + ini);
        *(mat + i * n + ini) = prev;
        prev = temp;
    }
}

void rotar_anillo_antihorario(int *mat, int n, int nivel) {
    int ini = nivel;
    int fin = n - 1 - nivel;
    if (ini >= fin) return;

    int prev = *(mat + ini * n + (ini + 1));

    for (int i = ini; i <= fin; i++) {
        int temp = *(mat + i * n + ini);
        *(mat + i * n + ini) = prev;
        prev = temp;
    }
    for (int j = ini + 1; j <= fin; j++) {
        int temp = *(mat + fin * n + j);
        *(mat + fin * n + j) = prev;
        prev = temp;
    }
    for (int i = fin - 1; i >= ini; i--) {
        int temp = *(mat + i * n + fin);
        *(mat + i * n + fin) = prev;
        prev = temp;
    }
    for (int j = fin - 1; j >= ini + 1; j--) {
        int temp = *(mat + ini * n + j);
        *(mat + ini * n + j) = prev;
        prev = temp;
    }
}

void rotar_multinivel_rec(int *mat, int n, int nivel_actual, int total_niveles) {
    if (nivel_actual >= total_niveles) {
        return;
    }
    if (nivel_actual % 2 == 0) {
        rotar_anillo_horario(mat, n, nivel_actual);
    } else {
        rotar_anillo_antihorario(mat, n, nivel_actual);
    }
    rotar_multinivel_rec(mat, n, nivel_actual + 1, total_niveles);
}

void verificar_palindromos(const int *mat, int n) {
    int filas_pal = 0;
    int cols_pal = 0;

    printf("\n--- VALIDACION DE FILAS Y COLUMNAS PALINDROMICAS ---\n");
    for (int i = 0; i < n; i++) {
        int es_pal = 1;
        for (int j = 0; j < n / 2; j++) {
            if (*(mat + i * n + j) != *(mat + i * n + (n - 1 - j))) {
                es_pal = 0;
                break;
            }
        }
        if (es_pal) {
            printf("  * La Fila %d es palindromica.\n", i);
            filas_pal++;
        }
    }

    for (int j = 0; j < n; j++) {
        int es_pal = 1;
        for (int i = 0; i < n / 2; i++) {
            if (*(mat + i * n + j) != *(mat + (n - 1 - i) * n + j)) {
                es_pal = 0;
                break;
            }
        }
        if (es_pal) {
            printf("  * La Columna %d es palindromica.\n", j);
            cols_pal++;
        }
    }

    if (filas_pal == 0 && cols_pal == 0) {
        printf("  * Ninguna fila ni columna es palindromica tras la rotacion.\n");
    } else {
        printf("  -> Total filas palindromicas: %d | Total columnas palindromicas: %d\n",
               filas_pal, cols_pal);
    }
}

int main(void) {
    srand((unsigned int)time(NULL));
    printf("=================================================================\n");
    printf("   ROTACION MULTINIVEL DE ANILLOS EN MATRICES CUADRADAS\n");
    printf("=================================================================\n\n");

    int n = leer_entero("Ingrese dimension N de la matriz cuadrada (2 - 12): ", 2, 12);
    int modo = leer_entero("Modo de datos (1 = Aleatorios 1..9, 2 = Manual): ", 1, 2);

    int *mat = (int *)malloc(n * n * sizeof(int));
    if (!mat) {
        printf("Error al reservar memoria.\n");
        system("pause");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (modo == 1) {
                *(mat + i * n + j) = (rand() % 9) + 1;
            } else {
                char msg[64];
                snprintf(msg, sizeof(msg), "  Celda [%d][%d] (0 - 99): ", i, j);
                *(mat + i * n + j) = leer_entero(msg, 0, 99);
            }
        }
    }

    imprimir_matriz(mat, n, "MATRIZ ANTES DE LA ROTACION");

    int niveles = n / 2;
    printf("\n--- INFORMACION DE NIVELES (ANILLOS) ---\n");
    printf("  * Numero de anillos concentricos: %d (%s)\n",
           niveles, (niveles % 2 == 0) ? "PAR" : "IMPAR");
    for (int k = 0; k < niveles; k++) {
        printf("  * Anillo %d -> Rota en sentido %s\n",
               k, (k % 2 == 0) ? "HORARIO" : "ANTIHORARIO");
    }

    rotar_multinivel_rec(mat, n, 0, niveles);

    imprimir_matriz(mat, n, "MATRIZ DESPUES DE LA ROTACION");
    verificar_palindromos(mat, n);

    printf("\n");
    free(mat);
    system("pause");
    return 0;
}
