/*
 * ============================================================================
 * EJERCICIO 7: Analizador de Matriz con Transformaciones Recursivas y Punteros
 * ============================================================================
 * Algoritmo: Lee dinamicamente una matriz cuadrada N x N con punteros dobles
 *            (double **). Calcula su determinante recursivamente por cofactores
 *            para verificar si es de rango completo (det != 0) y evalua si es
 *            ortogonal verificando que A * A^T = I (producto punto entre filas
 *            i, j igual a 1 si i==j y 0 si i!=j). Luego genera la matriz
 *            transpuesta in-place mediante una funcion recursiva que intercambia
 *            unicamente elementos con aritmetica de punteros sin matriz auxiliar.
 * Funciones:
 *   - transponer_inplace_rec(): Transpone recursivamente in-place.
 *   - determinante_rec(): Calcula determinante por expansion de cofactores.
 *   - es_ortogonal(): Verifica ortogonalidad con punteros.
 * Punteros y Recursividad:
 *   - Punteros dobles (double **) y aritmetica *(*(mat + f) + c).
 * Tipos de datos: double, int.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

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

void imprimir_matriz(double **mat, int n, const char *titulo) {
    printf("\n--- %s (%dx%d) ---\n", titulo, n, n);
    for (int i = 0; i < n; i++) {
        printf("  ");
        for (int j = 0; j < n; j++) {
            printf("%8.3f ", *(*(mat + i) + j));
        }
        printf("\n");
    }
}

void transponer_inplace_rec(double **mat, int n, int f, int c) {
    if (f >= n) {
        return;
    }
    if (c >= n) {
        transponer_inplace_rec(mat, n, f + 1, f + 2);
        return;
    }
    double temp = *(*(mat + f) + c);
    *(*(mat + f) + c) = *(*(mat + c) + f);
    *(*(mat + c) + f) = temp;

    transponer_inplace_rec(mat, n, f, c + 1);
}

int es_ortogonal(double **mat, int n) {
    const double eps = 1e-5;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            double prod = 0.0;
            for (int k = 0; k < n; k++) {
                prod += (*(*(mat + i) + k)) * (*(*(mat + j) + k));
            }
            double esperado = (i == j) ? 1.0 : 0.0;
            if (fabs(prod - esperado) > eps) {
                return 0;
            }
        }
    }
    return 1;
}

double determinante_rec(double **mat, int n, int mask_filas, int mask_cols, int orden_act) {
    if (orden_act == 1) {
        for (int i = 0; i < n; i++) {
            if (!(mask_filas & (1 << i))) {
                for (int j = 0; j < n; j++) {
                    if (!(mask_cols & (1 << j))) {
                        return *(*(mat + i) + j);
                    }
                }
            }
        }
        return 0.0;
    }

    int fila_piv = -1;
    for (int i = 0; i < n; i++) {
        if (!(mask_filas & (1 << i))) {
            fila_piv = i;
            break;
        }
    }

    double det = 0.0;
    int signo = 1;
    for (int j = 0; j < n; j++) {
        if (!(mask_cols & (1 << j))) {
            double elem = *(*(mat + fila_piv) + j);
            double sub_det = determinante_rec(mat, n,
                                              mask_filas | (1 << fila_piv),
                                              mask_cols | (1 << j),
                                              orden_act - 1);
            det += signo * elem * sub_det;
            signo = -signo;
        }
    }
    return det;
}

int main(void) {
    printf("=================================================================\n");
    printf("   ANALIZADOR DE MATRIZ: TRANSPUESTA IN-PLACE Y ORTOGONALIDAD\n");
    printf("=================================================================\n\n");

    int n = leer_entero("Ingrese el orden N de la matriz cuadrada (2 - 6): ", 2, 6);
    int modo = leer_entero("Modo (1 = Ingreso manual, 2 = Ejemplo matriz ortogonal): ", 1, 2);

    double **mat = (double **)malloc(n * sizeof(double *));
    for (int i = 0; i < n; i++) {
        *(mat + i) = (double *)malloc(n * sizeof(double));
        for (int j = 0; j < n; j++) {
            if (modo == 1) {
                char msg[64];
                snprintf(msg, sizeof(msg), "  Valor [%d][%d] (-100 a 100): ", i, j);
                *(*(mat + i) + j) = leer_double(msg, -100.0, 100.0);
            } else {
                *(*(mat + i) + j) = ((i + 1) % n == j) ? 1.0 : 0.0;
            }
        }
    }

    imprimir_matriz(mat, n, "MATRIZ ORIGINAL (A)");

    int ortogonal = es_ortogonal(mat, n);
    double det = determinante_rec(mat, n, 0, 0, n);
    int rango_completo = (fabs(det) > 1e-7);

    transponer_inplace_rec(mat, n, 0, 1);

    imprimir_matriz(mat, n, "MATRIZ TRANSPUESTA IN-PLACE (A^T)");

    int simetrica = 1;
    /* Comparamos si A == A^T verificando elementos fuera de la diagonal */
    for (int i = 0; i < n; i++) {
        for (int j = i + 1; j < n; j++) {
            if (fabs(*(*(mat + i) + j) - *(*(mat + j) + i)) > 1e-6) {
                simetrica = 0;
            }
        }
    }

    printf("\n=================================================================\n");
    printf("   INFORME FINAL COMPARATIVO DE PROPIEDADES\n");
    printf("=================================================================\n");
    printf("  * Dimension de la matriz : %d x %d\n", n, n);
    printf("  * Determinante calculado : %.4f\n", det);
    printf("  * Es de Rango Completo?  : %s (det %s 0)\n",
           rango_completo ? "SI (Invertible, Rango = N)" : "NO (Singular, Rango < N)",
           rango_completo ? "!=" : "==");
    printf("  * Es Matriz Ortogonal?   : %s (A * A^T %s I)\n",
           ortogonal ? "SI (A^T = A^-1)" : "NO",
           ortogonal ? "==" : "!=");
    printf("  * Es Matriz Simetrica?   : %s (A %s A^T)\n\n",
           simetrica ? "SI" : "NO",
           simetrica ? "==" : "!=");

    for (int i = 0; i < n; i++) {
        free(*(mat + i));
    }
    free(mat);

    system("pause");
    return 0;
}
