/* EJERCICIO 4: e^x
   Funcionamiento: aproxima e^x con 1+x+x^2/2!+... usando una cantidad de terminos.
   Tipos: double para x, factoriales y suma; int para terminos y ciclos.
*/
#include <stdio.h>
int main(void){double x,s=0,p=1,f=1;int n,i;scanf("%lf%d",&x,&n);for(i=0;i<n;i++){if(i) {p*=x;f*=i;}s+=p/f;}printf("e^x = %.10f\n",s);return 0;}
