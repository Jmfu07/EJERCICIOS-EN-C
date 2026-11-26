/*
 * Guia: Ejercicios de Lenguaje C (Progresion I) - Ejercicio 11
 * Enunciado: Escribe un programa que lea 2 numeros enteros de 3 digitos e
 * imprima su producto. Por ejemplo, si los numeros son 325 y 426 se presentara
 * en el formato:
 *        325
 *      x 426
 * ------------
 *       1950
 *       650
 *      1300
 * ------------
 *     138450
 */

#include <stdio.h>
#include <stdlib.h>

int leer_entero_3_digitos(const char *prompt) {
    char buffer[128];
    long valor;
    char *fin;

    while (1) {
        printf("%s", prompt);
        if (!fgets(buffer, sizeof(buffer), stdin)) {
            printf("\nError de lectura.\n");
            exit(1);
        }
        valor = strtol(buffer, &fin, 10);
        while (*fin == ' ' || *fin == '\t') {
            fin++;
        }
        if (fin == buffer || (*fin != '\n' && *fin != '\r' && *fin != '\0')) {
            printf("Entrada invalida. Ingrese un numero entero.\n");
            continue;
        }
        if (valor < 100 || valor > 999) {
            printf("El numero debe ser un entero positivo de 3 digitos (100 a 999).\n");
            continue;
        }
        return (int)valor;
    }
}

int main(void) {
    int a, b;
    int unidades, decenas, centenas;
    int p1, p2, p3;
    long producto;

    printf("=== PROGRESION I - EJERCICIO 11: PRODUCTO DE 2 ENTEROS DE 3 DIGITOS ===\n\n");
    a = leer_entero_3_digitos("Ingrese el primer numero entero de 3 digitos (100-999): ");
    b = leer_entero_3_digitos("Ingrese el segundo numero entero de 3 digitos (100-999): ");

    unidades = b % 10;
    decenas = (b / 10) % 10;
    centenas = b / 100;

    p1 = a * unidades;
    p2 = a * decenas;
    p3 = a * centenas;
    producto = (long)a * (long)b;

    printf("\nFormato de multiplicacion:\n\n");
    printf("%12d\n", a);
    printf("       x %3d\n", b);
    printf("------------\n");
    printf("%12d\n", p1);
    printf("%11d \n", p2);
    printf("%10d  \n", p3);
    printf("------------\n");
    printf("%12ld\n\n", producto);

    system("pause");
    return 0;
}
