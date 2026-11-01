/* EJERCICIO 8: PALINDROMO
   Funcionamiento: compara los extremos, ignora espacios y repite con la parte interior.
   Tipos: char[] para la cadena e int para posiciones y valor logico (0 o 1).
*/
#include <stdio.h>
#include <string.h>
#include <ctype.h>
int pal(char s[],int i,int j){while(i<j&&!isalnum((unsigned char)s[i]))i++;while(i<j&&!isalnum((unsigned char)s[j]))j--;if(i>=j)return 1;if(tolower((unsigned char)s[i])!=tolower((unsigned char)s[j]))return 0;return pal(s,i+1,j-1);}
int main(void){char s[100];printf("Cadena: ");fgets(s,100,stdin);s[strcspn(s,"\n")]=0;printf(pal(s,0,(int)strlen(s)-1)?"Es palindromo\n":"No es palindromo\n");return 0;}
