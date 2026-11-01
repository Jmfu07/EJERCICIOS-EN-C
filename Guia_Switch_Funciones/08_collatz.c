/* EJERCICIO 8: COLLATZ
Funcionamiento: si n es par se divide entre 2; si es impar se calcula 3n+1; termina en 1.
Tipo: long long para soportar valores mayores durante la secuencia.
*/
#include <stdio.h>
int main(void) {
    long long n;
    scanf("%lld",&n);
    if(n<=0)return 0;
    while(n!=1) {
        printf("%lld ",n);
        if(n%2==0)n/=2;
        else n=3*n+1;
    }
    printf("1\n");
    return 0;
}
