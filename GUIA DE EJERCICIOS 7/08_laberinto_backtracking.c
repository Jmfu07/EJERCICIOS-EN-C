/*
 * ============================================================================
 * EJERCICIO 8: Resolucion de Laberinto con Restricciones y Backtracking
 * ============================================================================
 * Algoritmo: Carga un laberinto desde un archivo de texto ("laberinto.txt").
 *            Si el archivo no existe, lo crea automaticamente con un laberinto
 *            de ejemplo donde '#' es pared, 'E' es entrada, 'S' es salida y
 *            '.', '+', '*' son simbolos de terreno transitables.
 *            Mediante una funcion recursiva con backtracking encuentra TODOS
 *            los caminos posibles de 'E' a 'S' cumpliendo la restriccion de
 *            no pasar por mas de 2 celdas consecutivas con el mismo simbolo.
 *            Registra el camino mas corto, el mas largo y verifica si algun
 *            camino visita las 4 esquinas del laberinto.
 * Funciones:
 *   - cargar_o_crear_laberinto(): Lee el archivo o crea uno por defecto.
 *   - buscar_caminos_rec(): Backtracking recursivo con restriccion de simbolos.
 * Punteros y Recursividad:
 *   - Matrices dinamicas de caracteres y visitados manipuladas con punteros.
 * Tipos de datos: char, int, struct Coordenada, FILE *.
 * ============================================================================
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    int f;
    int c;
} Coord;

void crear_laberinto_por_defecto(const char *nombre_archivo) {
    FILE *fp = fopen(nombre_archivo, "w");
    if (!fp) return;
    fprintf(fp, "4 5\n");
    fprintf(fp, "E.+.*\n");
    fprintf(fp, "+#+.+\n");
    fprintf(fp, ".*.+.\n");
    fprintf(fp, "+.*.S\n");
    fclose(fp);
}

char **cargar_o_crear_laberinto(const char *nombre_archivo, int *filas, int *cols) {
    FILE *fp = fopen(nombre_archivo, "r");
    if (!fp) {
        printf("  [Aviso] '%s' no encontrado. Creando archivo de ejemplo...\n", nombre_archivo);
        crear_laberinto_por_defecto(nombre_archivo);
        fp = fopen(nombre_archivo, "r");
        if (!fp) return NULL;
    }

    if (fscanf(fp, "%d %d", filas, cols) != 2 || *filas <= 0 || *cols <= 0) {
        fclose(fp);
        crear_laberinto_por_defecto(nombre_archivo);
        fp = fopen(nombre_archivo, "r");
        if (!fp || fscanf(fp, "%d %d", filas, cols) != 2) {
            if (fp) fclose(fp);
            return NULL;
        }
    }

    char **lab = (char **)malloc((*filas) * sizeof(char *));
    for (int i = 0; i < *filas; i++) {
        *(lab + i) = (char *)malloc((*cols + 2) * sizeof(char));
        fscanf(fp, "%s", *(lab + i));
    }
    fclose(fp);
    return lab;
}

typedef struct {
    int total_caminos;
    int min_len;
    int max_len;
    Coord *camino_corto;
    Coord *camino_largo;
    int pasa_4_esquinas;
} EstadisticasLab;

void buscar_caminos_rec(char **lab, int **vis, int filas, int cols,
                        int f, int c, char ultimo_simb, int consec,
                        Coord *camino_act, int len_act,
                        EstadisticasLab *est) {
    char simb_actual = *(*(lab + f) + c);

    if (simb_actual == 'S') {
        est->total_caminos++;

        int esq1 = 0, esq2 = 0, esq3 = 0, esq4 = 0;
        for (int i = 0; i < len_act; i++) {
            int rf = (camino_act + i)->f;
            int rc = (camino_act + i)->c;
            if (rf == 0 && rc == 0) esq1 = 1;
            if (rf == 0 && rc == cols - 1) esq2 = 1;
            if (rf == filas - 1 && rc == 0) esq3 = 1;
            if (rf == filas - 1 && rc == cols - 1) esq4 = 1;
        }
        if (esq1 && esq2 && esq3 && esq4) {
            est->pasa_4_esquinas = 1;
        }

        if (len_act < est->min_len) {
            est->min_len = len_act;
            for (int i = 0; i < len_act; i++) {
                *(est->camino_corto + i) = *(camino_act + i);
            }
        }
        if (len_act > est->max_len) {
            est->max_len = len_act;
            for (int i = 0; i < len_act; i++) {
                *(est->camino_largo + i) = *(camino_act + i);
            }
        }
        return;
    }

    const int df[4] = {-1, 1, 0, 0};
    const int dc[4] = {0, 0, -1, 1};

    for (int d = 0; d < 4; d++) {
        int nf = f + df[d];
        int nc = c + dc[d];

        if (nf >= 0 && nf < filas && nc >= 0 && nc < cols) {
            char sig_simb = *(*(lab + nf) + nc);
            if (sig_simb != '#' && *(*(vis + nf) + nc) == 0) {
                int nuevo_consec = (sig_simb == ultimo_simb) ? (consec + 1) : 1;
                if (nuevo_consec <= 2) {
                    *(*(vis + nf) + nc) = 1;
                    (camino_act + len_act)->f = nf;
                    (camino_act + len_act)->c = nc;

                    buscar_caminos_rec(lab, vis, filas, cols, nf, nc,
                                       sig_simb, nuevo_consec,
                                       camino_act, len_act + 1, est);

                    *(*(vis + nf) + nc) = 0;
                }
            }
        }
    }
}

void imprimir_camino(char **lab, int filas, int cols, const Coord *camino, int len, const char *titulo) {
    printf("\n--- %s (Longitud: %d celdas) ---\n  Ruta: ", titulo, len);
    for (int i = 0; i < len; i++) {
        int f = (camino + i)->f;
        int c = (camino + i)->c;
        printf("(%d,%d)[%c]%s", f, c, *(*(lab + f) + c), (i + 1 < len) ? " -> " : "\n");
    }

    printf("\n  Mapa del camino ('@' = celda recorrida):\n");
    for (int i = 0; i < filas; i++) {
        printf("    ");
        for (int j = 0; j < cols; j++) {
            int en_camino = 0;
            for (int k = 0; k < len; k++) {
                if ((camino + k)->f == i && (camino + k)->c == j) {
                    en_camino = 1;
                    break;
                }
            }
            char ch = *(*(lab + i) + j);
            if (en_camino && ch != 'E' && ch != 'S') {
                printf("@ ");
            } else {
                printf("%c ", ch);
            }
        }
        printf("\n");
    }
}

int main(void) {
    printf("=================================================================\n");
    printf("   RESOLUCION DE LABERINTO CON BACKTRACKING Y RESTRICCIONES\n");
    printf("=================================================================\n");
    printf("Regla: No pasar por mas de 2 celdas consecutivas con el mismo simbolo.\n\n");

    const char *archivo = "laberinto.txt";
    int filas = 0, cols = 0;
    char **lab = cargar_o_crear_laberinto(archivo, &filas, &cols);
    if (!lab) {
        printf("Error al cargar el laberinto.\n");
        system("pause");
        return 1;
    }

    printf("--- LABERINTO CARGADO DESDE '%s' (%dx%d) ---\n", archivo, filas, cols);
    int f_ini = 0, c_ini = 0;
    for (int i = 0; i < filas; i++) {
        printf("  ");
        for (int j = 0; j < cols; j++) {
            char ch = *(*(lab + i) + j);
            printf("%c ", ch);
            if (ch == 'E') {
                f_ini = i;
                c_ini = j;
            }
        }
        printf("\n");
    }

    int **vis = (int **)malloc(filas * sizeof(int *));
    for (int i = 0; i < filas; i++) {
        *(vis + i) = (int *)calloc(cols, sizeof(int));
    }

    int max_celdas = filas * cols;
    Coord *camino_act = (Coord *)malloc(max_celdas * sizeof(Coord));
    EstadisticasLab est;
    est.total_caminos = 0;
    est.min_len = max_celdas + 1;
    est.max_len = 0;
    est.camino_corto = (Coord *)malloc(max_celdas * sizeof(Coord));
    est.camino_largo = (Coord *)malloc(max_celdas * sizeof(Coord));
    est.pasa_4_esquinas = 0;

    *(*(vis + f_ini) + c_ini) = 1;
    camino_act->f = f_ini;
    camino_act->c = c_ini;

    buscar_caminos_rec(lab, vis, filas, cols, f_ini, c_ini,
                       *(*(lab + f_ini) + c_ini), 1, camino_act, 1, &est);

    printf("\n--- RESULTADOS DEL BACKTRACKING ---\n");
    printf("  * Cantidad total de caminos validos encontrados: %d\n", est.total_caminos);
    printf("  * Existe algun camino que pase por las 4 esquinas?: %s\n",
           est.pasa_4_esquinas ? "SI" : "NO");

    if (est.total_caminos > 0) {
        imprimir_camino(lab, filas, cols, est.camino_corto, est.min_len, "CAMINO MAS CORTO");
        imprimir_camino(lab, filas, cols, est.camino_largo, est.max_len, "CAMINO MAS LARGO");
    } else {
        printf("  No se encontraron caminos que cumplan la restriccion.\n");
    }

    printf("\n");
    for (int i = 0; i < filas; i++) {
        free(*(lab + i));
        free(*(vis + i));
    }
    free(lab);
    free(vis);
    free(camino_act);
    free(est.camino_corto);
    free(est.camino_largo);

    system("pause");
    return 0;
}
