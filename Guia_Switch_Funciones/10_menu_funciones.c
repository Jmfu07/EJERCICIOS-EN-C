/* EJERCICIO 10: MENU DE FUNCIONES
Funcionamiento: un switch selecciona clasificar pares/impares, buscar perfectos o invertir 4 digitos.
Tipos: int para enteros, sumas, opciones y posiciones.
*/
#include <stdio.h>
#include <stdlib.h>
int perfecto(int n) {
    int s=0,d;
    for(d=1; d<=n/2; d++)if(n%d==0)s+=d;
    return s==n;
}
int main(void) {
    int op,n,i,x,inv;
    do {
        printf("1 Pares/impares 2 Perfectos 3 Invertir 4 digitos 0 Salir: ");
        scanf("%d",&op);
        switch(op) {
            case 1:for(i=0; i<10; i++) {
                scanf("%d",&x);
                printf(x%2?"Impar\n":"Par\n");
            }
            break;
            case 2:for(n=1; n<=100; n++)if(perfecto(n))printf("%d\n",n);
            break;
            case 3:scanf("%d",&n);
            inv=0;
            for(i=0; i<4; i++) {
                inv=inv*10+n%10;
                n/=10;
            }
            printf("%d\n",inv);
            break;
        }
    }
    while(op);
    system("pause");
    return 0;
}
