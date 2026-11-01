/* EJERCICIO 11: MCD RECURSIVO
Funcionamiento: aplica MCD(x,y)=MCD(y,x%y) hasta que y=0.
Tipo: int para entradas y resultado.
*/
#include <stdio.h>
int mcd(int x,int y) {
    if(!y)return x;
    return mcd(y,x%y);
}
int main(void) {
    int x,y;
    scanf("%d%d",&x,&y);
    printf("%d\n",mcd(x,y));
    return 0;
}
