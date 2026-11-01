/* EJERCICIO 12: DECIMAL A BINARIO
Funcionamiento: se divide entre 2 recursivamente y se imprime el residuo al regresar.
Tipos: unsigned int representa el numero no negativo.
*/
#include <stdio.h>
#include <stdlib.h>


// Función recursiva clásica SOLO para la parte entera
void bin_entera(int n) {
    if (n > 1) bin_entera(n / 2);
    printf("%d", n % 2);
}

// Función para procesar la parte fraccionaria (multiplicando por 2)
void bin_fraccionaria(double f) {
    if (f == 0) return;
    
    printf("."); // Imprime el punto decimal binario
    
    // Multiplicamos por 2 hasta un límite de dígitos (ej. 6) para evitar bucles infinitos
    for (int i = 0; i < 6 && f > 0; i++) {
        f *= 2;
        int bit = (int)f; // Tomamos la parte entera (0 o 1)
        printf("%d", bit);
        f -= bit;         // Nos quedamos solo con el nuevo residuo decimal
    }
}

int main() {
    double n;
    printf("Introduce un numero decimal: ");
    if (scanf("%lf", &n) != 1) return 1;

    if (n == 0) {
        printf("0\n");
    } else {
        int parte_entera = (int)n;
        double parte_fraccionaria = n - parte_entera;

        // 1. Imprimir parte entera
        bin_entera(parte_entera);
        
        // 2. Imprimir parte fraccionaria si existe
        bin_fraccionaria(parte_fraccionaria);
        
        printf("\n");
    }

    system("pause");
    return 0;
}
