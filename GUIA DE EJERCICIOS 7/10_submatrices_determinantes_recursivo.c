/*
 * ============================================================================
 * EJERCICIO 10: Generador de Submatrices y Determinantes con Recursividad
 * ============================================================================
 * Algoritmo: Lee una matriz cuadrada de tamano N x N y un orden k (1 <= k <= N).
 *            Genera todas las submatrices contiguas de orden k x k utilizando
 *            desplazamientos de punteros sobre la matriz original (sin crear
 *            copias auxiliares de las submatrices). Para cada submatriz calcula
 *            su determinante mediante una funcion recursiva basada en expansion
 *            por cofactores e identifica las submatrices con determinante:
 *              - Mayor
 *              - Menor
 *              - Mas cercano a cero
 * Funciones:
 *   - det_submatriz_rec(): Calcula el determinante k x k por cofactores.
 *   - imprimir_submatriz(): Imprime una submatriz k x k usando punteros.
 * Punteros y Recursividad:
 *   - Puntero base desplazado (mat + f0 * n + c0) y mascara de columnas.
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

/*
 * Calcula el determinante de una submatriz k x k cuyo elemento superior
 * izquierdo apunta a `base_sub`, con salto de fila `stride` (= N),
 * sin crear estructuras auxiliares completas.
 */
double det_submatriz_rec(const double *base_sub, int stride, int k,
                         int fila_rel, int mask_cols) {
    int restantes = k - fila_rel;
    if (restantes == 1) {
        for (int c = 0; c < k; c++) {
            if (!(mask_cols & (1 << c))) {
                return *(base_sub + fila_rel * stride + c);
            }
        }
        return 0.0;
    }

    double det = 0.0;
    int signo = 1;
    for (int c = 0; c < k; c++) {
        if (!(mask_cols & (1 << c))) {
            double elem = *(base_sub + fila_rel * stride + c);
            double sub = det_submatriz_rec(base_sub, stride, k,
                                           fila_rel + 1, mask_cols | (1 << c));
            det += signo * elem * sub;
            signo = -signo;
        }
    }
    return det;
}

void imprimir_submatriz(const double *base_sub, int stride, int k,
                        int f0, int c0, double det, const char *etiqueta) {
    printf("\n  --- %s (Origen Fila %d, Col %d | Det = %.4f) ---\n",
           etiqueta, f0, c0, det);
    for (int i = 0; i < k; i++) {
        printf("    ");
        for (int j = 0; j < k; j++) {
            printf("%8.2f ", *(base_sub + i * stride + j));
        }
        printf("\n");
    }
}

int main(void) {
    printf("=================================================================\n");
    printf("   GENERADOR DE SUBMATRICES Y DETERMINANTES POR COFACTORES\n");
    printf("=================================================================\n\n");

    int n = leer_entero("Ingrese el tamano N de la matriz cuadrada (2 - 7): ", 2, 7);
    int k = leer_entero("Ingrese el orden k de las submatrices (1 - N): ", 1, n);
    int modo = leer_entero("Modo de llenado (1 = Manual, 2 = Valores automaticos): ", 1, 2);

    double *mat = (double *)malloc(n * n * sizeof(double));
    if (!mat) {
        printf("Error de memoria.\n");
        system("pause");
        return 1;
    }

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (modo == 1) {
                char msg[64];
                snprintf(msg, sizeof(msg), "  Celda [%d][%d] (-100 a 100): ", i, j);
                *(mat + i * n + j) = leer_double(msg, -100.0, 100.0);
            } else {
                *(mat + i * n + j) = (double)((i * 3 + j * 2 + 1) % 11 - 4);
            }
        }
    }

    printf("\n--- MATRIZ PRINCIPAL (%dx%d) ---\n", n, n);
    for (int i = 0; i < n; i++) {
        printf("  ");
        for (int j = 0; j < n; j++) {
            printf("%8.2f ", *(mat + i * n + j));
        }
        printf("\n");
    }

    double max_det = -1e30, min_det = 1e30, cero_det = 1e30;
    int f_max = 0, c_max = 0;
    int f_min = 0, c_min = 0;
    int f_cero = 0, c_cero = 0;
    int total_sub = 0;

    printf("\n--- DETERMINANTES DE SUBMATRICES DE ORDEN %dx%d ---\n", k, k);
    for (int f0 = 0; f0 <= n - k; f0++) {
        for (int c0 = 0; c0 <= n - k; c0++) {
            const double *ptr_sub = mat + f0 * n + c0;
            double det = det_submatriz_rec(ptr_sub, n, k, 0, 0);
            total_sub++;

            printf("  Submatriz #%2d en (%d, %d) -> Determinante = %10.4f\n",
                   total_sub, f0, c0, det);

            if (det > max_det) {
                max_det = det;
                f_max = f0;
                c_max = c0;
            }
            if (det < min_det) {
                min_det = det;
                f_min = f0;
                c_min = c0;
            }
            if (fabs(det) < fabs(cero_det)) {
                cero_det = det;
                f_cero = f0;
                c_cero = c0;
            }
        }
    }

    printf("\n=================================================================\n");
    printf("   INFORME FINAL DE SUBMATRICES (%d evaluadas)\n", total_sub);
    printf("=================================================================");
    imprimir_submatriz(mat + f_max * n + c_max, n, k, f_max, c_max, max_det,
                       "SUBMATRIZ CON DETERMINANTE MAYOR");
    imprimir_submatriz(mat + f_min * n + c_min, n, k, f_min, c_min, min_det,
                       "SUBMATRIZ CON DETERMINANTE MENOR");
    imprimir_submatriz(mat + f_cero * n + c_cero, n, k, f_cero, c_cero, cero_det,
                       "SUBMATRIZ CON DETERMINANTE MAS CERCANO A CERO");
    printf("\n");

    free(mat);
    system("pause");
    return 0;
}
