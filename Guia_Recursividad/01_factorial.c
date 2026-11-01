/* EJERCICIO 1: FACTORIAL
   Funcionamiento: n! es 1 cuando n=0 o n=1. Para los demas valores se calcula
   n * (n-1)!; por eso la funcion se llama a si misma.
   Tipos: int para el dato y long long para evitar desbordamiento en resultados pequenos.
*/
#include <stdio.h>
long long factorial(int n) { 
   if (n <= 1) 
   return 1; 
   return n * factorial(n - 1); 
}

int main() { 
   int n; 
   printf("Ingrese n (0 a 20): ");
   scanf("%d", &n); 
   if (n < 0 || n > 20) printf("Valor invalido.\n"); 
   else printf("%d! = %lld\n", n, factorial(n)); 
   return 0; 
}
