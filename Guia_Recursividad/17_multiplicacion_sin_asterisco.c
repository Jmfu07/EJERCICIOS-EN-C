/* EJERCICIO 17: MULTIPLICACION SIN *
Funcionamiento: multiplicar a por b equivale a sumar a b veces. El caso negativo cambia el signo.
Tipos: int para operandos y resultado.
*/
#include <stdio.h>
int mult(int a,int b) {
    if(!b)return 0;
    if(b<0)return-mult(a,-b);
    return a+mult(a,b-1);
}
int main(void) {
    int a,b;
    scanf("%d%d",&a,&b);
    printf("Resultado: %d\n",mult(a,b));
    return 0;
}
