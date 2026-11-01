/* EJERCICIO 11: CONTAR VOCALES
Funcionamiento: se revisa un caracter por llamada y se suma 1 si es vocal.
Tipos: char[] para texto, char para cada letra e int para contador/posicion.
*/
#include <stdio.h>
#include <ctype.h>
#include <stdlib.h>
int vocales(char s[],int p) {
    char c;
    if(!s[p])return 0;
    c=(char)tolower((unsigned char)s[p]);
    return (c=='a'||c=='e'||c=='i'||c=='o'||c=='u')+vocales(s,p+1);
}
int main(void) {
    char s[100];
    fgets(s,100,stdin);
    printf("Vocales: %d\n",vocales(s,0));
    system("pause");
    return 0;
}
