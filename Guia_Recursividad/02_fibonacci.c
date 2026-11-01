/* EJERCICIO 2: FIBONACCI
Funcionamiento: F(0)=0 y F(1)=1. Cada termino posterior es F(n-1)+F(n-2).
Tipos: int para n y long long para el termino calculado.
*/

#include <stdio.h>
#include <stdlib.h>

long long fibonacci(int n) 
{
    if (n <= 1) return n;
    return fibonacci(n-1) + fibonacci(n-2);
}

int main() 
{
    int n;
    for (int i = 0; i <= 40; i++) {
        printf("F(%d) = %lld\n", i, fibonacci(i));
    }
    
    //para encontrar el valor que quieres en su posicion
    printf("Ingrese n (0 a 40): ");
    scanf("%d", &n);
    if (n < 0 || n > 40) printf("Valor invalido.\n");
    else printf("F(%d) = %lld\n", n, fibonacci(n));
    system("pause");
    return 0;
}
