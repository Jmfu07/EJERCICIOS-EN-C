/* EJERCICIO 5: VALOR ABSOLUTO
Funcionamiento: si x es negativo se cambia su signo; si no, permanece igual.
Tipo: double para admitir numeros reales.
*/
#include <stdio.h>
int main(void) {
    double x;
    scanf("%lf",&x);
    if(x<0)x=-x;
    printf("%.4f\n",x);
    return 0;
}
