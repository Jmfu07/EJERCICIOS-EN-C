/* EJERCICIO 4: INVERSION DE CADENA
Funcionamiento: se intercambian el primer y ultimo caracter y se repite hacia el centro.
Tipos: char[] almacena la cadena; int representa posiciones.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void invertir(char s[], int i, int j) {
    char t;
    if (i >= j) return;
    t=s[i];
    s[i]=s[j];
    s[j]=t;
    invertir(s,i+1,j-1);
}

int main() {
    char s[100];
    printf("Ingrese una cadena: ");
    fgets(s,100,stdin);
    s[strcspn(s,"\n")]=0;
    invertir(s,0,(int)strlen(s)-1);
    printf("Invertida: %s\n",s);
    system("pause");
    return 0;
}
