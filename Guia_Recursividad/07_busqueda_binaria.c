/* EJERCICIO 7: BUSQUEDA BINARIA
Funcionamiento: se compara con el elemento central y se descarta la mitad que no sirve.
El arreglo debe estar ordenado.
Tipos: int[] para datos e int para indices, valor y posicion.
*/
#include <stdio.h>
#include <stdlib.h>

int buscar(int a[],int i,int f,int x) {
    if(i>f)return -1;
    int m=(i+f)/2;
    if(a[m]==x)return m;
    if(x<a[m])return buscar(a,i,m-1,x);
    return buscar(a,m+1,f,x);
}

int main(void) {
    int a[100],n,x,i,p;
    printf("Cantidad: ");
    scanf("%d",&n);
    for(i=0; i<n; i++)scanf("%d",&a[i]);
    printf("Buscar: ");
    scanf("%d",&x);
    p=buscar(a,0,n-1,x);
    printf(p<0?"No encontrado\n":"Posicion: %d\n",p);
    system("pause");
    return 0;
}
