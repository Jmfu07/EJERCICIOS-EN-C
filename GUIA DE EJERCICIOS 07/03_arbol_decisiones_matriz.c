/*
 * ============================================================================
 * EJERCICIO 3: Arbol de Decisiones Simulado con Matrices y Recursividad
 * ============================================================================
 * Algoritmo: Representa un arbol/grafo de decisiones sobre una matriz F x C.
 *            Cada celda contiene un codigo de decision:
 *              0 = Nodo Terminal (Meta alcanzada con exito)
 *              1 = Mover Arriba    (-1,  0)
 *              2 = Mover Abajo     (+1,  0)
 *              3 = Mover Izquierda ( 0, -1)
 *              4 = Mover Derecha   ( 0, +1)
 *              5 = Mover Diagonal Inferior-Derecha (+1, +1)
 *            Una funcion recursiva simula el recorrido desde una celda inicial
 *            marcando las celdas visitadas mediante punteros para detectar
 *            ciclos/repeticiones o salidas de limites.
 * Funciones:
 *   - recorrer_arbol_rec(): Recorre recursivamente la matriz de decisiones.
 * Punteros y Recursividad:
 *   - Acceso a matrices de decisiones y visitados mediante aritmetica de punteros.
 * Tipos de datos: int, punteros int *.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>

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
            printf("  [Error] El valor debe estar entre %d y %d.\n", min, max);
            limpiar_buffer();
        } else {
            limpiar_buffer();
            return valor;
        }
    }
}

const char *nombre_decision(int cod) {
    switch (cod) {
        case 0: return "TERMINAL(0)";
        case 1: return "ARRIBA(1)";
        case 2: return "ABAJO(2)";
        case 3: return "IZQUIERDA(3)";
        case 4: return "DERECHA(4)";
        case 5: return "DIAG_INF_DER(5)";
        default: return "DESCONOCIDO";
    }
}

/*
 * Retorna:
 *   1 = Exito (llego a nodo terminal 0)
 *  -1 = Fallo por ciclo (celda repetida)
 *  -2 = Fallo por salir de los limites
 */
int recorrer_arbol_rec(const int *mat, int *visitado, int filas, int cols,
                       int f_act, int c_act, int *decisiones) {
    if (f_act < 0 || f_act >= filas || c_act < 0 || c_act >= cols) {
        printf(" -> [FUERA DE LIMITES en (%d, %d)]\n", f_act, c_act);
        return -2;
    }

    if (*(visitado + f_act * cols + c_act) == 1) {
        printf(" -> [CICLO DETECTADO: Celda (%d, %d) repetida]\n", f_act, c_act);
        return -1;
    }

    *(visitado + f_act * cols + c_act) = 1;
    int cod = *(mat + f_act * cols + c_act);

    printf("  Paso %2d: Celda (%d, %d) -> Decision: %s\n",
           *decisiones, f_act, c_act, nombre_decision(cod));

    if (cod == 0) {
        return 1;
    }

    (*decisiones)++;
    int df = 0, dc = 0;
    if (cod == 1) { df = -1; dc =  0; }
    else if (cod == 2) { df =  1; dc =  0; }
    else if (cod == 3) { df =  0; dc = -1; }
    else if (cod == 4) { df =  0; dc =  1; }
    else if (cod == 5) { df =  1; dc =  1; }

    return recorrer_arbol_rec(mat, visitado, filas, cols,
                              f_act + df, c_act + dc, decisiones);
}

int main(void) {
    printf("=================================================================\n");
    printf("   ARBOL DE DECISIONES SIMULADO CON MATRICES Y RECURSIVIDAD\n");
    printf("=================================================================\n");
    printf("Codigos: 0=Terminal, 1=Arriba, 2=Abajo, 3=Izquierda, 4=Derecha, 5=Diag\n\n");

    int filas = leer_entero("Numero de filas (2 - 10): ", 2, 10);
    int cols  = leer_entero("Numero de columnas (2 - 10): ", 2, 10);
    int modo  = leer_entero("Modo (1 = Escenario predeterminado, 2 = Ingreso manual): ", 1, 2);

    int *mat = (int *)malloc(filas * cols * sizeof(int));
    int *visitado = (int *)calloc(filas * cols, sizeof(int));

    if (!mat || !visitado) {
        printf("Error de memoria.\n");
        system("pause");
        return 1;
    }

    if (modo == 1) {
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < cols; j++) {
                if (i == filas - 1 && j == cols - 1) {
                    *(mat + i * cols + j) = 0;
                } else if (i == 0 && j < cols - 1) {
                    *(mat + i * cols + j) = 4;
                } else if (j == cols - 1 && i < filas - 1) {
                    *(mat + i * cols + j) = 2;
                } else if (i == 1 && j == 0) {
                    *(mat + i * cols + j) = 2;
                } else if (i == 2 && j == 0) {
                    *(mat + i * cols + j) = 1;
                } else {
                    *(mat + i * cols + j) = 5;
                }
            }
        }
    } else {
        for (int i = 0; i < filas; i++) {
            for (int j = 0; j < cols; j++) {
                char msg[64];
                snprintf(msg, sizeof(msg), "  Decision en [%d][%d] (0-5): ", i, j);
                *(mat + i * cols + j) = leer_entero(msg, 0, 5);
            }
        }
    }

    printf("\n--- MATRIZ DE DECISIONES ---\n");
    for (int i = 0; i < filas; i++) {
        printf("  ");
        for (int j = 0; j < cols; j++) {
            printf("%2d ", *(mat + i * cols + j));
        }
        printf("\n");
    }

    printf("\nSeleccione posicion de inicio:\n");
    int f_ini = leer_entero("  Fila inicial: ", 0, filas - 1);
    int c_ini = leer_entero("  Columna inicial: ", 0, cols - 1);

    printf("\n--- CAMINO RECORRIDO ---\n");
    int decisiones = 0;
    int estado = recorrer_arbol_rec(mat, visitado, filas, cols, f_ini, c_ini, &decisiones);

    printf("\n--- RESUMEN DEL RECORRIDO ---\n");
    printf("  * Numero de decisiones tomadas: %d\n", decisiones);
    if (estado == 1) {
        printf("  * Resultado del recorrido: VALIDO (Se alcanzo un nodo terminal)\n\n");
    } else if (estado == -1) {
        printf("  * Resultado del recorrido: FALLIDO (Detenido por ciclo / celda repetida)\n\n");
    } else {
        printf("  * Resultado del recorrido: FALLIDO (Detenido por salir de los limites)\n\n");
    }

    free(mat);
    free(visitado);
    system("pause");
    return 0;
}
