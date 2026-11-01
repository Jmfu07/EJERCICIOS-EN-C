/* EJERCICIO 6: EXPRESION ARITMETICA SIMPLE
Funcionamiento: se busca primero + o -, luego *, y la expresion se divide en subexpresiones.
Esta version acepta numeros enteros y los operadores +, - y *.
Tipos: char[] para la expresion e int para indices y resultado.
*/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
int eval(char s[],int i,int f) {
    int p=-1,n=0;
    char op=0;
    while(i<=f&&s[i]==' ')i++;
    while(f>=i&&s[f]==' ')f--;
    if(i>f)return 0;
    if(s[i]=='('&&s[f]==')')return eval(s,i+1,f-1);
    for(int k=f; k>=i; k--) {
        if(s[k]==')')n++;
        else if(s[k]=='(')n--;
        else if(!n&&(s[k]=='+'||s[k]=='-')&&k>i) {
            p=k;
            op=s[k];
            break;
        }
    }
    if(p<0)for(int k=f; k>=i; k--) {
        if(s[k]==')')n++;
        else if(s[k]=='(')n--;
        else if(!n&&s[k]=='*') {
            p=k;
            op='*';
            break;
        }
    }
    if(p<0) {
        char t[100];
        int j=0;
        for(int k=i; k<=f; k++)if(s[k]!=' ')t[j++]=s[k];
        t[j]=0;
        return atoi(t);
    }
    int a=eval(s,i,p-1),b=eval(s,p+1,f);
    return op=='+'?a+b:op=='-'?a-b:a*b;
}
int main(void) {
    char s[100];
    printf("Ejemplo 3+2*4: ");
    fgets(s,100,stdin);
    s[strcspn(s,"\n")]=0;
    printf("Resultado: %d\n",eval(s,0,(int)strlen(s)-1));
    return 0;
}
