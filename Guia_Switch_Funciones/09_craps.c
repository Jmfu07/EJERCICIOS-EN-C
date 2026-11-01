/* EJERCICIO 9: CRAPS
Funcionamiento: se lanzan dos dados. 7/11 gana, 2/3/12 pierde; otra suma se vuelve punto.
Tipos: int para dados y sumas; time_t se usa indirectamente al inicializar aleatoriedad.
*/
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
int main(void) {
    int a,b,s,p;
    srand((unsigned)time(NULL));
    a=rand()%6+1;
    b=rand()%6+1;
    s=a+b;
    printf("%d + %d = %d\n",a,b,s);
    if(s==7||s==11) {
        puts("Gana jugador");
        return 0;
    }
    if(s==2||s==3||s==12) {
        puts("Gana casa");
        return 0;
    }
    p=s;
    do {
        a=rand()%6+1;
        b=rand()%6+1;
        s=a+b;
        printf("Tirada: %d\n",s);
    }
    while(s!=p&&s!=7);
    puts(s==p?"Gana jugador":"Gana casa");
    return 0;
}
