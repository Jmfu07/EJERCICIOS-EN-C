/*
 * ============================================================================
 * EJERCICIO 6: Sistema de Simulacion de Ecosistema con Matriz Dinamica
 * ============================================================================
 * Algoritmo: Simula un ecosistema en una matriz dinamica F x C con entidades:
 *              0 = Vacio (.)
 *              1 = Presa (P)
 *              2 = Depredador (D)
 *            Reglas por turno (aplicadas recursivamente por turnos):
 *              - Un depredador busca la presa mas cercana en su vecindad;
 *                si la alcanza, se mueve a su celda y la consume (hambre = 0).
 *                Si no encuentra presa cercana, se mueve a celda vacia vecina
 *                e incrementa su contador de turnos sin comer. Si acumula 2
 *                turnos sin presa, muere (la celda pasa a 0).
 *              - Una celda vacia con al menos 2 presas vecinas genera una
 *                nueva presa (reproduccion).
 *            La matriz se actualiza in-place usando codificacion de bits en
 *            las celdas con punteros, sin crear una copia completa de la matriz.
 * Funciones:
 *   - simular_turnos_rec(): Ejecuta recursivamente cada turno del ecosistema.
 *   - contar_estadisticas(): Calcula presas, depredadores y celdas vacias.
 * Punteros y Recursividad:
 *   - Matriz dinamica manipulada exclusivamente con aritmetica de punteros.
 * Tipos de datos: int, punteros int *.
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

void contar_estadisticas(const int *mat, int filas, int cols,
                         int *vacios, int *presas, int *depredadores) {
    *vacios = 0;
    *presas = 0;
    *depredadores = 0;
    for (int i = 0; i < filas * cols; i++) {
        int tipo = *(mat + i) & 0x03;
        if (tipo == 0) (*vacios)++;
        else if (tipo == 1) (*presas)++;
        else if (tipo == 2) (*depredadores)++;
    }
}

void mostrar_ecosistema(const int *mat, int filas, int cols, const char *titulo) {
    int v, p, d;
    contar_estadisticas(mat, filas, cols, &v, &p, &d);
    printf("\n--- %s ---\n", titulo);
    for (int i = 0; i < filas; i++) {
        printf("  ");
        for (int j = 0; j < cols; j++) {
            int tipo = *(mat + i * cols + j) & 0x03;
            char simb = (tipo == 1) ? 'P' : ((tipo == 2) ? 'D' : '.');
            printf("%c(%d) ", simb, tipo);
        }
        printf("\n");
    }
    printf("  [Poblacion] Presas (1): %d | Depredadores (2): %d | Vacios (0): %d\n",
           p, d, v);
}

int contar_presas_vecinas(const int *mat, int filas, int cols, int f, int c) {
    int conteo = 0;
    for (int df = -1; df <= 1; df++) {
        for (int dc = -1; dc <= 1; dc++) {
            if (df == 0 && dc == 0) continue;
            int nf = f + df, nc = c + dc;
            if (nf >= 0 && nf < filas && nc >= 0 && nc < cols) {
                int v = *(mat + nf * cols + nc);
                if ((v & 0x03) == 1 && !((v >> 4) & 1)) conteo++;
            }
        }
    }
    return conteo;
}

/*
 * Codificacion in-place en cada entero de la matriz:
 *   Bits 0..1 : Estado actual (0=vacio, 1=presa, 2=depredador)
 *   Bits 2..3 : Turnos sin comer del depredador (0..2)
 *   Bit  4    : Flag de entidad ya movida/procesada en el turno actual
 */
void simular_turnos_rec(int *mat, int filas, int cols, int turno_act, int total_turnos) {
    if (turno_act > total_turnos) {
        return;
    }

    for (int i = 0; i < filas * cols; i++) {
        *(mat + i) &= ~16;
    }

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < cols; c++) {
            int celda = *(mat + f * cols + c);
            int tipo = celda & 0x03;
            int movido = (celda >> 4) & 1;

            if (tipo == 2 && !movido) {
                int hambre = (celda >> 2) & 0x03;
                int mejor_nf = -1, mejor_nc = -1;
                int dist_min = 9999;

                for (int nf = 0; nf < filas; nf++) {
                    for (int nc = 0; nc < cols; nc++) {
                        if ((*(mat + nf * cols + nc) & 0x03) == 1) {
                            int dist = abs(nf - f) + abs(nc - c);
                            if (dist < dist_min) {
                                dist_min = dist;
                                mejor_nf = nf;
                                mejor_nc = nc;
                            }
                        }
                    }
                }

                int paso_f = f, paso_c = c;
                if (mejor_nf != -1) {
                    if (mejor_nf > f) paso_f++;
                    else if (mejor_nf < f) paso_f--;
                    if (mejor_nc > c) paso_c++;
                    else if (mejor_nc < c) paso_c--;
                }

                int dest_tipo = *(mat + paso_f * cols + paso_c) & 0x03;
                if ((paso_f != f || paso_c != c) && dest_tipo == 1) {
                    *(mat + paso_f * cols + paso_c) = 2 | (0 << 2) | 16;
                    *(mat + f * cols + c) = 0;
                } else {
                    hambre++;
                    if (hambre >= 2) {
                        *(mat + f * cols + c) = 0;
                    } else if ((paso_f != f || paso_c != c) && dest_tipo == 0) {
                        *(mat + paso_f * cols + paso_c) = 2 | (hambre << 2) | 16;
                        *(mat + f * cols + c) = 0;
                    } else {
                        *(mat + f * cols + c) = 2 | (hambre << 2) | 16;
                    }
                }
            }
        }
    }

    for (int f = 0; f < filas; f++) {
        for (int c = 0; c < cols; c++) {
            int celda = *(mat + f * cols + c);
            int tipo = celda & 0x03;
            int movido = (celda >> 4) & 1;
            if (tipo == 0 && !movido) {
                if (contar_presas_vecinas(mat, filas, cols, f, c) >= 2) {
                    *(mat + f * cols + c) = 1 | 16;
                }
            }
        }
    }

    char titulo[64];
    snprintf(titulo, sizeof(titulo), "ESTADO TRAS TURNO %d", turno_act);
    mostrar_ecosistema(mat, filas, cols, titulo);

    simular_turnos_rec(mat, filas, cols, turno_act + 1, total_turnos);
}

int main(void) {
    srand((unsigned int)time(NULL));
    printf("=================================================================\n");
    printf("   SIMULACION DE ECOSISTEMA CON MATRIZ DINAMICA Y RECURSIVIDAD\n");
    printf("=================================================================\n\n");

    int filas  = leer_entero("Numero de filas del ecosistema (3 - 10): ", 3, 10);
    int cols   = leer_entero("Numero de columnas del ecosistema (3 - 10): ", 3, 10);
    int turnos = leer_entero("Cantidad de turnos a simular (1 - 10): ", 1, 10);

    int *mat = (int *)malloc(filas * cols * sizeof(int));
    if (!mat) {
        printf("Error de memoria.\n");
        system("pause");
        return 1;
    }

    for (int i = 0; i < filas * cols; i++) {
        int r = rand() % 100;
        if (r < 45) *(mat + i) = 0;
        else if (r < 80) *(mat + i) = 1;
        else *(mat + i) = 2;
    }

    int v_ini, p_ini, d_ini;
    contar_estadisticas(mat, filas, cols, &v_ini, &p_ini, &d_ini);
    mostrar_ecosistema(mat, filas, cols, "ESTADO INICIAL DEL ECOSISTEMA");

    simular_turnos_rec(mat, filas, cols, 1, turnos);

    int v_fin, p_fin, d_fin;
    contar_estadisticas(mat, filas, cols, &v_fin, &p_fin, &d_fin);

    printf("\n=================================================================\n");
    printf("   ESTADISTICAS FINALES DE POBLACION (Comparativa)\n");
    printf("=================================================================\n");
    printf("  * Presas       : Inicial = %3d | Final = %3d | Diferencia = %+d\n",
           p_ini, p_fin, p_fin - p_ini);
    printf("  * Depredadores : Inicial = %3d | Final = %3d | Diferencia = %+d\n",
           d_ini, d_fin, d_fin - d_ini);
    printf("  * Celdas Vacias: Inicial = %3d | Final = %3d | Diferencia = %+d\n\n",
           v_ini, v_fin, v_fin - v_ini);

    free(mat);
    system("pause");
    return 0;
}
