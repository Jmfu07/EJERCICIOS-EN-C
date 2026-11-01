/* EJERCICIO 3: SUMA DE DIGITOS
Funcionamiento: se suma el ultimo digito n%10 y se repite con n/10 hasta quedar un digito.
Tipos: int para el numero y la suma.
*/
#include <stdio.h>
int sumaDigitos(int n) {
    if (n < 0) n = -n;
    if (n < 10) return n;
    return n % 10 + sumaDigitos(n / 10);
}
int main(void) {
    int n;
    printf("Ingrese un entero: ");
    scanf("%d", &n);
    printf("Suma = %d\n", sumaDigitos(n));
    return 0;
}
