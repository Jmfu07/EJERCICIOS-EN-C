/* EJERCICIO 10: TORRES DE HANOI
Funcionamiento: mueve n-1 discos a la torre auxiliar, mueve el mayor al destino y vuelve a mover n-1.
Tipos: int para discos y char para nombres de torres.
*/
#include <stdio.h>
void hanoi(int n,char o,char a,char d) {
    if(n==1) {
        printf("Mover disco 1 de %c a %c\n",o,d);
        return;
    }
    hanoi(n-1,o,d,a);
    printf("Mover disco %d de %c a %c\n",n,o,d);
    hanoi(n-1,a,o,d);
}
int main(void) {
    int n;
    printf("Discos: ");
    scanf("%d",&n);
    if(n>0&&n<16)hanoi(n,'A','B','C');
    return 0;
}
