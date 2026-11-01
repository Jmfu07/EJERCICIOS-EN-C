/* EJERCICIO 12: DECIMAL A BINARIO
Funcionamiento: se divide entre 2 recursivamente y se imprime el residuo al regresar.
Tipos: unsigned int representa el numero no negativo.
*/
#include <stdio.h>
void bin(unsigned int n) {
    if(n>1)bin(n/2);
    printf("%u",n%2);
}
int main(void) {
    unsigned int n;
    scanf("%u",&n);
    if(!n)printf("0");
    else bin(n);
    printf("\n");
    return 0;
}
