/* EJERCICIO 20: ELIMINAR ESPACIOS
Funcionamiento: cada caracter no blanco se copia al destino; los espacios solo se saltan.
Tipos: char[] para cadenas e int para las posiciones de lectura/escritura.
*/
#include <stdio.h>
void quitar(char o[],char d[],int i,int j) {
    if(!o[i]) {
        d[j]=0;
        return;
    }
    if(o[i]!=' '&&o[i]!='\t'&&o[i]!='\n') {
        d[j]=o[i];
        quitar(o,d,i+1,j+1);
    }else quitar(o,d,i+1,j);
}
int main(void) {
    char o[100],d[100];
    fgets(o,100,stdin);
    quitar(o,d,0,0);
    printf("%s\n",d);
    return 0;
}
