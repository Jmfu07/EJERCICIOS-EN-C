/* EJERCICIO 2: FIBONACCI
   Funcionamiento: F(0)=0 y F(1)=1. Cada termino posterior es F(n-1)+F(n-2).
   Tipos: int para n y long long para el termino calculado.
*/
#include <stdio.h>
long long fibonacci(int n) { if (n <= 1) return n; return fibonacci(n-1) + fibonacci(n-2); }
int main(void) { int n; printf("Ingrese n (0 a 40): "); scanf("%d", &n); if (n < 0 || n > 40) printf("Valor invalido.\n"); else printf("F(%d) = %lld\n", n, fibonacci(n)); return 0; }
