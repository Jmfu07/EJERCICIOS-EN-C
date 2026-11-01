/* EJERCICIO 15: SUMA DE MATRIZ
Funcionamiento: la posicion lineal visita cada celda; fila=pos/columnas y columna=pos%columnas.
Tipos: int[][] para la matriz y int para dimensiones, posicion y suma.
*/
#include <stdio.h>
#include <stdlib.h>

int suma(int a[][20],int f,int c,int p) {
    if(p==f*c)return 0;
    return a[p/c][p%c]+suma(a,f,c,p+1);
}

int main() {
    int a[20][20],f,c,i,j;
    //PRIMEROS 2 scanf para filas y columnas
    scanf("%d%d",&f,&c);
    // Luego, leer los elementos de la matriz
    for(i=0; i<f; i++)for(j=0; j<c; j++)scanf("%d",&a[i][j]);
    // Finalmente, llamar a la función suma y mostrar el resultado
    printf("Suma: %d\n",suma(a,f,c,0));
    system("pause");
    return 0;
}
