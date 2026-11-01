/* EJERCICIO 14: NUMERO PRIMO
Funcionamiento: prueba divisores desde 2; si alguno divide exactamente, no es primo.
Tipos: int para numero, divisor y resultado logico.
*/
#include <stdio.h>
#include <stdlib.h>
int primo(int n,int d) {
    if(n<2)return 0;
    if(d>n/d)return 1;
    if(n%d==0)return 0;
    return primo(n,d+1);
}
int main(void) {
    int n;
    scanf("%d",&n);
    printf(primo(n,2)?"Es primo\n":"No es primo\n");
    system("pause");
    return 0;
}
