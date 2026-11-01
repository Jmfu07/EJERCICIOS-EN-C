/* EJERCICIO 5: CONTAR ELEMENTOS
Funcionamiento: cada llamada cuenta un elemento y reduce el tamano; termina en tamano 0.
Tipos: int[] para el arreglo e int para el tamano y el resultado.
*/
#include <stdio.h>
#include <stdlib.h>

int contar(int a[], int n) {
    if (n <= 0) return 0;
    return 1 + contar(a,n-1);
}

int main() {
    int a[100],n,i;
    printf("Cantidad: ");
    scanf("%d",&n);
    if(n<0||n>100) {
        printf("Cantidad invalida.\n");
        system("pause");
        return 0;
    }
    for(i=0; i<n; i++)scanf("%d",&a[i]);
    printf("Elementos: %d\n",contar(a,n));
    system("pause");
    return 0;
}
