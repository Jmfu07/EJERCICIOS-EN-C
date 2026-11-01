/* EJERCICIO 19: POTENCIA
Funcionamiento: a^b es 1 cuando b=0; en otro caso es a por a^(b-1).
Tipos: int para base/exponente y long long para resultado.
*/
#include <stdio.h>
#include <stdlib.h>
long long potencia(int a,int b) {
    if(!b)return 1;
    return a*potencia(a,b-1);
}
int main(void) {
    int a,b;
    scanf("%d%d",&a,&b);
    if(b<0)printf("Exponente invalido\n");
    else printf("Resultado: %lld\n",potencia(a,b));
    system("pause");
    return 0;
}
