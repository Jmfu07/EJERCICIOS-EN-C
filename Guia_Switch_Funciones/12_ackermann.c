/* EJERCICIO 12: ACKERMANN
Funcionamiento: A(0,n)=n+1; A(m,0)=A(m-1,1); en otro caso A(m,n)=A(m-1,A(m,n-1)).
Tipos: unsigned int para argumentos naturales y unsigned long long para el resultado.
*/
#include <stdio.h>
unsigned long long A(unsigned int m,unsigned int n) {
    if(!m)return n+1;
    if(!n)return A(m-1,1);
    return A(m-1,(unsigned int)A(m,n-1));
}
int main(void) {
    unsigned int m,n;
    scanf("%u%u",&m,&n);
    if(m<=3&&n<=10)printf("%llu\n",A(m,n));
    else puts("Valores demasiado grandes");
    return 0;
}
