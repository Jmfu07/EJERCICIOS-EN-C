/* EJERCICIO 1: INTERVALO
   Funcionamiento: se verifica si el real cumple 100 <= x <= 200.
   Tipos: double para el numero porque puede tener decimales; int no es necesario.
*/
#include <stdio.h>
int main(void){double x;scanf("%lf",&x);printf(x>=100&&x<=200?"Dentro\n":"Fuera\n");return 0;}
