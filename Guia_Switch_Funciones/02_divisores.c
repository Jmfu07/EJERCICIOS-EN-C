/* EJERCICIO 2: DIVISORES
Funcionamiento: el operador % comprueba si el residuo es cero.
Tipos: int para los dos enteros y sus residuos.
*/
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    int a,b;
    scanf("%d%d",&a,&b);
    if(!a||!b)printf("No se divide entre cero\n");
    else printf(a%b==0||b%a==0?"Uno es divisor del otro\n":"No son divisores exactos\n");
    system("pause");
    return 0;
}
