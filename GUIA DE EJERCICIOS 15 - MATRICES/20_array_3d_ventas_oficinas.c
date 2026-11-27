/* EJERCICIO 20: ARRAY MULTIDIMENSIONAL 3D (ventasEmpleadoOficinaEdificio)
 * Del material: "Arrays o arreglos multidimensionales en C (CU00515F)".
 * Declara un arreglo tridimensional [6][10][2] (empleados x oficinas x edificios),
 * asigna 4234 al elemento (empleado 3, oficina 7, edificio 2 -> indices [2][6][1]),
 * muestra total de elementos, modifica sumando 10 y permite consultas validadas.
 */
#include <stdio.h>
#include <stdlib.h>

static void limpiar_buffer(void) {
    int c;
    while ((c = getchar()) != '\n' && c != EOF) {}
}

static int leer_rango(const char *msg, int min, int max) {
    int v;
    for (;;) {
        printf("%s [%d a %d]: ", msg, min, max);
        if (scanf("%d", &v) == 1 && v >= min && v <= max) {
            limpiar_buffer();
            return v;
        }
        limpiar_buffer();
        printf("  Error: valor fuera de rango (%d a %d).\n", min, max);
    }
}

int main(void) {
    int ventasEmpleadoOficinaEdificio[6][10][2] = {{{0}}};
    int emp, ofi, edi, total_elem = 6 * 10 * 2;

    /* Empleado 3 -> indice 2, Oficina 7 -> indice 6, Edificio 2 -> indice 1 */
    ventasEmpleadoOficinaEdificio[2][6][1] = 4234;

    printf("=== EJERCICIO 20: ARRAY 3D DE VENTAS (6 x 10 x 2) ===\n\n");
    printf("El empleado numero 3 de la oficina numero 7 del edificio numero 2 ha vendido %d unidades.\n",
           ventasEmpleadoOficinaEdificio[2][6][1]);

    printf("\nRespuestas:\n");
    printf("a) Total de elementos del array 6x10x2: %d elementos.\n", total_elem);
    printf("b) Acceder a un indice fuera de rango produce comportamiento indefinido (lectura de memoria ajena o cierre).\n");

    ventasEmpleadoOficinaEdificio[2][6][1] += 10;
    printf("c) Tras sumar 10: El empleado numero 3 de la oficina numero 7 del edificio numero 2 ha vendido %d unidades.\n\n",
           ventasEmpleadoOficinaEdificio[2][6][1]);

    printf("--- Consulta interactiva (indices validados) ---\n");
    emp = leer_rango("Empleado", 1, 6);
    ofi = leer_rango("Oficina", 1, 10);
    edi = leer_rango("Edificio", 1, 2);
    printf("Ventas registradas para (%d, %d, %d): %d unidades.\n\n",
           emp, ofi, edi, ventasEmpleadoOficinaEdificio[emp - 1][ofi - 1][edi - 1]);

    system("pause");
    return 0;
}
