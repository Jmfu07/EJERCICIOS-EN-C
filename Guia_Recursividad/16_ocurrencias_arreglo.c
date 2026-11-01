/* EJERCICIO 16: OCURRENCIAS
Funcionamiento: se compara el ultimo elemento y se suma 1 si coincide; luego se reduce n.
Tipos: int[] para arreglo, int para tamano, buscado y contador.
*/
#include <stdio.h>
#include <stdlib.h>
int ocurr(int a[],int n,int x) {
    if(!n)return 0;
    return(a[n-1]==x)+ocurr(a,n-1,x);
}
int main(void) {
    int a[100],n,x,i;
    scanf("%d",&n);
    for(i=0; i<n; i++)scanf("%d",&a[i]);
    scanf("%d",&x);
    printf("Ocurrencias: %d\n",ocurr(a,n,x));
    system("pause");
    return 0;
}
