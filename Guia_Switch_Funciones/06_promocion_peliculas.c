/* EJERCICIO 6: PROMOCION
Funcionamiento: de tres precios se paga la suma menos el precio mayor, es decir, las dos mas baratas.
Tipo: double para precios con centavos.
*/
#include <stdio.h>
#include <stdlib.h>
int main(void) {
    double a,b,c,m;
    scanf("%lf%lf%lf",&a,&b,&c);
    m=a;
    if(b>m)m=b;
    if(c>m)m=c;
    printf("Pagar: %.2f\n",a+b+c-m);
    system("pause");
    return 0;
}
