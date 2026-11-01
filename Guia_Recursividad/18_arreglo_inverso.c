/* EJERCICIO 18: IMPRIMIR ARREGLO EN ORDEN INVERSO
   Funcionamiento: imprime el ultimo elemento y vuelve a llamar con un tamano menor.
   Tipos: int[] para datos e int para tamano.
*/
#include <stdio.h>
void inverso(int a[],int n){if(!n)return;printf("%d ",a[n-1]);inverso(a,n-1);}
int main(void){int a[100],n,i;scanf("%d",&n);for(i=0;i<n;i++)scanf("%d",&a[i]);inverso(a,n);printf("\n");return 0;}
