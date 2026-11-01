/* EJERCICIO 7: CALIFICACIONES
   Funcionamiento: se acumula la suma y se actualizan minimo y maximo al leer cada nota.
   Tipos: double para notas/promedio; int para el contador.
*/
#include <stdio.h>
int main(void){double x,s=0,ma=0,mi=0;int i;for(i=0;i<4;i++){scanf("%lf",&x);s+=x;if(i==0||x>ma)ma=x;if(i==0||x<mi)mi=x;}printf("Promedio %.2f, Maxima %.2f, Minima %.2f\n",s/4,ma,mi);return 0;}
