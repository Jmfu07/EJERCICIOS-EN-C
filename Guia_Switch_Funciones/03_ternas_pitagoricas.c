/* EJERCICIO 3: TERNAS PITAGORICAS
Funcionamiento: se prueban catetos a,b e hipotenusa c y se valida a^2+b^2=c^2.
Tipos: int para lados y ciclos; el producto cabe en int porque los lados llegan a 500.
*/
#include <stdio.h>
int main(void) {
    int a,b,c;
    for(a=1; a<=500; a++)for(b=a; b<=500; b++)for(c=b; c<=500; c++)if(a*a+b*b==c*c)printf("%d %d %d\n",a,b,c);
    return 0;
}
