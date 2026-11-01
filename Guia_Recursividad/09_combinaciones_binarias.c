/* EJERCICIO 9: COMBINACIONES BINARIAS
Funcionamiento: en cada posicion se prueba primero 0 y despues 1 hasta completar n.
Tipos: char[] guarda la combinacion e int controla posiciones y longitud.
*/
#include <stdio.h>
void generar(char s[],int p,int n) {
    if(p==n) {
        s[p]=0;
        printf("%s\n",s);
        return;
    }
    s[p]='0';
    generar(s,p+1,n);
    s[p]='1';
    generar(s,p+1,n);
}
int main(void) {
    char s[30];
    int n;
    printf("Longitud: ");
    scanf("%d",&n);
    if(n>=0&&n<30)generar(s,0,n);
    return 0;
}
