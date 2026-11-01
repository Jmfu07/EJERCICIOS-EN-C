/* EJERCICIO 13: MCD
Funcionamiento: aplica Euclides: MCD(x,y)=MCD(y,x%y), hasta que y sea 0.
Tipos: int para ambos enteros y el resultado.
*/
#include <stdio.h>
#include <stdlib.h>

int mcd(int x,int y) {
    if(y==0)return x<0?-x:x;
    return mcd(y,x%y);
}

int main(void) {
    int x,y;
    scanf("%d%d",&x,&y);
    printf("MCD: %d\n",mcd(x,y));
    system("pause");
    return 0;
}
